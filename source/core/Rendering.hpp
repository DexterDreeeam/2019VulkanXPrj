
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> Rendering.hpp
 *
 *  Abstract:
 *
 *    >> header for Rendering.cpp
 *
 */

#if !defined(__RENDERING_HPP__)
#define __RENDERING_HPP__

#define GLFW_INCLUDE_VULKAN
#include "../../_lib_external/glfw/inc/glfw3.h"
 //#include "../../_lib_external/LunargVulkan_1_1_92_1/inc/vulkan/vulkan.h"

#include "../core/_Macros.hpp"
#include "../core/CoreGlobal.hpp"
#include "../core/VkBase.hpp"
#include "../core/Presentation.hpp"
#include "../core/GraphicsPipeline.hpp"

#include <vector>
#include <stdexcept>

_x_NS_START_

class c_vk_rendering
{
public_def:
    friend class c_vk;
    friend class c_vk_link;

public_fun:
    c_vk_rendering(c_vk_base * base, c_vk_presentation * presentation, c_vk_pipeline * pipeline)
        :
        p_base(base)
       ,p_presentation(presentation)
       ,p_pipeline(pipeline)
    {
    
    }
    ~c_vk_rendering() { }

private_mem:
    c_vk_base * p_base;
    c_vk_presentation * p_presentation;
    c_vk_pipeline * p_pipeline;

    ::std::vector<VkFramebuffer> m_swapChainFramebuffers;
    VkCommandPool m_commandPool;
    ::std::vector<VkCommandBuffer> m_commandBuffers;

private_fun:
    //---------frame buffers---------//
    void f_createFramebuffers();
    void f_destroyFramebuffers()
    {
        for (auto framebuffer : m_swapChainFramebuffers) 
        {
            vkDestroyFramebuffer(p_base->m_device, framebuffer, nullptr);
        }
    }

    //---------command pool---------//
    void f_createCommandPool();

    void f_destroyCommandPool()
    {
        vkDestroyCommandPool(p_base->m_device, m_commandPool, nullptr);
    }

    //---------command buffers---------//
    void f_createCommandBuffers();
    #if __CODE_START__(WINDOW_RESIZE)
        void f_reconstructCommandBuffers()
        {
            vkFreeCommandBuffers(p_base->m_device, m_commandPool, static_cast<t_U32>(m_commandBuffers.size()), m_commandBuffers.data());
        }
    #endif __CODE_END__(WINDOW_RESIZE)

};

_x_NS_END_

#endif