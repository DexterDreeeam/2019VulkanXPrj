
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

#define GLFW_INCLUDE_VULKAN
#include "../../_lib_external/glfw/inc/glfw3.h"
//#include "../../_lib_external/LunargVulkan_1_1_92_1/inc/vulkan/vulkan.h"

#include "../core/Macros.hpp"
#include "../core/CoreGlobal.hpp"
#include "../core/VkBase.hpp"

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
    c_vk_presentation(c_vk_base * base) : p_base(base) { };
    ~c_vk_presentation() { };

private_mem:
    c_vk_base * p_base;

    VkSurfaceKHR m_surface;
    VkSwapchainKHR m_swapChain;
    ::std::vector<VkImage> m_swapChainImages;
    ::std::vector<VkImageView> m_swapChainImageViews;

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
    void f_destroySwapChain(){ vkDestroySwapchainKHR(p_base->m_device, m_swapChain, nullptr); }

    //---------ImageView---------//
    void f_createImageViews();
    void f_destroyImageViews();

    //---------Others---------//
    QueueFamilyIndices f_findQueueFamilies();
};

_x_NS_END_

#endif