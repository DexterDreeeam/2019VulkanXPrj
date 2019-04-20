
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> _appData.cpp
 *
 *  Abstract:
 *
 *    >> _appData.cpp
 *
 */

#include "../../core/data/Data.hpp"

_x_NS_START_

static t_U32 findMemoryType(VkPhysicalDevice physicalDevice, t_U32 typeFilter, VkMemoryPropertyFlags properties);

const ::std::vector<t_Vertex> vertices =
{
    {{ 0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }},
    {{ 0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f }},
    {{-0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f }}
};

void c_vk_data::f_createVertexBuffer()
{
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkDeviceSize vertexBufferOffset = 0;
    t_U32 vertexCount = static_cast<t_U32>(vertices.size());

    VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = sizeof(vertices[0]) * vertices.size();
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(p_base->m_device, &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS)
    {
    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<_appData.cpp> Failed to create vertex buffer!");
    #endif __CODE_START__(DEBUG_X)
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(p_base->m_device, vertexBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(
            p_base->m_physicalDevice,
            memRequirements.memoryTypeBits, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
            );

    if (vkAllocateMemory(p_base->m_device, &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS)
    {
    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<_appData.cpp> Failed to allocate vertex buffer memory!");
    #endif __CODE_END__(DEBUG_X)
    }

    vkBindBufferMemory(p_base->m_device, vertexBuffer, vertexBufferMemory, 0);

    void* data;
    vkMapMemory(p_base->m_device, vertexBufferMemory, 0, bufferInfo.size, 0, &data);
        memcpy(data, vertices.data(), (size_t)bufferInfo.size);
    vkUnmapMemory(p_base->m_device, vertexBufferMemory);


    m_vertexBuffers.push_back(vertexBuffer);
    m_vertexBufferMemorys.push_back(vertexBufferMemory);
    m_vertexBufferOffsets.push_back(vertexBufferOffset);
    m_vertexCounts.push_back(vertexCount);
}

static t_U32 findMemoryType(VkPhysicalDevice physicalDevice, t_U32 typeFilter, VkMemoryPropertyFlags properties) 
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (t_U32 i = 0; i != memProperties.memoryTypeCount; ++i) 
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<_appData.cpp> Failed to find suitable memory type!");
    #endif __CODE_START__(DEBUG_X)
}

_x_NS_END_