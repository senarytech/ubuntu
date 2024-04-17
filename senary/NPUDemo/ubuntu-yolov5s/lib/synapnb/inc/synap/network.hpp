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
///
/// Synap Neural Network.
///

#pragma once
#include "synap/tensor.hpp"
#include <memory>
#include <string>

namespace synaptics {
namespace synap {

class NetworkPrivate;

/// Load and execute a neural network on the NPU accelerator.
class Network {
    // Implementation details
    std::unique_ptr<NetworkPrivate> d;

public:
    Network();
    Network(Network&& rhs);
    Network(const Network& rhs) = delete;
    Network& operator=(Network&& rhs);
    Network& operator=(const Network& rhs) = delete;
    ~Network();


    /// Load model.
    /// In case another model was previously loaded it is disposed before loading the one specified.
    ///
    /// @param ebg_file            path to a network executable binary graph file
    /// @param ebg_meta_file       path to the network's metadata
    /// @return                    true if success
    bool load_model(const std::string& ebg_file, const std::string& ebg_meta_file);


    /// Load model.
    /// In case another model was previously loaded it is disposed before loading the one specified.
    ///
    /// @param ebg_data            network executable binary graph data, as from e.g. fread()
    /// @param ebg_data_size       size in bytes of ebg_data
    /// @param ebg_meta_data       network's metadata (JSON-formatted text)
    /// @return                    true if success
    bool load_model(const void* ebg_data, size_t ebg_data_size, const char* ebg_meta_data);


    /// Run inference.
    /// Input data to be processed are read from input tensor(s).
    /// Inference results are generated in output tensor(s).
    ///
    /// @return true if success, false if inference failed or network not correctly initialized.
    bool predict();


    /// Collection of input tensors that can be accessed by index and iterated.
    Tensors inputs;

    /// Collection of output tensors that can be accessed by index and iterated.
    Tensors outputs;
};


/// Get synap version.
///
/// @return version number
SynapVersion synap_version();


}  // namespace synap
}  // namespace synaptics
