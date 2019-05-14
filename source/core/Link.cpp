
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
#if __CODE_START__(WINDOW_RESIZE)
    if(is_windowMinimize)
    {
        t_S32 width, height;
        glfwGetFramebufferSize(p_base->m_glfwWindow, &width, &height);
        if (width == 0 || height == 0)
        { 
            glfwWaitEvents();
            return;
        }
        else
        {
            is_windowMinimize = false;
            return;
        }
    }
#endif __CODE_END__(WINDOW_RESIZE)

    currentFrame = (++currentFrame) % MAX_FRAMES_IN_FLIGHT;

    //vkQueueWaitIdle(p_base->m_presentQueue); //wait prensent queue idle
    vkWaitForFences(
        p_base->m_device, 
        1, 
        &m_flightFences[currentFrame], 
        VK_TRUE, 
        ::std::numeric_limits<t_U64>::max()
        ); //wait 'currentFrame' cmd complete in graphics queue throught fence flag

    //--------------------------- CPU Wait Line ---------------------------//
    //if (is_needWindowResize)
    //{
    //    vkDeviceWaitIdle(p_base->m_device);
    //    f_reconstruct_onWindowResize();
    //    is_needWindowResize = false;
    //    return;
    //}

    t_U32 imageIndex;
    VkResult rst;
    rst = vkAcquireNextImageKHR(
        p_base->m_device, 
        p_rendering->m_presentation.m_swapChain, 
        ::std::numeric_limits<uint64_t>::max(), 
        m_imageAvailableSemaphores[currentFrame], 
        VK_NULL_HANDLE, 
        &imageIndex
        );
    #if __CODE_START__(WINDOW_RESIZE)
        if (rst == VK_ERROR_OUT_OF_DATE_KHR || rst == VK_SUBOPTIMAL_KHR || is_needWindowResize)
        {
            vkDeviceWaitIdle(p_base->m_device);
            f_reconstruct_onWindowResize();
            is_needWindowResize = false;
            if (rst == VK_ERROR_OUT_OF_DATE_KHR) return;
        }
        #if __CODE_START__(DEBUG_X)
            else if (rst != VK_SUCCESS)
            {
                throw ::std::runtime_error("<Link.cpp> Failed to acquire swap chain image!");
            }
        #endif __CODE_END__(DEBUG_X)
    #endif __CODE_END__(WINDOW_RESIZE)

    //--------- logical computing ---------//
    //p_mgr->f_updateUniform();
    p_data->f_updateUniformBuffer(/*imageIndex*/);

    vkResetFences(p_base->m_device, 1, &m_flightFences[currentFrame]);

    /*
        Wait 'm_imageAvailableSemaphores[currentFrame]'
        Sign 'm_renderFinishedSemaphores[currentFrame]' When graphics queue complete this cmd submit
        Sign 'm_flightFences[currentFrame]' When graphics queue complete this cmd submit
    */
    VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphores[currentFrame] };
    VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphores[currentFrame] };
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
    if (vkQueueSubmit(p_base->m_graphicsQueue, 1, &submitInfo, m_flightFences[currentFrame]) != VK_SUCCESS)
    {
    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<Link.cpp> Failed to submit draw command buffer!");
    #endif __CODE_END__(DEBUG_X)
    }

    /*
        Wait 'm_renderFinishedSemaphores[currentFrame]' semaphore signal
    */
    VkSwapchainKHR swapChains[] = { p_rendering->m_presentation.m_swapChain };
    VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional
    rst = vkQueuePresentKHR(p_base->m_presentQueue, &presentInfo); //submit cmd to present queue
    #if __CODE_START__(WINDOW_RESIZE)
        if (rst == VK_ERROR_OUT_OF_DATE_KHR || rst == VK_SUBOPTIMAL_KHR || is_needWindowResize)
        {
            return;
        }
        #if __CODE_START__(DEBUG_X)
            else if (rst != VK_SUCCESS)
            {
                throw ::std::runtime_error("<Link.cpp> Failed to present swap chain image!");
            }
        #endif __CODE_END__(DEBUG_X)
    #endif __CODE_END__(WINDOW_RESIZE)
}

void c_vk_link::f_createSemaphores()
{
    VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    for(t_U32 i = 0; i != MAX_FRAMES_IN_FLIGHT; ++i)
    {
        if (vkCreateSemaphore(p_base->m_device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS)
        {
        #if __CODE_START__(DEBUG_X)
            throw ::std::runtime_error("<Rendering.cpp> Failed to create semaphores!");
        #endif __CODE_END__(DEBUG_X)
        }
        if (vkCreateSemaphore(p_base->m_device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS)
        {
        #if __CODE_START__(DEBUG_X)
            throw ::std::runtime_error("<Rendering.cpp> Failed to create semaphores!");
        #endif __CODE_END__(DEBUG_X)
        }
    }
}

void c_vk_link::f_createFences()
{
    VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; //create fence in the signaled state
    for (t_U32 i = 0; i != MAX_FRAMES_IN_FLIGHT; ++i)
    {
        if (vkCreateFence(p_base->m_device, &fenceInfo, nullptr, &m_flightFences[i]) != VK_SUCCESS)
        {
        #if __CODE_START__(DEBUG_X)
            throw ::std::runtime_error("<Rendering.cpp> Failed to create Fences!");
        #endif __CODE_END__(DEBUG_X)
        }
    }
}

#if __CODE_START__(WINDOW_RESIZE)
void c_vk_link::f_reconstruct_onWindowResize()
{
    #if __CODE_START__(WINDOW_RESIZE)
        t_S32 width, height;
        glfwGetFramebufferSize(p_base->m_glfwWindow, &width, &height);
        if(width == 0 || height == 0)
        {
            is_windowMinimize = true;
            return;
        }
    #endif __CODE_END__(WINDOW_RESIZE)

    f_cleanup_onWindowResize();

    p_rendering->m_presentation.f_createSwapChain();
    p_rendering->m_presentation.f_createImages();
    p_rendering->m_presentation.f_createImageViews();

    p_rendering->m_pipeline.f_createRenderPass();
    p_rendering->m_pipeline.f_createGraphicsPipeline();
    
    p_rendering->f_createFramebuffers();

    p_data->f_createUniformBuffer();

    p_rendering->m_pipeline.f_createDescriptorPool();
    p_rendering->m_pipeline.f_createDescriptorSets();

    p_rendering->f_createCommandBuffers();
}

void c_vk_link::f_cleanup_onWindowResize()
{
    p_rendering->f_destroyFramebuffers();

    p_data->f_destroyUniformBuffer();

    p_rendering->m_pipeline.f_destroyDescriptorSets();
    p_rendering->m_pipeline.f_destroyDescriptorPool();

    p_rendering->f_reconstructCommandBuffers();
    
    p_rendering->m_pipeline.f_destroyGraphicsPipeline();
    p_rendering->m_pipeline.f_destroyRenderPass();

    p_rendering->m_presentation.f_destroyImageViews();
    p_rendering->m_presentation.f_destroySwapChain();
}
#endif __CODE_END__(WINDOW_RESIZE)

_x_NS_END_