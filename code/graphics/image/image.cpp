#include "graphics/image/image.hpp"

Graphics::Image::Image(VkImage _image, Borrowed):
    Unique_handle(_image)
{ }
