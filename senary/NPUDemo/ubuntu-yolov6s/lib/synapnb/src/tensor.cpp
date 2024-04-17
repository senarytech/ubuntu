/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright (C) 2020 Synaptics Incorporated. All rights reserved.
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
/// Synap tensor.
///

#include "synap/tensor.hpp"
#include "synap/logging.hpp"
#include "network_private.hpp"
#include "synap/string_utils.hpp"
#include "synap/timer.hpp"
#include "metadata.hpp"
#include <cmath>

using namespace std;

namespace synaptics {
namespace synap {


static bool to_fp32(const uint8_t* src, float* dst, size_t size, const TensorAttributes* src_attr);
static bool assign_fp16(void* dst, const uint8_t* src, size_t size, const TensorAttributes* attr);
bool assign_affine(int8_t* dst, const uint8_t* src, size_t size, const TensorAttributes* attr);
template<typename T>
bool assign_dynfp(T* dst, const uint8_t* src, size_t size, const TensorAttributes* attr);


struct Tensor::Private
{
    // Associated network
    NetworkPrivate* _np{};

    // Tensor index
    int32_t _index;

    // Tensor in/out type
    Type _type{};

    // Tensor attributes
    unique_ptr<const TensorAttributes> _attr{};

    // Default buffer (used if no external buffer assigned to the tensor)
    Buffer _default_buffer{};

    // Current data buffer if any
    Buffer* _buffer{&_default_buffer};

    // Current data buffer set to the network if any.
    // Always equivalent to _buffer except at the beginning when it is null.
    Buffer* _set_buffer{};

    // Contains dequantized data if dequantization not done by the network itself
    std::vector<float> _dequantized_data;
    
    // Scaling factor to apply when assigning scalar tensors
    double _scalar_scale{};
};


//
// Tensor
//

Tensor::Tensor(NetworkPrivate* np, int32_t ix, Type ttype, const TensorAttributes* attr): d{new Private}
{
    d->_np = np;
    d->_index = ix;
    d->_type = ttype;
    d->_attr.reset(new TensorAttributes(*attr));
    if (is_scalar()) {
        // Extract scaling factor for cropping tensors
        auto dim = format_parse::get_int(d->_attr->format, "tensor_dim", 0);
        if (dim) {
            d->_scalar_scale = 32768.0 / dim;
        }
    }
}

Tensor::Tensor(Tensor&& rhs) noexcept
{
}


Tensor::~Tensor() {}

const std::string& Tensor::name() const
{
    return d->_attr->name;
}


const Shape& Tensor::shape() const
{
    return d->_attr->shape;
}


const Dimensions Tensor::dimensions() const
{
    return Dimensions(d->_attr->shape, d->_attr->layout);
}


Layout Tensor::layout() const
{
    return d->_attr->layout;
}


string Tensor::format() const
{
    return d->_attr->format;
}


DataType Tensor::data_type() const
{
    return d->_attr->dtype;
}


Security Tensor::security() const
{
    return d->_attr->security;
}


size_t Tensor::size() const
{
    return item_count() * synap_type_size(d->_attr->dtype);
}


size_t Tensor::item_count() const {
    size_t c = 1;
    for (const auto& d : d->_attr->shape) {
        c *= d;
    }
    return c;
}


bool Tensor::is_scalar() const
{
    return d->_attr->shape.size() == 1 && d->_attr->shape[0] == 1;
}


bool Tensor::set_buffer(Buffer* buffer)
{
    if (!buffer) {
        LOGI << "Unset buffer for: " << name();
        d->_buffer = d->_set_buffer = nullptr;
        return true;
    }
    if (buffer == d->_set_buffer) {
        LOGI << "Reassigning same buffer, nothing to do: " << name();
        return true;
    }
    if (buffer->size() == 0) {
        if (!buffer->resize(size())) {
            LOGE << "Unable to resize buffer for tensor " << name() << " size: " << size();
            return false;
        }
    }
    else if (buffer->size() != size()) {
        LOGE << "Bad buffer size for " << name() << ". Expected : " << size()
             << ", got: " << buffer->size();
        return false;
    }

    bool success{};
    switch (d->_type) {
    case Type::in:
        success = d->_np->register_buffer(buffer, d->_index, true);
        break;
    case Type::out:
        success = d->_np->register_buffer(buffer, d->_index, false);
        break;
    case Type::none:
        break;
    }

    if (!success) {
        LOGE << "Failed to set buffer for tensor: " << name();
        return false;
    }

    LOGI << "Buffer set for tensor: " << name();
    d->_set_buffer = d->_buffer = buffer;
    return true;
}


Buffer* Tensor::buffer()
{
    return d->_buffer;
}


void* Tensor::data()
{
    if (d->_buffer == &d->_default_buffer && d->_buffer->size() == 0) {
        // Automatically allocate memory for default buffer
        d->_buffer->resize(size());
    }
    return d->_buffer ? d->_buffer->data() : nullptr;
}


const void* Tensor::data() const
{
    return d->_buffer ? d->_buffer->data() : nullptr;
}


bool Tensor::assign(int32_t value)
{
    if (!is_scalar()) {
        LOGE << "Not a scalar tensor: " << d->_attr->shape;
        return false;
    }
    if (d->_scalar_scale) {
        value = round(value * d->_scalar_scale);
        LOGV << "Scalar value rescaled to: " << value;
    }
    DataType synap_type = data_type();
    if (synap_type_is_integral(synap_type)) {
        return assign(&value, synap_type_size(synap_type));
    }
    LOGE << "Unable to assign value to tensor of type: " << synap_type;
    return false;
}


bool Tensor::assign(const void* data, size_t sz)
{
    if (!d->_buffer) {
        LOGE << "Tensor has no associated buffer";
        return false;
    }
    if (sz != size()) {
        LOGE << "Bad data size. Expected: " << size() << ", got: " << sz;
        return false;
    }
    return d->_buffer->assign(data, sz);
}


bool Tensor::assign(const uint8_t* in_data, size_t sz)
{
    if (!d->_buffer) {
        LOGE << "Tensor has no associated buffer";
        return false;
    }
    if (sz != item_count()) {
        LOGE << "Bad data size. Expected: " << item_count() << ", got: " << sz;
        return false;
    }

    const TensorAttributes* attr{d->_attr.get()};
    const QuantizationInfo& qi{attr->qi};
    switch(data_type()) {
    case DataType::byte:
    case DataType::uint8:
        // Direct copy (assume scale & mean matches with qi.scale & qi.zero_point)
        return d->_buffer->assign(in_data, sz);
    case DataType::int8:
        if (qi.scheme == QuantizationScheme::dynamic_fixed_point)  {
            return assign_dynfp(static_cast<int8_t*>(data()), in_data, sz, attr);
        }
        else if (qi.scheme == QuantizationScheme::affine_asymmetric) {
            return assign_affine(static_cast<int8_t*>(data()), in_data, sz, attr);
        }
        break;
    case DataType::int16:
        if (qi.scheme == QuantizationScheme::dynamic_fixed_point)  {
            return assign_dynfp(static_cast<int16_t*>(data()), in_data, sz, attr);
        }
        break;
    case DataType::float16:
        return assign_fp16(data(), in_data, sz, d->_attr.get());
    default:
        break;
    }

    LOGE << "Unable to convert to tensor data type: " << data_type();
    return false;
}


bool Tensor::assign(const Tensor& src) {
    if (!d->_buffer) {
        LOGE << "Tensor has no associated buffer";
        return false;
    }
    if (!src.d->_buffer) {
        LOGE << "Source tensor has no associated buffer";
        return false;
    }
    if (src.size() != size()) {
        LOGE << "Bad data size. Expected: " << size() << ", got: " << src.size();
        return false;
    }
    if (src.data_type() != data_type()) {
        LOGE << "Bad data type. Expected: " << data_type() << ", got: " << src.data_type();
        return false;
    }
    if (!d->_buffer->resize(size())) {
        LOGE << "Unable to resize tensor buffer";
        return false;
    }
    memcpy(data(), src.data(), size());
    return true;
}


const float* Tensor::as_float() const
{
    const void* raw_data = data();
    if (!raw_data) {
        LOGE << "Tensor contains no data";
        return nullptr;
    }

    if (data_type() == DataType::float32) {
        // Nothing to do, tensor is already in float
        return static_cast<const float*>(raw_data);
    }

    // Dequantize data
    auto raw_data_bytes = static_cast<const uint8_t*>(raw_data);
    size_t n = item_count();
    d->_dequantized_data.resize(n);
    to_fp32(raw_data_bytes, d->_dequantized_data.data(), n, d->_attr.get());
    return d->_dequantized_data.data();
}


//
// Tensors
//

const Tensor& Tensors::operator[](size_t index) const
{
    if (index >= _tensors->size()) {
        LOGE << "Invalid tensor index " << index << " of " << _tensors->size();
        static TensorAttributes null_attributes{};
        static Tensor null_tensor(nullptr, 0, {}, &null_attributes);
        return null_tensor;
    }
    return (*_tensors)[index];
}


//
// Data conversion
//


// Assume little endian memory
static inline bool get_int32(const void* src, DataType src_type, int32_t* dest)
{
    switch (src_type) {
    case DataType::int8:
        *dest = *static_cast<const int8_t*>(src);
        return true;
    case DataType::uint8:
        *dest = *static_cast<const uint8_t*>(src);
        return true;
    case DataType::int16:
        *dest = *static_cast<const int16_t*>(src);
        return true;
    case DataType::uint16:
        *dest = *static_cast<const uint16_t*>(src);
        return true;
    case DataType::int32:
        *dest = *static_cast<const int32_t*>(src);
        return true;
    case DataType::uint32:
        *dest = *static_cast<const uint32_t*>(src);
        return true;
    default:
        LOGE << "int32 conversion not supported from: " << (int)src_type;
        return false;
    }

    LOGE << "int32 conversion not supported from: " << (int)src_type;
    return false;
}


static inline float dfp_to_fp32(int32_t val, int8_t fl)
{
    float result =
        (fl > 0) ? val * (1.0f / ((float)((int64_t)1 << fl))) : val * ((float)((int64_t)1 << -fl));
    return result;
}


static inline float affine_to_fp32(int32_t val, float scale, int32_t zero_point)
{
    float data;
    data = ((float)val - zero_point) * scale;
    return data;
}


static inline float fp16_to_fp32(int16_t in)
{
    typedef union {
        uint32_t u;
        float f;
    } _fp32_t;

    constexpr _fp32_t magic = {(254 - 15) << 23};
    constexpr _fp32_t infnan = {(127 + 16) << 23};
    _fp32_t o;
    // Non-sign bits
    o.u = (in & 0x7fff) << 13;
    o.f *= magic.f;
    if (o.f >= infnan.f) {
        o.u |= 255 << 23;
    }
    // Sign bit
    o.u |= (in & 0x8000) << 16;
    return o.f;
}


static inline bool dtype_to_fp32(uint8_t* src, float* dst, const TensorAttributes* src_attr)
{
    switch (src_attr->dtype) {
    case DataType::float32:
        *dst = *(float*)src;
        return true;
    case DataType::float16:
        *dst = fp16_to_fp32(*(int16_t*)src);
        return true;
    case DataType::int8:
    case DataType::uint8:
    case DataType::int16:
    case DataType::int32: {
        int32_t src_value = 0;
        if (!get_int32(src, src_attr->dtype, &src_value))
            return false;
        switch (src_attr->qi.scheme) {
        case QuantizationScheme::dynamic_fixed_point:
            *dst = dfp_to_fp32(src_value, src_attr->qi.fractional_length);
            return true;
        case QuantizationScheme::affine_asymmetric:
            *dst = affine_to_fp32(src_value, src_attr->qi.scale_factor, src_attr->qi.zero_point);
            return true;
        case QuantizationScheme::none:
            *dst = (float)src_value;
            return true;
        }
        LOGE << "Conversion from unknown quantization scheme: " << (int)src_attr->qi.scheme;
        return false;
    } break;
    case DataType::invalid:
    case DataType::byte:
    case DataType::uint16:
    case DataType::uint32:
        LOGE << "Conversion from dtype " << (int)src_attr->dtype << " not implemented";
        return false;
    }
    LOGE << "Conversion from unknown data type: " << (int)src_attr->dtype;
    return false;
}


// Convert raw data buffer to float
bool to_fp32(const uint8_t* src, float* dst, size_t size, const TensorAttributes* src_attr)
{
    bool success = true;
    size_t stride = synap_type_size(src_attr->dtype);
    for (int32_t j = 0; j < size; j++) {
        // @todo move switch outside loop
        if (!dtype_to_fp32((uint8_t*)&src[stride * j], &dst[j], src_attr)) {
            dst[j] = 0;
            success = false;
        }
    }

    if (!success) {
        LOGE << "Conversion to FP32 failed";
    }
    return success;
}


static inline uint16_t fp32_to_fp16(float in)
{
    uint32_t fp32 = *reinterpret_cast<uint32_t*>(&in);
    uint32_t t1 = (fp32 & 0x80000000u) >> 16;  /* sign bit. */
    uint32_t t2 = (fp32 & 0x7F800000u) >> 13;  /* Exponent bits */
    uint32_t t3 = (fp32 & 0x007FE000u) >> 13;  /* Mantissa bits, no rounding */
    uint32_t fp16 = 0u;
    if( t2 >= 0x023c00u )
    {
        fp16 = t1 | 0x7BFF;     /* Don't round to infinity. */
    }
    else if( t2 <= 0x01c000u )
    {
        fp16 = t1;
    }
    else
    {
        t2 -= 0x01c000u;
        fp16 = t1 | t2 | t3;
    }
    return fp16;
}


static inline uint16_t uint8_to_fp16(uint8_t in, float mean, float scale)
{
    return fp32_to_fp16((static_cast<float>(in) - mean) / scale);
}


// Convert src data to 16bits floating-point
static bool assign_fp16(void* dst, const uint8_t* src, size_t size, const TensorAttributes* attr)
{
    Timer t;
    uint16_t* dst_fp16 = static_cast<uint16_t*>(dst);
    const uint8_t* src_end = &src[size];
    const float mean = attr->mean.size() > 0? attr->mean[0] : 0; /// @TODO use the right mean value!
    while (src != src_end) {
        *dst_fp16++ = uint8_to_fp16(*src++, mean, attr->scale);
    }
    LOGI << "Quantized data to fp16 mean:" << mean << " scale: " << attr->scale << " in " << t;
    return true;
}


// Convert src data to affine asymmetric int8
bool assign_affine(int8_t* dst, const uint8_t* src, size_t size, const TensorAttributes* attr)
{
    // Direct copy (assume scale & mean matches with qi.scale & qi.zero_point)
    Timer t;
    auto end = &dst[size];
    while(dst != end) {
        *dst++ = *src++ - 128;
    }
    LOGI << "Quantized data to asymmetric_affine " << attr->dtype << " in " << t;
    return true;
}


// Convert src data to dynamic fixed point
template<typename T>
bool assign_dynfp(T* dst, const uint8_t* src, size_t size, const TensorAttributes* attr)
{
    Timer t;
    const int mean = attr->mean.empty() ? 0 : attr->mean[0];   /// @TODO use the right mean value
    const float scale =  attr->scale;
    auto end = &dst[size];
    while(dst != end) {
        *dst++ = ((*src++ - mean) << attr->qi.fractional_length) / scale;
    }
    LOGI << "Quantized data to dynamic_fixed_point " << attr->dtype << " scale:" << scale << " in " << t;
    return true;
}


}  // namespace synap
}  // namespace synaptics
