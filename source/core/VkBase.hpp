
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> VkBase.hpp
 *
 *  Abstract:
 *
 *    >> VkBase header file
 *
 */

#if !defined(__VK_BASE_HPP__)
#define __VK_BASE_HPP__

#define GLFW_INCLUDE_VULKAN
#include "../../_lib_external/glfw/inc/glfw3.h"
 //#include "../../_lib_external/LunargVulkan_1_1_92_1/inc/vulkan/vulkan.h"

#include "../core/Macros.hpp"

_x_NS_START_

class c_vk_base
{
public_def:
    friend class c_vk;
    friend class c_vk_presentation;
    friend class c_vk_pipeline;

private_mem:
    GLFWwindow * m_glfwWindow; //window object -> Type: [GLFWwindow *]
    VkInstance m_vkInstance; //vk instance handle -> Type: [VkInstance]
    VkPhysicalDevice m_physicalDevice; //physical device handle -> Type: [VkPhysicalDevice]
    VkDevice m_device; //logical device handle -> Type: [VkDevice]
    VkQueue m_graphicsQueue; //graphics queue in device -> Type: [VkQueue]
    VkQueue m_presentQueue; //present queue in device -> Type: [VkQueue]

    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;
};

_x_NS_END_

#endif