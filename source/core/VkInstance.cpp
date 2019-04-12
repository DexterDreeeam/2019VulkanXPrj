
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> VkInstance.cpp
 *
 *  Abstract:
 *
 *    >> VkInstance.cpp
 *
 */

#include "../core/VkInstance.hpp"

_x_NS_START_

void c_vk::f_glfwInit(t_U32 width, t_U32 height, const char * title)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    m_glfwWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
}

void c_vk::f_glfwDestroy()
{
    glfwDestroyWindow(m_glfwWindow);
    glfwTerminate();
}

_x_NS_END_