
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> Presentation.hpp
 *
 *  Abstract:
 *
 *    >> header for Presentation.cpp
 *
 */

#if !defined(__PRESENTATION_HPP__)
#define __PRESENTATION_HPP__

#include "../core/Macros.hpp"
#define GLFW_INCLUDE_VULKAN
#include "../../_lib_external/glfw/inc/glfw3.h"
//#include "../../_lib_external/LunargVulkan_1_1_92_1/inc/vulkan/vulkan.h"
#include <vector>
#include <algorithm>

#if __CODE_START__(DEBUG_X)
    #include "../utility/ValidationLayer.hpp"
    #include <iostream>
    #include <stdexcept>
    #include <functional>
    #include <cstring>
#endif __CODE_END__(DEBUG_X)

_x_NS_START_

extern const ::std::vector<const char * > g_deviceExtensions;

class c_vk_presentation
{
public_def:
    friend class c_vk;

    typedef struct QueueFamilyIndices
    {
        //::std::optional<uint32_t> graphicsFamily;
        t_S32 graphicsFamily = -1;
        t_S32 presentFamily = -1;
        t_Bool isComplete()
        {
            return graphicsFamily >= 0 && presentFamily >= 0;
        }
    }QueueFamilyIndices;

    typedef struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        ::std::vector<VkSurfaceFormatKHR> formats;
        ::std::vector<VkPresentModeKHR> presentModes;
    }SwapChainSupportDetails;

public_fun:
    c_vk_presentation() { };
    ~c_vk_presentation() { };

private_mem:
    t_U32 m_width, m_height;
    VkPhysicalDevice m_physicalDevice; //physical device handle -> Type: [VkPhysicalDevice]
    VkDevice m_device; //logical device handle -> Type: [VkDevice]
    VkSurfaceKHR m_surface;
    
    VkSwapchainKHR m_swapChain;
    ::std::vector<VkImage> m_swapChainImages;
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;

private_fun:
    //---------Surface---------//
    void f_createSurface(VkInstance instance, GLFWwindow * window);
    void f_destroySurface(VkInstance instance);

    //---------SawpChain---------//
    SwapChainSupportDetails f_querySwapChainSupport();
    VkSurfaceFormatKHR f_chooseSwapSurfaceFormat(const ::std::vector<VkSurfaceFormatKHR> & availableFormats);
    VkPresentModeKHR f_chooseSwapPresentMode(const ::std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D f_chooseSwapExtent(const VkSurfaceCapabilitiesKHR & capabilities);
    void f_createSwapChain();
    void f_destroySwapChain(){ vkDestroySwapchainKHR(m_device, m_swapChain, nullptr); }

    //---------Others---------//
    QueueFamilyIndices f_findQueueFamilies();
};

_x_NS_END_

#endif