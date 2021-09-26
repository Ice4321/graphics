#pragma once

#include "graphics/device/logical.hpp"
#include "graphics/state/instance.hpp"

namespace Graphics {
    // TODO: This should probably be thread-safe in the future. Use std::atomic_ref or std::atomic?
    inline Logical_device* logical_device = nullptr;
    inline Instance* instance = nullptr;
}
