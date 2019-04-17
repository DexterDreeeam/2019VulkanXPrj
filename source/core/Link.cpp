
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> Link.cpp
 *
 *  Abstract:
 *
 *    >> Connection between redering and appData
 *
 */

#include "../core/Link.hpp"

_x_NS_START_

void c_vk_link::f_drawFrame()
{
    vkQueueWaitIdle(p_base->m_presentQueue);

    t_U32 imageIndex;
    vkAcquireNextImageKHR(
        p_base->m_device, 
        p_rendering->p_presentation->m_swapChain, 
        ::std::numeric_limits<uint64_t>::max(), 
        m_imageAvailableSemaphore, 
        VK_NULL_HANDLE, 
        &imageIndex
        );

    VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphore };
    VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphore };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &(p_rendering->m_commandBuffers[imageIndex]);
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;
    if (vkQueueSubmit(p_base->m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
    {
    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<Link.cpp> Failed to submit draw command buffer!");
    #endif __CODE_END__(DEBUG_X)
    }

    VkSwapchainKHR swapChains[] = { p_rendering->p_presentation->m_swapChain };
    VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional
    vkQueuePresentKHR(p_base->m_presentQueue, &presentInfo);
}

void c_vk_link::f_createSemaphores()
{
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(p_base->m_device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphore) != VK_SUCCESS)
    {
    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<Rendering.cpp> Failed to create semaphores!");
    #endif __CODE_END__(DEBUG_X)
    }
    if (vkCreateSemaphore(p_base->m_device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphore) != VK_SUCCESS)
    {
    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<Rendering.cpp> Failed to create semaphores!");
    #endif __CODE_END__(DEBUG_X)
    }
}

_x_NS_END_