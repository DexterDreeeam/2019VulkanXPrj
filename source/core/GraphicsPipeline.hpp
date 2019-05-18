
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

#include "../core/VkBase.hpp"
#include "../core/data/Data.hpp"

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
    friend class c_vk_rendering;
    friend class c_vk_link;

public_fun:
    c_vk_pipeline(c_vk_base * base, c_vk_data * data) : p_base(base), p_data(data) { }
    ~c_vk_pipeline() { }

private_mem:
    c_vk_base * p_base;
    c_vk_data * p_data;

    VkRenderPass m_renderPass;

    #if __CODE_START__(MSAA)
        VkImage m_colorImage;
        VkDeviceMemory m_colorImageMemory;
        VkImageView m_colorImageView;
    #endif __CODE_END__(MSAA)

    VkImage m_depthImage;
    VkDeviceMemory m_depthImageMemory;
    VkImageView m_depthImageView;

    ::std::vector<VkDescriptorSetLayout> m_descriptorSetLayouts;
    ::std::vector<VkDescriptorPool> m_descriptorPools;
    ::std::vector<VkDescriptorSet> m_descriptorSets;
    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_graphicsPipeline;

private_fun:
    //--------- render pass ---------//
    void f_createRenderPass();
    void f_destroyRenderPass(){ vkDestroyRenderPass(p_base->m_device, m_renderPass, nullptr); }

    //--------- color image(MSAA) ---------//
    #if __CODE_START__(MSAA)
        void f_createColorImage();
        void f_destroyColorImage()
        {
            vkDestroyImageView(p_base->m_device, m_colorImageView, nullptr);
            vkDestroyImage(p_base->m_device, m_colorImage, nullptr);
            vkFreeMemory(p_base->m_device, m_colorImageMemory, nullptr);
        }
    #endif __CODE_END__(MSAA)

    //--------- depth image ---------//
    void f_createDepthImage();
    void f_destroyDepthImage() 
    { 
        vkDestroyImageView(p_base->m_device, m_depthImageView, nullptr);
        vkDestroyImage(p_base->m_device, m_depthImage, nullptr);
        vkFreeMemory(p_base->m_device, m_depthImageMemory, nullptr);
    }
    VkFormat f_findDepthFormat();
    VkFormat f_findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    //--------- descriptor(uniform) ---------//
    void f_createDescriptorSetLayouts();
    void f_destroyDescriptorSetLayouts() 
    { 
        for(auto & layout : m_descriptorSetLayouts)
        {
            vkDestroyDescriptorSetLayout(p_base->m_device, layout, nullptr);
        } 
    }
    void f_createDescriptorPools();
    void f_destroyDescriptorPools() 
    { 
        for(auto & pool : m_descriptorPools)
        {
            vkDestroyDescriptorPool(p_base->m_device, pool, nullptr);
        }
    }
    void f_createDescriptorSets();
    void f_destroyDescriptorSets() { }

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