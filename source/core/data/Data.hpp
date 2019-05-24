
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
#include <map>
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

    ::std::vector<::std::vector<VkBuffer>> m_vertexBuffers;
    ::std::vector<::std::vector<VkDeviceMemory>> m_vertexBufferMemorys;
    ::std::vector<::std::vector<VkDeviceSize>> m_vertexBufferOffsets;

    ::std::vector<::std::vector<VkBuffer>> m_indexBuffers;
    ::std::vector<::std::vector<VkDeviceMemory>> m_indexBufferMemorys;
    ::std::vector<::std::vector<VkDeviceSize>> m_indexBufferOffsets;

    ::std::vector<::std::vector<VkBuffer>> m_uniformBuffers;
    ::std::vector<::std::vector<VkDeviceMemory>> m_uniformBufferMemorys;
    ::std::vector<::std::vector<UniformBufferObject>> m_uniformObjs;

    ::std::vector<::std::vector<::std::vector<VkImage>>> m_textureImages;
    ::std::vector<::std::vector<::std::vector<VkDeviceMemory>>> m_textureImageMemorys;
    ::std::vector<::std::vector<::std::vector<VkImageView>>> m_textureImageViews;
    ::std::vector<::std::vector<::std::vector<VkSampler>>> m_textureSamplers;

    ::std::vector<c_vk_model::t_Model> m_models;

    t_U32 m_shapesCount = 0;

    ::std::map<char, ::std::array<t_F32, 2>> m_printerSamplerTable;
    t_U32 m_printerModelIndex;
    t_U32 m_printerInputRow = 0;
    t_U32 m_printerInputColumn = 0;

private_fun:
    //--------- vertex buffer ---------//
    void f_createVertexBuffer();
    void f_destroyVertexBuffer()
    {
        for(int i = 0; i != m_vertexBuffers.size(); ++i)
        {
            for(int j = 0; j != m_vertexBuffers[i].size(); ++j)
            {
                vkFreeMemory(p_base->m_device, m_vertexBufferMemorys[i][j], nullptr);
                vkDestroyBuffer(p_base->m_device, m_vertexBuffers[i][j], nullptr);
            }
        }
    }

    //--------- index buffer ---------//
    void f_createIndexBuffer();
    void f_destroyIndexBuffer()
    {
        for (int i = 0; i != m_indexBuffers.size(); ++i)
        {
            for(int j = 0; j != m_indexBuffers[i].size(); ++j)
            {
                vkFreeMemory(p_base->m_device, m_indexBufferMemorys[i][j], nullptr);
                vkDestroyBuffer(p_base->m_device, m_indexBuffers[i][j], nullptr);
            }
        }
    }

    //--------- uniform buffer ---------//
    void f_createUniformObjs() 
    {
        m_uniformObjs.resize(m_models.size());
        for(int i = 0; i != m_models.size(); ++i)
        {
            m_uniformObjs[i].resize(m_models[i].shapes.size());
        }
    }
    void f_createUniformBuffer();
    void f_destroyUniformBuffer()
    {
        for (int i = 0; i != m_uniformBuffers.size()/*p_base->m_imageCount*/; ++i)
        {
            for(int j = 0; j != m_uniformBuffers[i].size(); ++j)
            {
                vkDestroyBuffer(p_base->m_device, m_uniformBuffers[i][j], nullptr);
                vkFreeMemory(p_base->m_device, m_uniformBufferMemorys[i][j], nullptr);
            }
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
                for(int k = 0; k != m_textureImages[i][j].size(); ++k)
                {
                    vkDestroyImage(p_base->m_device, m_textureImages[i][j][k], nullptr);
                    vkFreeMemory(p_base->m_device, m_textureImageMemorys[i][j][k], nullptr);
                }
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
        for(auto & vvImageViews : m_textureImageViews)
        {
            for(auto & vImageViews : vvImageViews)
            {
                for(auto & imageView : vImageViews)
                {
                    vkDestroyImageView(p_base->m_device, imageView, nullptr);
                }
            }
        }
    }
    void f_createTextureSamplers();
    void f_destroyTextureSamplers() 
    {
        for (auto & vvTextureSamplers : m_textureSamplers)
        {
            for (auto & vTextureSamplers : vvTextureSamplers)
            {
                for(auto & textureSampler : vTextureSamplers)
                {
                    vkDestroySampler(p_base->m_device, textureSampler, nullptr);
                }
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