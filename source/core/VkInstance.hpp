
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

#include "../core/Macros.hpp"

#define GLFW_INCLUDE_VULKAN
#include "../../_lib_external/glfw/inc/glfw3.h"
//#include "../../_lib_external/LunargVulkan_1_1_92_1/inc/vulkan/vulkan.h"

#include "../core/Presentation.hpp" 

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

class c_vk
{
public_def:
    friend class c_vk_presentation;

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
    typedef c_vk_presentation::QueueFamilyIndices QueueFamilyIndices;

public_fun:
    c_vk(c_vk_xdesc * vkDesc)
        : m_presentation()
    {
        f_glfwInit(&(vkDesc->glfw_xdesc));
        f_createVkInstance(&(vkDesc->vkInstance_xdesc));
    #if __CODE_START__(DEBUG_X)
        f_setupDebugCallback();
    #endif __CODE_END__(DEBUG_X)
            m_presentation.f_createSurface(m_vkInstance, m_glfwWindow);
        f_selectPhysicalDevice();
        f_createLogicalDevice();
            m_presentation.f_createSwapChain();
    }
    ~c_vk()
    {
            m_presentation.f_destroySwapChain();
        f_destroyLogicalDevice();
    #if __CODE_START__(DEBUG_X)
        ValidationDestroyDebugReportCallbackEXT(m_vkInstance, m_callback, nullptr);
    #endif __CODE_END__(DEBUG_X)
            m_presentation.f_destroySurface(m_vkInstance);
        f_destroyVkInstance();
        f_glfwDestroy();
    }

    //---------Framework---------//
    t_Bool is_glfwShouldNotClose() { return !glfwWindowShouldClose(m_glfwWindow); }
    void f_glfwPollEvents() { glfwPollEvents(); }
    void f_loopBody()
    {
        //todo
    }

private_mem:
    GLFWwindow * m_glfwWindow; //window object -> Type: [GLFWwindow *]
    VkInstance m_vkInstance; //vk instance handle -> Type: [VkInstance]
    VkPhysicalDevice m_physicalDevice; //physical device handle -> Type: [VkPhysicalDevice]
    VkDevice m_device; //logical device handle -> Type: [VkDevice]
    VkQueue m_graphicsQueue; //graphics queue in device -> Type: [VkQueue]
    VkQueue m_presentQueue; //present queue in device -> Type: [VkQueue]

    c_vk_presentation m_presentation; //presentation functions -> Type: [c_vk_presentation] -> Head: <Presentation.hpp>

    #if __CODE_START__(DEBUG_X)
        VkDebugReportCallbackEXT m_callback; //validation layer callback handle -> Type: [VkDebugReportCallbackEXT]
    #endif __CODE_START__(DEBUG_X)

private_fun:
    //---------glfw---------//
    void f_glfwInit(class c_vk_xdesc::glfw_xdesc * glfw_xdesc);
    void f_glfwDestroy()
    {
        glfwDestroyWindow(m_glfwWindow);
        glfwTerminate();
    }

    //---------Vulkan Instance---------//
    void f_createVkInstance(class c_vk_xdesc::vkInstance_xdesc * vkInstance_xdesc);
    void f_destroyVkInstance() { vkDestroyInstance(m_vkInstance, nullptr); }

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
    void f_destroyLogicalDevice(){ vkDestroyDevice(m_device, nullptr); }

    //---------Others---------//
    #if __CODE_START__(DEBUG_X)
        t_Bool is_EverythingSuitable(VkPhysicalDevice device)
        {
            SwapChainSupportDetails details = m_presentation.f_querySwapChainSupport();
            return 
                is_deviceExtensionSuitable(m_physicalDevice) && 
                is_deviceQueueFamilySuitable(m_physicalDevice) &&
                !details.formats.empty() &&
                !details.presentModes.empty();
        }
    #endif __CODE_END__(DEBUG_X)
};

_x_NS_END_

//------------------------------------

#endif