
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> GraphicsPipeline.hpp
 *
 *  Abstract:
 *
 *    >> GraphicsPipeline header file
 *
 */

#if !defined(__GRAPHICS_PIPELINE_HPP__)
#define __GRAPHICS_PIPELINE_HPP__

#define GLFW_INCLUDE_VULKAN
#include "../../_lib_external/glfw/inc/glfw3.h"
 //#include "../../_lib_external/LunargVulkan_1_1_92_1/inc/vulkan/vulkan.h"

#include "../core/Macros.hpp"
#include "../core/VkBase.hpp"

#include <vector>
#include <string>
#include <fstream>

#if __CODE_START__(DEBUG_X)
    #include <stdexcept>
#endif __CODE_END__(DEBUG_X)

_x_NS_START_

::std::vector<char> readFile(const ::std::string & filename);

class c_vk_pipeline
{
public_def:
    friend class c_vk;

public_fun:
    c_vk_pipeline(c_vk_base * base) : p_base(base) { }
    ~c_vk_pipeline() { }

private_mem:
    c_vk_base * p_base;
    VkRenderPass m_renderPass;
    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_graphicsPipeline;

private_fun:
    //---------render pass---------//
    void f_createRenderPass();
    void f_destroyRenderPass(){ vkDestroyRenderPass(p_base->m_device, m_renderPass, nullptr); }

    //---------pipeline layout---------//
    void f_createPipelineLayout();
    void f_destroyPipelineLayout(){ vkDestroyPipelineLayout(p_base->m_device, m_pipelineLayout, nullptr); }

    //---------shader module---------//
    VkShaderModule f_createShaderModule(const ::std::vector<char> & code);
    void f_destroyShaderModule(VkShaderModule shaderModule){ vkDestroyShaderModule(p_base->m_device, shaderModule, nullptr); }

    //---------graphics pipeline---------//
    void f_createGraphicsPipeline();
    void f_destroyGraphicsPipeline(){ vkDestroyPipeline(p_base->m_device, m_graphicsPipeline, nullptr); }
    
};

_x_NS_END_

#endif