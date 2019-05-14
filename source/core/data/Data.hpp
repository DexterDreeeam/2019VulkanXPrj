
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

#include "../../core/VkBase.hpp"
#include "../../core/data/Vertex.hpp"
#include <vector>
#include <array>
#include <chrono>

_x_NS_START_

extern const ::std::vector<::std::vector<t_Vertex>> vertices;
extern const ::std::vector<::std::vector<t_U16>> indices;

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
    c_vk_data(c_vk_base * base) : p_base(base) { }//for(auto & v : vertices) { m_vertexCounts.push_back(v.size()); } }
    ~c_vk_data() { }

private_mem:
    c_vk_base * p_base;

    ::std::vector<VkBuffer> m_vertexBuffers;
    ::std::vector<VkDeviceMemory> m_vertexBufferMemorys;
    ::std::vector<VkDeviceSize> m_vertexBufferOffsets;

    ::std::vector<VkBuffer> m_indexBuffers;
    ::std::vector<VkDeviceMemory> m_indexBufferMemorys;
    ::std::vector<VkDeviceSize> m_indexBufferOffsets;

    ::std::vector<VkBuffer> m_uniformBuffers;
    ::std::vector<VkDeviceMemory> m_uniformBufferMemorys;
    ::std::vector<UniformBufferObject> m_uniformObjs;

    ::std::vector<t_U32> m_pointNumbers;

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
    void f_createUniformObjs() { m_uniformObjs.resize(vertices.size()); }
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

    //--------- general buffer ---------//
    void f_createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer & buffer, VkDeviceMemory & bufferMemory);
    void f_copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};

_x_NS_END_

#endif