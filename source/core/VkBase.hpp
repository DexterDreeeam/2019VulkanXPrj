
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

#include "../core/_Macros.hpp"

_x_NS_START_

class c_vk_base
{
public_def:
    friend class c_vk;
    friend class c_vk_presentation;
    friend class c_vk_pipeline;
    friend class c_vk_rendering;
    friend class c_vk_link;

    typedef struct
    {
        //::std::optional<uint32_t> graphicsFamily;
        t_S32 graphicsFamily = -1;
        t_S32 presentFamily = -1;
        t_Bool isComplete()
        {
            return graphicsFamily >= 0 && presentFamily >= 0;
        }
    } QueueFamilyIndices;

private_mem:
    GLFWwindow * m_glfwWindow; //window object -> Type: [GLFWwindow *]
    VkInstance m_vkInstance; //vk instance handle -> Type: [VkInstance]
    VkPhysicalDevice m_physicalDevice; //physical device handle -> Type: [VkPhysicalDevice]
    VkDevice m_device; //logical device handle -> Type: [VkDevice]
    QueueFamilyIndices m_familyIndices;
    VkQueue m_graphicsQueue; //graphics queue in device -> Type: [VkQueue]
    VkQueue m_presentQueue; //present queue in device -> Type: [VkQueue]

    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;
};

_x_NS_END_

#endif