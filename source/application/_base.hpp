
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

#include "../core/_Macros.hpp"
#include "../core/VkInstance.hpp"

_x_NS_START_

class app
{
public_fun:
    app()
    { }

    void initVulkan(t_U32 width, t_U32 height, const char * title)
    {
        c_vk::c_vk_xdesc vkDesc = {};
        vkDesc.glfw_xdesc.width = width;
        vkDesc.glfw_xdesc.height = height;
        vkDesc.glfw_xdesc.title = title;
        vkDesc.vkInstance_xdesc.applicationName = __XVK_APPLICATION_NAME__;
        vkDesc.vkInstance_xdesc.engineName = __XVK_ENGINE_NAME__;
        p_vk = new c_vk(&vkDesc);
    }

    void initApp()
    { }

    void mainLoop()
    {
        while (p_vk->is_glfwShouldNotClose())
        {
            p_vk->f_glfwPollEvents();
            p_vk->f_loopBody();
        }

        p_vk->f_waitDeviceIdle();
    }

    void cleanup()
    {
        delete p_vk;
    }

private_mem:
    c_vk * p_vk;

private_fun:

};

_x_NS_END_

//---------------------------

#endif