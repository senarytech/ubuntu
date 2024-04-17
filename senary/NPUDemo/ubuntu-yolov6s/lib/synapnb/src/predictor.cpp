#include "predictor.hpp"

#include "synap/allocator.hpp"
#include "synap/logging.hpp"
#include "synap_device.h"

using namespace std;

namespace synaptics {
namespace synap {


/// Helper class used to initalize synap device once at first use and deinitialize it at
/// program termination.
class SynapDevice {
public:
    SynapDevice() : _initialized{synap_init()}
    {
        LOGV << "Synap device initialized: " << _initialized;
    }

    ~SynapDevice()
    {
        if (_initialized) {
            LOGV << "Deinitializing Synap device";
            if (!synap_deinit()) {
                LOGE << "Failed to deinit Synap device";
            }
        }
    }

    operator bool() const { return _initialized; }

private:
    bool _initialized{};
};


bool Predictor::init()
{
    static SynapDevice synap_dev = SynapDevice();
    LOGV << "Initializing NPU";
    if (!synap_dev) {
        LOGE << "Failed to initialize synap device";
        return false;
    }
    return true;
}


bool Predictor::load_model(const void* model, size_t size)
{
    LOGV << "Preparing network nbg: size: " << size;

    if (!synap_prepare_network(model, size, &_network)) {
        LOGE << "Failed to prepare network";
        return false;
    }

    return true;
}


bool Predictor::predict()
{
    LOGV << "Predicting...";
    if (_network && !synap_run_network(_network)) {
        LOGE << "run network failed";
        return false;
    }
    return true;
}

Predictor::Predictor()
{
    LOGV << "Predictor: " << this;
}


Predictor::~Predictor()
{
    if (_network && !synap_release_network(_network)) {
        LOGE << "Failed to release network for Predictor: " << this;
    }
}


BufferAttachment Predictor::attach_buffer(uint32_t bid)
{
    BufferAttachment tensor_handle{};
    LOGV << "Attaching io buffer with bid " << bid;

    if (!synap_attach_io_buffer(_network, bid, &tensor_handle)) {
        LOGE << "Failed to attach bid: " << bid;
        return {};
    }
    return tensor_handle;
}


bool Predictor::set_buffer(int32_t index, BufferAttachment handle, bool is_input)
{
    if (!(is_input ? synap_set_input(_network, handle, index) :
                     synap_set_output(_network, handle, index))) {
        LOGE << "Failed to set " << (is_input ? "input" : "output") << " buffer " << index;
        return false;
    }
    return true;
}


bool Predictor::detach_buffer(BufferAttachment handle)
{
    if (!synap_detach_io_buffer(_network, handle)) {
        LOGE << "Failed to detach io buffer";
        return false;
    }
    return true;
}


bool Predictor::lock()
{
    return synap_lock_npu();
}


bool Predictor::unlock()
{
    return synap_unlock_npu();
}


}  // namespace synap
}  // namespace synaptics
