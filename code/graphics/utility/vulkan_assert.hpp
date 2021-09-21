#pragma once

#include "utility/assert.hpp"

#define VULKAN_ASSERT(_expression) ASSERT(_expression == VK_SUCCESS)
