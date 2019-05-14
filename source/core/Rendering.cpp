
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> Rendering.cpp
 *
 *  Abstract:
 *
 *    >> Rendering.cpp
 *
 */

#include "../core/Rendering.hpp"

_x_NS_START_

void c_vk_rendering::f_createFramebuffers()
{
    m_swapChainFramebuffers.resize(m_presentation.m_swapChainImageViews.size());

    for (t_U32 i = 0; i != m_presentation.m_swapChainImageViews.size(); ++i)
    {
        VkImageView attachments[] = { m_presentation.m_swapChainImageViews[i] };

        VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = m_pipeline.m_renderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = p_base->m_swapChainExtent.width;
            framebufferInfo.height = p_base->m_swapChainExtent.height;
            framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(p_base->m_device, &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]) != VK_SUCCESS) 
        {
        #if __CODE_START__(DEBUG_X)
            throw ::std::runtime_error("<Rendering.cpp> Failed to create framebuffer!");
        #endif __CODE_END__(DEBUG_X)
        }
    }
}

void c_vk_rendering::f_createCommandPool()
{
    c_vk_base::QueueFamilyIndices queueFamilyIndices = p_base->m_familyIndices;
    VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
        poolInfo.flags = 0; // Optional

    if (vkCreateCommandPool(p_base->m_device, &poolInfo, nullptr, &(p_base->m_commandPool)) != VK_SUCCESS)
    {
    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<Rendering.cpp> Failed to create command pool!");
    #endif __CODE_END__(DEBUG_X)
    }
}

void c_vk_rendering::f_createCommandBuffers()
{
    m_commandBuffers.resize(m_swapChainFramebuffers.size());

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = p_base->m_commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (t_U32)m_commandBuffers.size();

    if (vkAllocateCommandBuffers(p_base->m_device, &allocInfo, m_commandBuffers.data()) != VK_SUCCESS)
    {
    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<Rendering.cpp> Failed to allocate command buffers!");
    #endif __CODE_END__(DEBUG_X)
    }

    for (t_U32 i = 0; i != m_commandBuffers.size(); ++i)
    {
        VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
            beginInfo.pInheritanceInfo = nullptr; // Optional
        if (vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo) != VK_SUCCESS) 
        {
        #if __CODE_START__(DEBUG_X)
            throw ::std::runtime_error("<Rendering.cpp> Failed to begin recording command buffer!");
        #endif __CODE_END__(DEBUG_X)
        }

        VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
        VkRenderPassBeginInfo renderPassInfo = {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = m_pipeline.m_renderPass;
            renderPassInfo.framebuffer = m_swapChainFramebuffers[i];
            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = p_base->m_swapChainExtent;
            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &clearColor;
        
        vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.m_graphicsPipeline);
            
            vkCmdBindVertexBuffers(
                m_commandBuffers[i], 
                0, 
                p_data->m_vertexBuffers.size(), 
                p_data->m_vertexBuffers.data(), 
                p_data->m_vertexBufferOffsets.data()
            ); //vertice
            vkCmdBindIndexBuffer(
                m_commandBuffers[i], 
                p_data->m_indexBuffers[0],
                p_data->m_indexBufferOffsets[0],
                VK_INDEX_TYPE_UINT16//VK_INDEX_TYPE_UINT16
            ); //indice
            vkCmdBindDescriptorSets(
                m_commandBuffers[i], 
                VK_PIPELINE_BIND_POINT_GRAPHICS, 
                m_pipeline.m_pipelineLayout, 
                0, 
                1, 
                &(m_pipeline.m_descriptorSets[0]/*i]*/), 
                0, 
                nullptr
            ); //uniform
            vkCmdDrawIndexed( //vkCmdDraw
                m_commandBuffers[i], 
                p_data->m_pointNumbers[0],
                1, 
                0, 
                0,
                0
            ); //draw
        
        vkCmdEndRenderPass(m_commandBuffers[i]);
        
        if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS) 
        {
        #if __CODE_START__(DEBUG_X)
            throw ::std::runtime_error("<Rendering.cpp> Failed to record command buffer!");
        #endif __CODE_END__(DEBUG_X)
        }
    }
}

_x_NS_END_