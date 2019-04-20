
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> Link.hpp
 *
 *  Abstract:
 *
 *    >> header for Link.cpp
 *
 */

#if !defined(__LINK_HPP__)
#define __LINK_HPP__

#include "../core/Rendering.hpp"

_x_NS_START_

class c_vk_link
{
public_def:
    friend class c_vk;

public_fun:
    c_vk_link(c_vk_base * base, c_vk_rendering * rendering) 
        : 
        p_base(base), p_rendering(rendering)
       ,currentFrame(0)
       ,m_imageAvailableSemaphores(MAX_FRAMES_IN_FLIGHT)
       ,m_renderFinishedSemaphores(MAX_FRAMES_IN_FLIGHT) 
       ,m_flightFences(MAX_FRAMES_IN_FLIGHT)
    {
    }
    ~c_vk_link() { }

    //const ::std::vector<VkBuffer> & f_getVertexBuffers() const { return p_data->m_vertexBuffers; }
    //const ::std::vector<t_U64> & f_getVertexBuffersOffsets() const { return p_data->m_vertexBufferOffsets; }
    //const ::std::vector<VkDeviceMemory> & f_getVertexBufferMemorys() const { return p_data->m_vertexBufferMemorys; }

private_mem:
    c_vk_base * p_base;
    c_vk_data * p_data;
    c_vk_rendering * p_rendering;

    /*
        semaphore be used amongst GPU queues : CPU wouldn't stop submit, submit cmd queue would cause CPU memory overflow
        fence be used between GPU queue and CPU : CPU would stop to wait for the cmd complete
    */
    const t_U32 MAX_FRAMES_IN_FLIGHT = __XVK_MAX_FRAMES_IN_FLIGHT__;
    t_U32 currentFrame;
    ::std::vector<VkSemaphore> m_imageAvailableSemaphores;
    ::std::vector<VkSemaphore> m_renderFinishedSemaphores;
    ::std::vector<VkFence> m_flightFences;

    #if __CODE_START__(WINDOW_RESIZE)
        t_Bool is_needWindowResize = false;
        t_Bool is_windowMinimize = false;
    #endif __CODE_END__(WINDOW_RESIZE)
    
private_fun:
    //---------draw---------//
    void f_drawFrame();

    //---------semaphores & fences---------//
    void f_createSemaphores();
    void f_destroySemaphores()
    {
        for(t_U32 i = 0; i != MAX_FRAMES_IN_FLIGHT; ++i)
        {
            vkDestroySemaphore(p_base->m_device, m_imageAvailableSemaphores[i], nullptr);
            vkDestroySemaphore(p_base->m_device, m_renderFinishedSemaphores[i], nullptr);
        }
    }
    void f_createFences();
    void f_destroyFences()
    {
        for (t_U32 i = 0; i != MAX_FRAMES_IN_FLIGHT; ++i)
        {
            vkDestroyFence(p_base->m_device, m_flightFences[i], nullptr);
        }
    }

    #if __CODE_START__(WINDOW_RESIZE)
        //--------- recreate swap chain ---------//
        void f_reconstruct_onWindowResize();
        void f_cleanup_onWindowResize();
    #endif __CODE_END__(WINDOW_RESIZE)

};

_x_NS_END_

#endif