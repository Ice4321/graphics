#include "graphics/image/image.hpp"

Graphics::Image::Image(Borrowed, VkImage _image, VkFormat _format, VkExtent2D _extent):
    Unique_handle(_image),
    format(_format),
    extent(_extent)
{ }


VkFormat const& Graphics::Image::get_format() const noexcept {
    return format;
}

VkExtent2D const& Graphics::Image::get_extent() const noexcept {
    return extent;
}
