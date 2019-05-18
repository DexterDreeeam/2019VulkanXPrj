
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> _appData.hpp
 *
 *  Abstract:
 *
 *    >> header for _appData.cpp
 *
 */

#if !defined(__DATA_HPP__)
#define __DATA_HPP__

#include "../../core/CoreGlobal.hpp"
#include "../../core/VkBase.hpp"
#include "../../core/data/Vertex.hpp"
#include "../../core/data/Model.hpp"
#include <vector>
#include <array>
#include <chrono>
#include <algorithm>

_x_NS_START_

//::std::vector<::std::vector<t_Vertex>> vertices;
//::std::vector<::std::vector<t_U16>> indices;

class c_vk_data
{
public_def:
    friend class c_vk;
    friend class c_vk_rendering;
    friend class c_vk_pipeline;
    friend class c_vk_link;
    friend class c_eventMgr;

    struct UniformBufferObject 
    {
        t_Mat4 model;
        t_Mat4 view;
        t_Mat4 proj;
    };

public_fun:
    c_vk_data(c_vk_base * base) 
        : 
        p_base(base)
        #if __CODE_START__(MIPMAP)
            , m_mipLevel(static_cast<t_U32>(-1)) 
        #endif __CODE_END__(MIPMAP)
    { }//for(auto & v : vertices) { m_vertexCounts.push_back(v.size()); } }
    ~c_vk_data() { }

private_mem:
    c_vk_base * p_base;

    #if __CODE_START__(MIPMAP)
        t_U32 m_mipLevel;
    #endif __CODE_START__(MIPMAP)

    ::std::vector<VkBuffer> m_vertexBuffers;
    ::std::vector<VkDeviceMemory> m_vertexBufferMemorys;
    ::std::vector<VkDeviceSize> m_vertexBufferOffsets;

    ::std::vector<VkBuffer> m_indexBuffers;
    ::std::vector<VkDeviceMemory> m_indexBufferMemorys;
    ::std::vector<VkDeviceSize> m_indexBufferOffsets;

    ::std::vector<VkBuffer> m_uniformBuffers;
    ::std::vector<VkDeviceMemory> m_uniformBufferMemorys;
    ::std::vector<UniformBufferObject> m_uniformObjs;

    ::std::vector<::std::vector<VkImage>> m_textureImages;
    ::std::vector<::std::vector<VkDeviceMemory>> m_textureImageMemorys;
    ::std::vector<::std::vector<VkImageView>> m_textureImageViews;
    ::std::vector<::std::vector<VkSampler>> m_textureSamplers;


    ::std::vector<t_U32> m_pointNumbers;

    ::std::vector<c_vk_model::t_model> m_models;

private_fun:
    //--------- vertex buffer ---------//
    void f_createVertexBuffer();
    void f_destroyVertexBuffer()
    {
        for(t_U32 i = 0; i != m_vertexBuffers.size(); ++i)
        {
            vkFreeMemory(p_base->m_device, m_vertexBufferMemorys[i], nullptr);
            vkDestroyBuffer(p_base->m_device, m_vertexBuffers[i], nullptr);
        }
    }

    //--------- index buffer ---------//
    void f_createIndexBuffer();
    void f_destroyIndexBuffer()
    {
        for (t_U32 i = 0; i != m_indexBuffers.size(); ++i)
        {
            vkFreeMemory(p_base->m_device, m_indexBufferMemorys[i], nullptr);
            vkDestroyBuffer(p_base->m_device, m_indexBuffers[i], nullptr);
        }
    }

    //--------- uniform buffer ---------//
    void f_createUniformObjs() { m_uniformObjs.resize(m_models.size()); }
    void f_createUniformBuffer();
    void f_destroyUniformBuffer()
    {
        for (size_t i = 0; i != m_uniformBuffers.size()/*p_base->m_imageCount*/; ++i)
        {
            vkDestroyBuffer(p_base->m_device, m_uniformBuffers[i], nullptr);
            vkFreeMemory(p_base->m_device, m_uniformBufferMemorys[i], nullptr);
        }
    }
    void f_updateUniformBuffer(/*t_U32 currentImage*/);

    //--------- textures ---------//
    void f_createTextureImage();
    void f_destroyTextureImage() 
    { 
        for(int i = 0; i != m_textureImages.size(); ++i)
        {
            for(int j = 0; j != m_textureImages[i].size(); ++j)
            {
                vkDestroyImage(p_base->m_device, m_textureImages[i][j], nullptr);
                vkFreeMemory(p_base->m_device, m_textureImageMemorys[i][j], nullptr);
            }
        }
    }
    void f_generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, t_U32 mipLevels);
    void f_createImage(
        uint32_t width, uint32_t height, t_U32 mipLevel, VkSampleCountFlagBits numSamples, VkFormat format,
        VkImageTiling tiling, VkImageUsageFlags usage,
        VkMemoryPropertyFlags properties, VkImage & image, VkDeviceMemory & imageMemory
    );
    void f_createTextureImageViews();
    void f_destroyTextureImageViews()
    {
        for(auto & vImageViews : m_textureImageViews)
        {
            for(auto & imageView : vImageViews)
            {
                vkDestroyImageView(p_base->m_device, imageView, nullptr);
            }
        }
    }
    void f_createTextureSamplers();
    void f_destroyTextureSamplers() 
    {
        for (auto & vTextureSampler : m_textureSamplers)
        {
            for (auto & textureSampler : vTextureSampler)
            {
                vkDestroySampler(p_base->m_device, textureSampler, nullptr);
            }
        }
    }

    //--------- general buffer ---------//
    void f_createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer & buffer, VkDeviceMemory & bufferMemory);
    void f_copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void f_transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, t_U32 mipLevel);
    void f_copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    VkCommandBuffer f_beginSingleTimeCommands();
    void f_endSingleTimeCommands(VkCommandBuffer commandBuffer);
};

_x_NS_END_

#endif