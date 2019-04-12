
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

//------------------------------------

_x_NS_START_

typedef struct
{
    t_U32 width;
    t_U32 height;
    const char * title;
}c_vk_desc;


class c_vk
{
public_fun:
    c_vk(c_vk_desc * vkDesc)
    {
        f_glfwInit(vkDesc->width, vkDesc->height, vkDesc->title);
    }
    ~c_vk()
    {
        f_glfwDestroy();
    }

    void f_glfwInit(t_U32 width, t_U32 height, const char * title);
    void f_glfwDestroy();

    bool is_glfwNotClose() { return !glfwWindowShouldClose(m_glfwWindow); }
    void f_glfwPollEvents() { glfwPollEvents(); }
    void f_loopBody()
    {
        //todo
    }

private_mem:
    GLFWwindow * m_glfwWindow;

};

_x_NS_END_

//------------------------------------

#endif