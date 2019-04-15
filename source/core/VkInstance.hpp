
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
#include "../../_lib_external/glfw/inc/glfw3.h"
#include "../../_lib_external/LunargVulkan_1_1_92_1/inc/vulkan/vulkan.h"

#if __CODE_START__(DEBUG_X)
    #include "../utility/ValidationLayer.hpp"
    #include <vector>
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
    typedef struct
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

public_fun:
    c_vk(c_vk_xdesc * vkDesc)
    {
        f_glfwInit(&(vkDesc->glfw_xdesc));
        f_createVkInstance(&(vkDesc->vkInstance_xdesc));
#if __CODE_START__(DEBUG_X)
        f_setupDebugCallback();
#endif __CODE_END__(DEBUG_X)
    }
    ~c_vk()
    {
#if __CODE_START__(DEBUG_X)
        ValidationDestroyDebugReportCallbackEXT(m_vkInstance, m_callback, nullptr);
#endif __CODE_END__(DEBUG_X)
        f_destroyVkInstance();
        f_glfwDestroy();
    }

    //---------glfw Framework---------//
    bool is_glfwShouldNotClose() { return !glfwWindowShouldClose(m_glfwWindow); }
    void f_glfwPollEvents() { glfwPollEvents(); }
    void f_loopBody()
    {
        //todo
    }

private_mem:
    GLFWwindow * m_glfwWindow; //window object -> Type: [GLFWwindow *]
    VkInstance m_vkInstance; //vk instance handle -> Type: [VkInstance_T *]

#if __CODE_START__(DEBUG_X)
    VkDebugReportCallbackEXT m_callback;
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
};

_x_NS_END_

//------------------------------------

#endif