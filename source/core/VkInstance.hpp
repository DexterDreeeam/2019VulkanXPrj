
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> VkInstance.hpp
 *
 *  Abstract:
 *
 *    >> header for VkInstance.cpp
 *
 */

#if !defined(__VK_INSTANCE_HPP__)
#define __VK_INSTANCE_HPP__

#include "../core/Link.hpp"

#include <vector>
#include <map>
#include <set>
#include <string>
//#include <optional>

#if __CODE_START__(DEBUG_X)
    #include "../utility/ValidationLayer.hpp"
    #include <iostream>
    #include <stdexcept>
    #include <functional>
    #include <cstring>
#endif __CODE_END__(DEBUG_X)

//------------------------------------

_x_NS_START_

class c_vk;

extern c_vk * g_vk;

class c_vk
{
public_def:
    friend class c_eventMgr;

    typedef struct c_vk_xdesc
    {
        struct glfw_xdesc
        {
            t_U32 width;
            t_U32 height;
            const char * title;
        } glfw_xdesc;

        struct vkInstance_xdesc
        {
            const char * applicationName;
            const char * engineName;
        } vkInstance_xdesc;

    }c_vk_xdesc;

    typedef c_vk_presentation::SwapChainSupportDetails SwapChainSupportDetails;
    typedef c_vk_base::QueueFamilyIndices QueueFamilyIndices;

public_fun:
    c_vk()
        :
        m_base() // vk base handle
        , m_data(&m_base) // app data
        , m_rendering(&m_base, &m_data)
        , m_link(&m_base, &m_rendering, &m_data) 
    { }
    void f_setupVk(c_vk_xdesc * vkDesc); //initial vk
    ~c_vk(); //clean vk

    //---------Framework---------//
    t_Bool is_glfwShouldNotClose() { return !glfwWindowShouldClose(m_base.m_glfwWindow); }
    void f_glfwPollEvents() { glfwPollEvents(); }
    void f_loopBody()
    {
        m_link.f_drawFrame();
    }
    void f_waitDeviceIdle() { vkDeviceWaitIdle(m_base.m_device); }

private_mem:
    c_vk_base m_base; //vk core base struct -> Type: [c_vk_base] -> Head: <VkBase.hpp>
    c_vk_rendering m_rendering; //rendering functions -> Type: [c_vk_rendering] -> Head: <Rendering.hpp>
    c_vk_link m_link; //link functions -> Type: [c_vk_link] -> Head: <Link.hpp>
    c_vk_data m_data; //data functions -> Type: [c_vk_data] -> Head: <_appData.hpp>

    #if __CODE_START__(DEBUG_X)
        VkDebugReportCallbackEXT m_callback; //validation layer callback handle -> Type: [VkDebugReportCallbackEXT]
    #endif __CODE_START__(DEBUG_X)

private_fun:
    //---------glfw---------//
    void f_glfwInit(class c_vk_xdesc::glfw_xdesc * glfw_xdesc);
    void f_glfwDestroy()
    {
        glfwDestroyWindow(m_base.m_glfwWindow);
        glfwTerminate();
    }

    //---------Vulkan Instance---------//
    void f_createVkInstance(class c_vk_xdesc::vkInstance_xdesc * vkInstance_xdesc);
    void f_destroyVkInstance() { vkDestroyInstance(m_base.m_vkInstance, nullptr); }

    //---------Validation---------//
    #if __CODE_START__(DEBUG_X)
        void f_setupDebugCallback();
    #endif __CODE_END__(DEBUG_X)

    //---------Device---------//
    void f_selectPhysicalDevice();
    void f_createLogicalDevice();
    t_U32 f_deviceSuitabilityRate(VkPhysicalDevice device);
    t_Bool is_deviceExtensionSuitable(VkPhysicalDevice device);
    t_Bool is_deviceQueueFamilySuitable(VkPhysicalDevice device);
    t_Bool is_deviceSamplerAnisotropySupport(VkPhysicalDevice device);
    void f_destroyLogicalDevice(){ vkDestroyDevice(m_base.m_device, nullptr); }

    //---------Others---------//
    void f_findQueueFamilies();

    #if __CODE_START__(WINDOW_RESIZE)
        static void g_windowResizeCallback(GLFWwindow * window, t_S32 width, t_S32 height)
        {
            g_vk->m_link.is_needWindowResize = true;
        }
    #endif __CODE_START__(WINDOW_RESIZE)

    #if __CODE_START__(DEBUG_X)
        t_Bool is_EverythingSuitable(VkPhysicalDevice device)
        {
            SwapChainSupportDetails details = m_rendering.m_presentation.f_querySwapChainSupport();
            return 
                is_deviceExtensionSuitable(m_base.m_physicalDevice) &&
                is_deviceQueueFamilySuitable(m_base.m_physicalDevice) &&
                is_deviceSamplerAnisotropySupport(m_base.m_physicalDevice) &&
                !details.formats.empty() &&
                !details.presentModes.empty();
        }
    #endif __CODE_END__(DEBUG_X)

    #if __CODE_START__(MSAA)
        VkSampleCountFlagBits f_getMaxUsableSampleCount();
    #endif __CODE_END__(MSAA)
};

_x_NS_END_

//------------------------------------

#endif