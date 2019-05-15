
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> _base.hpp
 *
 *  Abstract:
 *
 *    >> xPrj base application class
 *
 */

#if !defined(___BASE_HPP__)
#define ___BASE_HPP__

//---------------------------

#include "../core/VkInstance.hpp"
#include "../core/EventMgr.hpp"

_x_NS_START_

class app
{
public_fun:
    app()
    { }

    void initApp(const char * vertPath, const char * fragPath, const char ** textures, t_U32 textureCount)
    {
        m_mgr.f_setVk(&m_vk);
        
        
        auto modelIndex = m_mgr.f_createNewModel();
            m_mgr.f_setVertShader(vertPath, modelIndex);
            m_mgr.f_setFragShader(fragPath, modelIndex);
            m_mgr.f_setTextures(textures, textureCount, modelIndex);
            m_mgr.f_setVertice();
            m_mgr.f_setIndice();
    }

    void setShaderPath(const char * vertPath, const char * fragPath, t_S32 index = -1)
    {
        if(index == -1)
        {
            
        }
    }

    void initVulkan(t_U32 width, t_U32 height, const char * title)
    {
        c_vk::c_vk_xdesc vkDesc = {};
            vkDesc.glfw_xdesc.width = width;
            vkDesc.glfw_xdesc.height = height;
            vkDesc.glfw_xdesc.title = title;
            vkDesc.vkInstance_xdesc.applicationName = __XVK_APPLICATION_NAME__;
            vkDesc.vkInstance_xdesc.engineName = __XVK_ENGINE_NAME__;
            //vkDesc.eventMgr = &m_mgr;
        m_vk.f_setupVk(&vkDesc);
    }


    void mainLoop()
    {
        while (m_vk.is_glfwShouldNotClose())
        {
            m_vk.f_glfwPollEvents();
            m_mgr.f_updateUniform();
            m_vk.f_loopBody();
        }

        m_vk.f_waitDeviceIdle();
    }

    void cleanup()
    {
    }

private_mem:
    c_vk m_vk;
    c_eventMgr m_mgr;

private_fun:

};

_x_NS_END_

//---------------------------

#endif