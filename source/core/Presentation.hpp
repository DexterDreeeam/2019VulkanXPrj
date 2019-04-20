
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

#include "../core/VkBase.hpp"
#include "../core/data/Data.hpp"

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
    friend class c_vk_rendering;
    friend class c_vk_link;

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

    //--------- SwapChainImage & SwapChainImageView ---------//
    void f_createImages();
    void f_createImageViews();
    void f_destroyImageViews();
};

_x_NS_END_

#endif