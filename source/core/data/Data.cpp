
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

const ::std::vector<::std::vector<t_Vertex>> vertices =
{
    {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{ 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{ 0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}},
    }
};

const ::std::vector<::std::vector<t_U16>> indices =
{
    {
        0, 1, 2, 2, 3, 0,
    }
};

void c_vk_data::f_createVertexBuffer()
{
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkDeviceSize vertexBufferOffset = 0;
    t_U32 vertexCount = static_cast<t_U32>(vertices[0].size());

    VkDeviceSize bufferSize = sizeof(vertices[0][0]) * vertexCount;
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    f_createBuffer(
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT, //VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer,
        stagingBufferMemory
    );

    void * data;
    vkMapMemory(p_base->m_device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices[0].data(), (size_t)bufferSize);
    vkUnmapMemory(p_base->m_device, stagingBufferMemory);

    f_createBuffer(
        bufferSize, 
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        vertexBuffer,
        vertexBufferMemory
    );

    f_copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(p_base->m_device, stagingBuffer, nullptr);
    vkFreeMemory(p_base->m_device, stagingBufferMemory, nullptr);

    m_vertexBuffers.push_back(vertexBuffer);
    m_vertexBufferMemorys.push_back(vertexBufferMemory);
    m_vertexBufferOffsets.push_back(vertexBufferOffset);
}

void c_vk_data::f_createIndexBuffer()
{
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    VkDeviceSize indexBufferOffset = 0;
    VkDeviceSize bufferSize = sizeof(indices[0][0]) * indices[0].size();
    t_U32 points = indices[0].size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    f_createBuffer(
        bufferSize, 
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
        stagingBuffer, 
        stagingBufferMemory
    );

    void * data;
    vkMapMemory(p_base->m_device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices[0].data(), (size_t)bufferSize);
    vkUnmapMemory(p_base->m_device, stagingBufferMemory);

    f_createBuffer(
        bufferSize, 
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
        indexBuffer, 
        indexBufferMemory
    );

    f_copyBuffer(stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(p_base->m_device, stagingBuffer, nullptr);
    vkFreeMemory(p_base->m_device, stagingBufferMemory, nullptr);

    m_indexBuffers.push_back(indexBuffer);
    m_indexBufferMemorys.push_back(indexBufferMemory);
    m_indexBufferOffsets.push_back(indexBufferOffset);
    m_pointNumbers.push_back(points);
}

void c_vk_data::f_createUniformBuffer()
{
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    m_uniformBuffers.resize(m_uniformObjs.size());//p_base->m_imageCount);
    m_uniformBufferMemorys.resize(m_uniformBuffers.size());//p_base->m_imageCount);

    for (size_t i = 0; i != m_uniformBuffers.size()/*p_base->m_imageCount*/; ++i)
    {
        f_createBuffer(
            bufferSize, 
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
            m_uniformBuffers[i], 
            m_uniformBufferMemorys[i]
        );
    }
}

void c_vk_data::f_updateUniformBuffer(/*t_U32 currentImage*/)
{
    /*
    void * data;
    vkMapMemory(p_base->m_device, m_uniformBufferMemorys[currentImage], 0, sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(p_base->m_device, m_uniformBufferMemorys[currentImage]);
    */

    for(t_U32 i = 0; i != m_uniformObjs.size(); ++i)
    {
        void * data;
        vkMapMemory(p_base->m_device, m_uniformBufferMemorys[i], 0, sizeof(UniformBufferObject), 0, &data);
            memcpy(data, &(m_uniformObjs[i]), sizeof(UniformBufferObject));
        vkUnmapMemory(p_base->m_device, m_uniformBufferMemorys[i]);
    }
}

void c_vk_data::f_createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer & buffer, VkDeviceMemory & bufferMemory)
{
    VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    if(vkCreateBuffer(p_base->m_device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<_appData.cpp> Failed to create buffer!");
    #endif __CODE_START__(DEBUG_X)
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(p_base->m_device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(p_base->m_physicalDevice, memRequirements.memoryTypeBits, properties);
    if(vkAllocateMemory(p_base->m_device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
    {
    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<_appData.cpp> Failed to allocate memory!");
    #endif __CODE_START__(DEBUG_X)
    }

    vkBindBufferMemory(p_base->m_device, buffer, bufferMemory, 0);
}

void c_vk_data::f_copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer;

    VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = p_base->m_commandPool;
        allocInfo.commandBufferCount = 1;
    vkAllocateCommandBuffers(p_base->m_device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);
        VkBufferCopy copyRegion = {};
            copyRegion.srcOffset = 0; // Optional
            copyRegion.dstOffset = 0; // Optional
            copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
    vkEndCommandBuffer(commandBuffer);


    VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;
    vkQueueSubmit(p_base->m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(p_base->m_graphicsQueue);

    vkFreeCommandBuffers(p_base->m_device, p_base->m_commandPool, 1, &commandBuffer);
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