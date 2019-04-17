
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> Link.hpp
 *
 *  Abstract:
 *
 *    >> header for Link.cpp
 *
 */

#if !defined(__LINK_HPP__)
#define __LINK_HPP__

#define GLFW_INCLUDE_VULKAN
#include "../../_lib_external/glfw/inc/glfw3.h"
 //#include "../../_lib_external/LunargVulkan_1_1_92_1/inc/vulkan/vulkan.h"

#include "../core/Macros.hpp"
#include "../core/CoreGlobal.hpp"
#include "../core/VkBase.hpp"
#include "../core/Rendering.hpp"

_x_NS_START_

class c_vk_link
{
public_def:
    friend class c_vk;

public_fun:
    c_vk_link(c_vk_base * base, c_vk_rendering * rendering) : p_base(base), p_rendering(rendering) { }
    ~c_vk_link() { }

private_mem:
    c_vk_base * p_base;
    c_vk_rendering * p_rendering;

    VkSemaphore m_imageAvailableSemaphore;
    VkSemaphore m_renderFinishedSemaphore;
    
private_fun:
    //---------draw---------//
    void f_drawFrame();

    //---------semaphores---------//
    void f_createSemaphores();
    void f_destroySemaphores()
    {
        vkDestroySemaphore(p_base->m_device, m_imageAvailableSemaphore, nullptr);
        vkDestroySemaphore(p_base->m_device, m_renderFinishedSemaphore, nullptr);
    }

};

_x_NS_END_

#endif