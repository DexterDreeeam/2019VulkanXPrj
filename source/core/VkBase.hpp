
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
#include "../core/CoreGlobal.hpp"

_x_NS_START_

class c_vk_base
{
public_def:
    friend class c_vk;
    friend class c_vk_presentation;
    friend class c_vk_pipeline;
    friend class c_vk_rendering;
    friend class c_vk_link;
    friend class c_vk_data;
    friend class c_eventMgr;
    friend VkCommandBuffer beginSingleTimeCommands();
    friend void endSingleTimeCommands(VkCommandBuffer commandBuffer);

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
    VkCommandPool m_commandPool;

    t_U32 m_imageCount;
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;

    #if __CODE_START__(MSAA)
        VkSampleCountFlagBits m_msaaSamples = VK_SAMPLE_COUNT_1_BIT;
    #endif __CODE_END__(MSAA)

    const char * m_vertPath;
    const char * m_fragPath;
};

_x_NS_END_

#endif