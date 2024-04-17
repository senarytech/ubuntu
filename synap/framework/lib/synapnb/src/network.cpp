/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright (C) 2013-2020 Synaptics Incorporated. All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner
 * unless Synaptics has otherwise provided express, written
 * permission.
 *
 * Use of the materials may require a license of intellectual property
 * from a third party or from Synaptics. This file conveys no express
 * or implied licenses to any intellectual property rights belonging
 * to Synaptics.
 *
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS", AND
 * SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
 * INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
 * INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND
 * BASED ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF
 * COMPETENT JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT
 * DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY
 * TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
 */

#include "predictor.hpp"
#include "metadata.hpp"
#include "synap/network.hpp"

#include "synap/file_utils.hpp"
#include "synap/logging.hpp"
#include "synap/timer.hpp"

#include "buffer_private.hpp"
#include "network_private.hpp"

using namespace std;

namespace synaptics {
namespace synap {


//
// NetworkPrivate
//


bool NetworkPrivate::do_predict()
{
    if (!_predictor) {
        LOGE << "Network not correctly initialized";
        return false;
    }
    
    // Check that input buffers have been assigned
    for (auto& in_tensor : _inputs) {
      auto buffer = in_tensor.buffer();
      if (!buffer || !buffer->size() || !in_tensor.set_buffer(buffer)) {
            LOGE << "Input data error for tensor " << in_tensor.name();
            return false;
        }
    }

    // Be sure output buffers are allocated and assigned
    for (auto& out_tensor : _outputs) {
      out_tensor.data();
      auto buffer = out_tensor.buffer();
      if (!buffer || !buffer->size() || !out_tensor.set_buffer(buffer)) {
            LOGE << "Output buffer error for tensor: " << out_tensor.name();
            return false;
        }
    }

    LOGI << "Start inference";
    Timer tmr;

    // Flush cache for inputs
    for (auto& t : _inputs) {
        if (!t.buffer()->priv()->cache_flush()) {
            LOGE << "Cache flush failed for input: " << t.name();
            return false;
        }
    }

    bool success = _predictor->predict();
    LOGI << "Inference time: " << tmr;
    if (!success) {
        LOGE << "Inference failed";
        return false;
    }

    // Invalidate cache for outputs
    for (auto& t : _outputs) {
        if (!t.buffer()->priv()->cache_invalidate()) {
            LOGE << "Cache invalidate failed for output: " << t.name();
            return false;
        }
    }

    return true;
}

bool NetworkPrivate::register_buffer(Buffer* buffer, size_t index, bool is_input)
{
    BufferAttachment handle = buffer->priv()->handle(this);
    if (!handle) {
        const BufferPrivate* bp = buffer->priv();
        handle = _predictor->attach_buffer(bp->bid());
        if (!handle) {
            LOGE << "Can't create buffer handle for " << (is_input ? "input " : "output ") << index;
            return false;
        }
        LOGV << "Created buffer handle for " << buffer << " : " << handle;

        _buffers.insert(buffer);
        buffer->priv()->register_network(this, handle);
    }

    // Set this as current buffer
    return _predictor->set_buffer(index, handle, is_input);
}


bool NetworkPrivate::unregister_buffer(Buffer* buffer)
{
    if (_buffers.find(buffer) == _buffers.end()) {
        LOGE << "Buffer not registered " << buffer;
        return false;
    }
    _buffers.erase(buffer);

    // Be sure it is not currently used as input or output buffer
    for (auto& t : _inputs) {
        if (buffer == t.buffer()) {
            LOGI << "Detaching buffer from input tensor " << t.name();
            t.set_buffer(nullptr);
        }
    }
    for (auto& t : _outputs) {
        if (buffer == t.buffer()) {
            LOGI << "Detaching buffer from output tensor " << t.name();
            t.set_buffer(nullptr);
        }
    }

    BufferAttachment buffer_handle = buffer->priv()->handle(this);
    if (!buffer_handle) {
        LOGE << "Invalid buffer handle " << buffer;
        return false;
    }

    buffer->priv()->unregister_network(this);
    if (!_predictor->detach_buffer(buffer_handle)) {
        LOGE << "Error deleting buffer handle " << buffer;
        return false;
    }

    return true;
}


void NetworkPrivate::unregister_buffers()
{
    // Unregister all associated buffers
    while (!_buffers.empty()) {
        unregister_buffer(*_buffers.begin());
    }
}


//
// Network
//


Network::Network() : d{new NetworkPrivate()}, inputs(d->_inputs), outputs(d->_outputs) {}


Network::Network(Network&& rhs) : Network()
{
    *this = move(rhs);
}


Network& Network::operator=(Network&& rhs)
{
    swap(d, rhs.d);
    inputs.swap(rhs.inputs);
    outputs.swap(rhs.outputs);
    return *this;
}


Network::~Network()
{
    d->unregister_buffers();
}


bool Network::load_model(const std::string& ebg_file, const std::string& ebg_meta_file)
{
    vector<uint8_t> ebg = binary_file_read(ebg_file);
    if (ebg.empty()) {
        LOGE << "Failed to load ebg: " << ebg_file;
        return false;
    }
    string ebg_meta = file_read(ebg_meta_file);
    if (ebg_meta.empty()) {
        LOGE << "Failed to load metafile: " << ebg_meta_file;
        return false;
    }
    return load_model(ebg.data(), ebg.size(), ebg_meta.c_str());
}


bool Network::load_model(const void* ebg_data, size_t ebg_data_size, const char* ebg_meta_data)
{
    // Check if this looks like an ebg
    if (!ebg_data || ebg_data_size < 4) {
        LOGE << "Invalid network model";
        return false;
    }
    auto ebg_magic = static_cast<const char*>(ebg_data);
    if (strncmp(ebg_magic, "VPMN", 4) == 0) {
        LOGE << "NBG models not supported anymore. Please recompile your model.";
        return false;
    }
    if (strncmp(ebg_magic, "EBGX", 4) != 0) {
        LOGE << "Not an EBG network model.";
        return false;
    }

    Timer tmr;
    if (setenv("VSI_NN_LOG_LEVEL", "0", 0) != 0) {
        LOGW << "Failed to disable ovxlib log messages";
    }

    NetworkMetadata meta = load_metadata(ebg_meta_data);
    if (!meta.valid) {
        LOGE << "Failed to load metadata";
        return false;
    }
    LOGI << "Network inputs: " << meta.inputs.size();
    LOGI << "Network outputs: " << meta.outputs.size();

    d->unregister_buffers();
    d->_predictor = make_unique<Predictor>();
    if (!d->_predictor->init()) {
        LOGE << "Failed to initialize NPU";
        d->_predictor = nullptr;
        return false;
    }
    if (!d->_predictor->load_model(ebg_data, ebg_data_size)) {
        LOGE << "Failed to load ebg model";
        d->_predictor = nullptr;
        return false;
    }
    LOGV << "Create Neural Network: " << tmr;

    // Create input and output tensors
    d->_inputs.clear();
    d->_outputs.clear();
    d->_inputs.reserve(meta.inputs.size());
    d->_outputs.reserve(meta.outputs.size());
    for (int32_t i = 0; i < meta.inputs.size(); i++) {
        d->_inputs.emplace_back(d.get(), i, Tensor::Type::in, &meta.inputs[i]);
    }
    for (int32_t i = 0; i < meta.outputs.size(); i++) {
        d->_outputs.emplace_back(d.get(), i, Tensor::Type::out, &meta.outputs[i]);
    }

    return true;
}


bool Network::predict()
{
    return d->do_predict();
}


}  // namespace synap
}  // namespace synaptics
