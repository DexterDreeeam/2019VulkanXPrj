
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> EventMgr.cpp
 *
 *  Abstract:
 *
 *    >> eventMgr module
 *
 */

#include "../core/EventMgr.hpp"

_x_NS_START_

void c_eventMgr::f_updateUniform()
{
    static auto startTime = ::std::chrono::high_resolution_clock::now();
    auto currentTime = ::std::chrono::high_resolution_clock::now();
    float time = ::std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    c_vk_data::UniformBufferObject ubo = {};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), (float)(p_vk->m_base.m_swapChainExtent.width) / (float)(p_vk->m_base.m_swapChainExtent.height), 0.1f, 10.0f);
        ubo.proj[1][1] *= -1;

    memcpy((void *)&(p_vk->m_data.m_uniformObjs[0]), (void *)&ubo, sizeof(c_vk_data::UniformBufferObject));
}

_x_NS_END_