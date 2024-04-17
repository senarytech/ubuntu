#pragma once
#include <cstdint>
#include <stddef.h>

namespace synaptics {
namespace synap {

typedef uint32_t BufferAttachment;

/// Predictor
class Predictor {
public:
    Predictor();
    ~Predictor();

    bool init();
    bool load_model(const void* model, size_t size);
    bool predict();

    BufferAttachment attach_buffer(uint32_t bid);
    bool set_buffer(int32_t index, BufferAttachment handle, bool is_input);
    bool detach_buffer(BufferAttachment handle);

    static bool lock();
    static bool unlock();

private:
    uint32_t _network{};
};

}  // namespace synap
}  // namespace synaptics
