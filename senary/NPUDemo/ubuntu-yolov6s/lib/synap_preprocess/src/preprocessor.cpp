/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright (cc) 2013-2022 Synaptics Incorporated. All rights reserved.
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

#include "synap/preprocessor.hpp"
#include "synap/image_utils.hpp"
#include "synap/string_utils.hpp"
#include "synap/image_convert.hpp"
#include "synap/logging.hpp"
#include "synap/tensor.hpp"


#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STBI_NEON
#include <stb_image_resize.h>

using namespace std;

namespace synaptics {
namespace synap {


static int image_type_depth(ImageType image_type)
{
    int depth = 0;
    switch (image_type) {
    case ImageType::invalid:
        break;
    case ImageType::grayscale:
        depth = 1;
        break;
    case ImageType::rgb:
        depth = 3;
        break;
    case ImageType::rgba:
        depth = 4;
        break;
    }
    return depth;
}


void Preprocessor::set_roi(const Rect& roi)
{
    _roi = roi;
}


bool Preprocessor::assign(Tensor& t, const InputData& data, Rect* assigned_rect) const
{
    // Supported formats
    static constexpr char rgb[] = "rgb";
    static constexpr char bgr[] = "bgr";

    const string t_fmt = format_parse::get_type(t.format());
    Dimensions t_dim = t.dimensions();
    size_t t_item_size = synap_type_size(t.data_type());
    Dimensions in_dim = data.dimensions();
    InputType in_type = data.type();
    Shape in_shape = data.shape();
    Layout in_layout = data.layout();
    string in_format = data.format();
    const uint8_t* in_data = static_cast<const uint8_t*>(data.data());
    size_t in_size = data.size();
    vector<uint8_t> decoded_data, resized_data, nchw_data, formatted_data;
    Rect dummy_rect;
    Rect* ar = assigned_rect? assigned_rect : &dummy_rect;
    ar->origin = Dim2d{0,0};

    if (!in_data) {
        LOGE << "Unable to write to tensor, no input data";
        return false;
    }
    if (in_type == InputType::invalid) {
        LOGE << "Unable to write to tensor, invalid input data";
        return false;
    }
    if (!t.data()) {
        LOGE << "Unable to write to tensor, data buffer address not available";
        return false;
    }

    // 1. Decode image
    ImageType img_type{};
    const char* nv_name = "nv12";
    const char* uv_name = "uv8";

    switch(in_type) {
    case InputType::nv21:
        nv_name = "nv21";
        uv_name = "vu8";
    case InputType::nv12:
        // Image resize or conversion to RGB not yet supported with nv12/nv21 input.
        // We only support splitting nv12/nv21 in its y and uv/vu components.
        LOGV << "Assign " << nv_name << ": " << t_dim.n << ", " << t_dim.h << ", " << t_dim.w
             << ", " << t_dim.c;
        ar->size.x = t_dim.w;
        ar->size.y = t_dim.h;
        if (t_dim.c == 1 && (t_fmt == "y8" || t_fmt == "")) {
            // Extract y component
            auto nv_size =  (t_dim.h * t_dim.w) * 3 / 2;
            if (data.size() != nv_size) {
                LOGE << "Input size mismatch, expected " << nv_size << ", got: " << data.size();
                return false;
            }
            return image_convert_yuv420sp_to_y(in_data, t_dim.h, t_dim.w, (uint8_t*)t.data());
        }
        else if (t_dim.c == 2 && (t_fmt == uv_name || t_fmt == "")) {
            // Extract vu components
            auto nv_size =  (t_dim.h * t_dim.w) * 6;
            if (data.size() != nv_size) {
                LOGE << "Input size mismatch, expected " << nv_size << ", got: " << data.size();
                return false;
            }
            return image_convert_yuv420sp_to_uv(in_data, t_dim.h * 2, t_dim.w * 2,
                                                (uint8_t*)t.data(), t.layout() == Layout::nchw);
        }
        LOGE << "Unable to assign " << nv_name << " data to tensor with formwt: " << t_fmt;
        return false;
    case InputType::encoded_image:
        // Convert image to RGB
        in_shape = Shape{1, 0, 0, 0};
        decoded_data = image_read(in_data, in_size, &in_shape[2], &in_shape[1], &img_type);
        in_shape[3] = image_type_depth(img_type);
        in_dim = Dimensions(in_shape, Layout::nhwc);
        in_type = InputType::image_8bits;
        in_layout = Layout::nhwc;
        in_format = "rgb";
        in_data = decoded_data.data();
        in_size = decoded_data.size();
        LOGV << "Input image decoded. Shape: " << in_shape;
        break;
    default:
        // Not an encoded image
        break;
    }

    // 2. Resize input image to tensor shape
    ar->size.x = in_dim.w;
    ar->size.y = in_dim.h;
    bool is_image_8bits_nhwc = in_type == InputType::image_8bits && in_layout == Layout::nhwc;
    if (is_image_8bits_nhwc && !in_dim.empty() && !t_dim.empty() && in_dim != t_dim) {
        bool keep_proportions = format_parse::get_int(t.format(), "keep_proportions", 1);
        LOGI << "Resizing image from " << in_dim << " to " << t_dim << " keep_proportions: " << keep_proportions;
        if (in_dim.c != t_dim.c || t_dim.c < 1 || t_dim.c > STBIR_MAX_CHANNELS) {
            LOGE << "Unable to convert image from " << in_dim.c << " to " << t_dim.c << " channels";
            return false;
        }
        uint8_t* outptr = static_cast<uint8_t*>(t.data());
        bool format_match = in_format == t_fmt || in_format.empty() || t_fmt.empty();
        bool layout_match = in_layout == t.layout() || in_layout == Layout::none || t.layout() == Layout::none;
        bool item_size_match = t_item_size == 1;
        bool write_to_tensor = format_match && layout_match && item_size_match;
        if (!write_to_tensor) {
            // Alloc temporary buffer to contain resized image
            resized_data.resize(t.item_count());
            outptr = resized_data.data();
        }
        // Resize (optionally preserving input proportions)
        auto out_h = t_dim.h;
        auto in_h = in_dim.h;
        if (keep_proportions) {
            float in_ratio = (float)in_dim.w / in_dim.h;
            out_h = t_dim.w / in_ratio + 0.5;
            int band_height = t_dim.h - out_h;
            if (band_height < 0) {
                out_h = t_dim.h;
                band_height *= in_dim.w / t_dim.w;
                in_h += band_height;
                LOGW << "Proportional scaling is cutting the bottom " << - band_height << " image pixels";
            }
            else if (band_height > 0) {
                LOGI << "Proportional scaling is filling the bottom " << band_height << " tensor pixels";
                memset(&outptr[out_h * t_dim.w], 128, band_height * t_dim.w);
                ar->size.y += band_height * in_dim.w / t_dim.w;
            }
        }
        if (!stbir_resize_uint8(in_data, in_dim.w , in_h, 0, outptr, t_dim.w, out_h, 0, in_dim.c)) {
            LOGE << "Error resizing image";
            return false;
        }
#if SYNAP_DUMP_RESIZED_IMAGE
        png_file_write(outptr, "resized.png", t_dim.w, t_dim.h, ImageType::rgb);
#endif
        if (write_to_tensor) {
            // We resized directly to output tensor, nothing else to do
            LOGI << "Image resized directly to output tensor";
            return true;
        }

        // Update info about image after resize
        in_shape[1] = in_dim.h = t_dim.h;
        in_shape[2] = in_dim.w = t_dim.w;
        in_data = resized_data.data();
        in_size = resized_data.size();
        decoded_data.resize(0);
    }
    if (!in_shape.empty() && in_shape != t.shape() && in_dim != t_dim) {
        LOGE << "Shape mismatch. Data: " << in_shape << ", tensor: " << t.shape();
        return false;
    }

    // 3. Convert input to nchw if needed (here we assume raw input is 8bits)
    // At the same time we also convert format to bgr if needed.
    // (Conversion to nhwc or to rgb not yet supported)
    if (in_type == InputType::image_8bits || in_type == InputType::raw && in_size == t.item_count()) {
        if (in_layout == Layout::nhwc && t.layout() == Layout::nchw) {
            bool convert_to_bgr = in_format == rgb && t_fmt == bgr;
            LOGV << "Assign: conversion from nhwc to nchw (to_bgr: " << convert_to_bgr << ")";
            nchw_data.resize(in_size);
            if (!nhwc_to_nchw(in_shape, in_data, nchw_data.data(), convert_to_bgr)) {
                LOGE << "Data conversion to nchw failed";
                return false;
            }
            in_data = nchw_data.data();
            in_layout = t.layout();
            in_format = convert_to_bgr? bgr : in_format;
            decoded_data.resize(0);
            resized_data.resize(0);
        }
    }
    if (in_layout != Layout::none && in_layout != t.layout()) {
        LOGE << "Layout mismatch. Data: " << in_layout << ", tensor: " << t.layout();
        return false;
    }

    // 4. Convert format if possible and not yet done in the step above
    if (in_layout == Layout::nhwc && in_dim.c == 3 && t_dim.c == 3 &&
        ((in_format == rgb && t_fmt == bgr) || in_format == bgr && t_fmt == rgb)) {
        LOGV << "Assign: conversion from nhwc/" << in_format << " to nhwc/" << t_fmt;
        formatted_data.resize(in_size);
        auto inptr = in_data;
        auto endptr = &inptr[in_size];
        auto outptr = formatted_data.data();
        for (; inptr < endptr; inptr += 3) {
            *outptr++ = inptr[2];
            *outptr++ = inptr[1];
            *outptr++ = inptr[0];
        }
        in_data = formatted_data.data();
        in_format = bgr;
        decoded_data.resize(0);
        resized_data.resize(0);
        nchw_data.resize(0);
    }
    if (t_fmt != "" && in_format != "" && in_format != t_fmt) {
        LOGE << "Format mismatch. Data: " << in_format << ", tensor: " << t_fmt;
        return false;
    }

    // 5. Assign data to tensor
    if (in_type == InputType::raw) {
        if (in_size != t.size()) {
            LOGE << "Size mismatch. Data: " << in_size << ", tensor: " << t.size();
            return false;
        }
        LOGV << "Assigning raw input data to tensor";
        const void* raw_data = in_data;
        return t.assign(raw_data, in_size);
    }

    // This is an 8-bits image. Assign to tensor (this will also convert data-type if needed)
    if (in_size != t.item_count()) {
        LOGE << "Size mismatch. Data items: " << in_size << ", tensor items: " << t.item_count();
        return false;
    }
    LOGV << "Assigning input data to tensor";
    return t.assign(in_data, in_size);
}


bool Preprocessor::assign(Tensors& ts, const InputData& data, size_t start_index, Rect* assigned_rect) const
{
    InputType in_type = data.type();
    // For nv12 and nv21 the input image is split in 2 network tensors
    size_t tensor_count = in_type ==  InputType::nv12 || in_type ==  InputType::nv21? 2 : 1;

    // Assign tensor data
    size_t end_index = min(start_index + tensor_count, ts.size());
    for(size_t i = start_index; i < end_index; i++) {
        if (!assign(ts[i], data, assigned_rect)) {
            LOGE << "Failed to assign data to input tensor " << i;
            return false;
        }
    }

    // Check if cropping inputs present
    constexpr size_t crop_tensor_count = 4;
    bool has_crop_inputs = end_index + crop_tensor_count <= ts.size();
    for(size_t i = 0; has_crop_inputs && i < crop_tensor_count; i++) {
        if (!ts[end_index + i].is_scalar()) {
            has_crop_inputs = false;
        }
    }
    
    if (!has_crop_inputs) {
        if (!_roi.empty()) {
            LOGE << "No cropping tensors at index: " << end_index;
            return false;
        }
        return true;
    }

    // Assign cropping inputs
    LOGV << "Assigning cropping tensors from index: " << end_index;
    int crop_rect[crop_tensor_count] = {};
    if (_roi.empty()) {
        // Take full image
        Dimensions dim = ts[start_index].dimensions();
        crop_rect[0] = dim.w;
        crop_rect[1] = dim.h;
    }
    else {
        // Crop to specified ROI
        crop_rect[0] = _roi.size.x;
        crop_rect[1] = _roi.size.y;
        crop_rect[2] = _roi.origin.x;
        crop_rect[3] = _roi.origin.y;
        // @TODO: adjust assigned_rect
    }
    for(size_t i = 0; i < crop_tensor_count; i++) {
        if (!ts[end_index + i].assign(crop_rect[i])) {
            LOGE << "Error setting cropping tensor at index: " << end_index;
            return false;
        }
    }

    return true;
}


}  // namespace synap
}  // namespace synaptics
