
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> Presentation.cpp
 *
 *  Abstract:
 *
 *    >> presentation module for vk core
 *
 */

#include "../core/Presentation.hpp"

_x_NS_START_

const ::std::vector<const char * > g_deviceExtensions =
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

void c_vk_presentation::f_createSurface(VkInstance instance, GLFWwindow * window)
{
    if(glfwCreateWindowSurface(instance, window, nullptr, &m_surface) != VK_SUCCESS)
    {
    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<Presentation.cpp> Failed to create window surface!");
    #endif __CODE_END__(DEBUG_X)
    }
}

void c_vk_presentation::f_destroySurface(VkInstance instance)
{
    vkDestroySurfaceKHR(instance, m_surface, nullptr);
}

c_vk_presentation::QueueFamilyIndices c_vk_presentation::f_findQueueFamilies()
{
    c_vk_presentation::QueueFamilyIndices indices;
    t_U32 queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);

    ::std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto & queueFamily : queueFamilies)
    {
        //check if the gpu device support the queueFamily contains graphics queue
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }
        //check if the gpu device support the queueFamily contains present queue
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, m_surface, &presentSupport);
        if (queueFamily.queueCount > 0 && presentSupport)
        {
            indices.presentFamily = i;
        }

        if (indices.isComplete())
        {
            break;
        }
        ++i;
    }
    return indices;
}

c_vk_presentation::SwapChainSupportDetails c_vk_presentation::f_querySwapChainSupport()
{
    t_U32 formatCount;
    t_U32 presentModeCount;
    SwapChainSupportDetails details;
    {
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &details.capabilities);
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, nullptr);
        vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, nullptr);
    
        if (formatCount != 0)
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, details.formats.data());
        }
        if (presentModeCount != 0)
        {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, details.presentModes.data());
        }
    }
    return details;
}

VkSurfaceFormatKHR c_vk_presentation::f_chooseSwapSurfaceFormat(const ::std::vector<VkSurfaceFormatKHR> & availableFormats) 
{
    if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) 
    {
        return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
    }

    for (const auto & availableFormat : availableFormats)
    {
        if (
            availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM 
            && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
            ) 
        {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

VkPresentModeKHR c_vk_presentation::f_chooseSwapPresentMode(const ::std::vector<VkPresentModeKHR>& availablePresentModes)
{
    VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

    for (const auto & availablePresentMode : availablePresentModes) 
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
        {
            return availablePresentMode;
        }
        else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) 
        {
            bestMode = availablePresentMode;
        }
    }
    return bestMode;
}

VkExtent2D c_vk_presentation::f_chooseSwapExtent(const VkSurfaceCapabilitiesKHR & capabilities) 
{
    if (capabilities.currentExtent.width != ::std::numeric_limits<uint32_t>::max()) 
    {
        return capabilities.currentExtent;
    }
    else 
    {
        VkExtent2D actualExtent = { m_width, m_height };
        {
            actualExtent.width = ::std::max(capabilities.minImageExtent.width, ::std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = ::std::max(capabilities.minImageExtent.height, ::std::min(capabilities.maxImageExtent.height, actualExtent.height));
        }
        return actualExtent;
    }
}

void c_vk_presentation::f_createSwapChain()
{
    SwapChainSupportDetails swapChainSupport = f_querySwapChainSupport();

    VkSurfaceFormatKHR surfaceFormat = f_chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = f_chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = f_chooseSwapExtent(swapChainSupport.capabilities);

    t_U32 imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (
        swapChainSupport.capabilities.maxImageCount > 0 
        && imageCount > swapChainSupport.capabilities.maxImageCount
        ) 
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    c_vk_presentation::QueueFamilyIndices indices = f_findQueueFamilies();
    t_U32 queueFamilyIndices[] = { static_cast<t_U32>(indices.graphicsFamily), static_cast<t_U32>(indices.presentFamily) };
    VkSwapchainCreateInfoKHR createInfo = {};
    {
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1; //always 1 unless stereoscopic 3D application
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; //VK_IMAGE_USAGE_TRANSFER_DST_BIT instead in post-rendering
        if (indices.graphicsFamily != indices.presentFamily) 
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }
        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; //blend
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;
    }
    if (vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS) 
    {
    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<Presentation.cpp> Failed to create swap chain!");
    #endif __CODE_END__(DEBUG_X)
    }

    vkGetSwapchainImagesKHR(m_device, m_swapChain, &imageCount, nullptr);
    m_swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(m_device, m_swapChain, &imageCount, m_swapChainImages.data());

    m_swapChainImageFormat = surfaceFormat.format;
    m_swapChainExtent = extent;
}

_x_NS_END_