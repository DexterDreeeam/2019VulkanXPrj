
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

#include <vector>

_x_NS_START_

class app
{
public_fun:
    app()
    { }

    void initApp()
    {
        m_mgr.f_setVk(&m_vk);
    }

    void setData(const char * vertPath, const char * fragPath, const char * objPath, ::std::vector<::std::vector<const char *>> & textures)
    {
        auto modelIndex = m_mgr.f_createNewModel();
        m_mgr.f_setVertShader(vertPath, modelIndex);
        m_mgr.f_setFragShader(fragPath, modelIndex);
        m_mgr.f_setTextures(textures, modelIndex);
        m_mgr.f_setModel(objPath, modelIndex);
    }

    void setPrinterData(const char * vertPath, const char * fragPath, const char * texturePath)
    {
        auto modelIndex = m_mgr.f_createNewModel();
        m_mgr.f_setVertShader(vertPath, modelIndex);
        m_mgr.f_setFragShader(fragPath, modelIndex);
        m_mgr.f_setPrinterTextures(texturePath, modelIndex);
        m_mgr.f_setPrinterModel(modelIndex);
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
            m_mgr.f_processPrinterInput();
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