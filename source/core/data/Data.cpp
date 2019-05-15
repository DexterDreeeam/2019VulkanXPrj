
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

#define STB_IMAGE_IMPLEMENTATION
#include "../../tool/stb_image.h"

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

void c_vk_data::f_createTextureImage()
{
    for(int i = 0; i != m_models.size(); ++i)
    {
        ::std::vector<VkImage> vImages;
        ::std::vector<VkDeviceMemory> vImageMemorys;

        for(int j = 0; j != m_models[i].textures.size(); ++j)
        {
            int texWidth, texHeight, texChannels;
            stbi_uc* pixels = stbi_load(m_models[i].textures[j], &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
            VkDeviceSize imageSize = texWidth * texHeight * 4;
            
            if (!pixels)
            {
            #if __CODE_START__(DEBUG_X)
                throw ::std::runtime_error("<_appData.cpp> failed to load texture image!");
            #endif __CODE_START__(DEBUG_X)
            }

            VkBuffer stagingBuffer;
            VkDeviceMemory stagingBufferMemory;
            f_createBuffer(
                imageSize,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                stagingBuffer,
                stagingBufferMemory
            );

            void * data;
            vkMapMemory(p_base->m_device, stagingBufferMemory, 0, imageSize, 0, &data);
                memcpy(data, pixels, static_cast<size_t>(imageSize));
            vkUnmapMemory(p_base->m_device, stagingBufferMemory);

            stbi_image_free(pixels);

            VkImage textureImage;
            VkDeviceMemory textureImageMemory;
            f_createImage(
                texWidth, texHeight, 
                VK_FORMAT_R8G8B8A8_UNORM, 
                VK_IMAGE_TILING_OPTIMAL, 
                VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                textureImage, textureImageMemory
            );

            f_transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
            f_copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
            f_transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

            vkDestroyBuffer(p_base->m_device, stagingBuffer, nullptr);
            vkFreeMemory(p_base->m_device, stagingBufferMemory, nullptr);

            vImages.push_back(textureImage);
            vImageMemorys.push_back(textureImageMemory);
        }

        m_textureImages.push_back(vImages);
        m_textureImageMemorys.push_back(vImageMemorys); 
    }
}

void c_vk_data::f_createImage(
    uint32_t width, uint32_t height, VkFormat format, 
    VkImageTiling tiling, VkImageUsageFlags usage, 
    VkMemoryPropertyFlags properties, VkImage & image, VkDeviceMemory & imageMemory
)
{
    VkImageCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(p_base->m_device, &imageInfo, nullptr, &image) != VK_SUCCESS)
    {
    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<_appData.cpp> Failed to create image!");
    #endif __CODE_START__(DEBUG_X)
    }

    VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(p_base->m_device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(p_base->m_physicalDevice, memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(p_base->m_device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
    {
    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<_appData.cpp> Failed to allocate image memory!");
    #endif __CODE_START__(DEBUG_X)
    }

    vkBindImageMemory(p_base->m_device, image, imageMemory, 0);
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
    VkCommandBuffer commandBuffer = f_beginSingleTimeCommands();

    VkBufferCopy copyRegion = {};
        copyRegion.size = size;
    
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    f_endSingleTimeCommands(commandBuffer);
}

void c_vk_data::f_transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) 
{
    VkCommandBuffer commandBuffer = f_beginSingleTimeCommands();

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    VkImageMemoryBarrier barrier = {};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.srcAccessMask = 0; // TODO
        barrier.dstAccessMask = 0; // TODO
        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else 
        {
        #if __CODE_START__(DEBUG_X)
            throw ::std::invalid_argument("<_appData.cpp> Unsupported layout transition!");
        #endif __CODE_START__(DEBUG_X)
        }

        vkCmdPipelineBarrier(
            commandBuffer,
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );

    f_endSingleTimeCommands(commandBuffer);
}

void c_vk_data::f_copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) 
{
    VkCommandBuffer commandBuffer = f_beginSingleTimeCommands();

    VkBufferImageCopy region = {};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = {
            width,
            height,
            1
        };

    vkCmdCopyBufferToImage(
        commandBuffer,
        buffer,
        image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
    );

    f_endSingleTimeCommands(commandBuffer);
}

VkCommandBuffer c_vk_data::f_beginSingleTimeCommands()
{
    VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = p_base->m_commandPool;
        allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(p_base->m_device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void c_vk_data::f_endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
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