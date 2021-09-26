#include "graphics/image/image.hpp"
#include "graphics/state/globals.hpp"
#include "graphics/utility/vulkan_assert.hpp"
#include "graphics/swap_chain.hpp"

std::vector<Graphics::Image> Graphics::Image::get_swap_chain_images(Swap_chain& _swap_chain) {
    // Application must not destroy these images
    
    std::uint32_t actual_image_count;
    VULKAN_ASSERT(vkGetSwapchainImagesKHR(*logical_device, _swap_chain, &actual_image_count, nullptr)); 
    std::vector<VkImage> created_images(actual_image_count);
    VULKAN_ASSERT(vkGetSwapchainImagesKHR(*logical_device, _swap_chain, &actual_image_count, created_images.data())); 

    std::vector<Image> result;
    result.reserve(actual_image_count);
    for(auto image : created_images) result.push_back(Image(image, _swap_chain)); // push_back() because ctor is private

    return result;
}

Graphics::Image::Image(VkImage _image, Swap_chain& _swap_chain): 
    Unique_handle(_image),
    format(_swap_chain.get_image_format()),
    extent(_swap_chain.get_image_extent())
{ }

VkFormat Graphics::Image::get_format() const noexcept {
    return format;
}

VkExtent2D Graphics::Image::get_extent() const noexcept {
    return extent;
}
