#pragma once

#include "graphics/device/logical.hpp"
#include "graphics/state/instance.hpp"

namespace Graphics {
    // These are pointers so that the objects can be stack-allocated in main() for automatic clean-up
    //inline Logical_device* logical_device = nullptr;
    //inline Instance* instance = nullptr;

    // TODO: This should probably be thread-safe in the future. Use std::atomic_ref or std::atomic?
}
