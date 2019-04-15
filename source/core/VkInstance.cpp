
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> VkInstance.cpp
 *
 *  Abstract:
 *
 *    >> VkInstance.cpp
 *
 */

#include "../core/VkInstance.hpp"

_x_NS_START_

void c_vk::f_glfwInit(class c_vk_xdesc::glfw_xdesc * glfw_xdesc)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    m_glfwWindow = glfwCreateWindow(glfw_xdesc->width, glfw_xdesc->height, glfw_xdesc->title, nullptr, nullptr);
}

void c_vk::f_createVkInstance(class c_vk_xdesc::vkInstance_xdesc * vkInstance_xdesc)
{

#if __CODE_START__(DEBUG_X)
    if(!ValidationLayerSupport())
    {
        throw ::std::runtime_error("<VkInstance.cpp> Validation layers requested, but not available.");
    }
#endif __CODE_END__(DEBUG_X)

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = vkInstance_xdesc->applicationName;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 1, 0);
    appInfo.pEngineName = vkInstance_xdesc->engineName;
    appInfo.engineVersion = VK_MAKE_VERSION(1, 1, 0);
    appInfo.apiVersion = VK_API_VERSION_1_1;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
#if __CODE_START__(DEBUG_X)
    ::std::vector<const char *> extensions = ValidationRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<t_U32>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.enabledLayerCount = static_cast<t_U32>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
#else
    t_U32 glfwExtensionCount = 0;
    const char ** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;
#endif __CODE_END__(DEBUG_X)

#if __CODE_START__(DEBUG_X)
    t_U32 extensionCount = 0;
    ::std::vector<VkExtensionProperties> extensionsAll;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    extensionsAll.resize(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionsAll.data());
    ::std::cout << "<VkInstance.cpp> Available extensions: " << ::std::endl;
    for(const auto & extension : extensionsAll)
    {
        ::std::cout << "\t" << extension.extensionName << ::std::endl;
    }
#endif __CODE_END__(DEBUG_X)

    if(vkCreateInstance(&createInfo, nullptr, &m_vkInstance) != VK_SUCCESS)
    {
#if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<VkInstance.cpp> Failed to create instance.");
#endif __CODE_END__(DEBUG_X)
    }
    
}

#if __CODE_START__(DEBUG_X)

void c_vk::f_setupDebugCallback()
{
    VkDebugReportCallbackCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    createInfo.pfnCallback = ValidationDebugCallback;

    if (ValidationCreateDebugReportCallbackEXT(m_vkInstance, &createInfo, nullptr, &m_callback) != VK_SUCCESS)
    {
        throw std::runtime_error("<VkInstance.cpp> Failed to set up debug callbac.");
    }
}

#endif __CODE_END__(DEBUG_X)

_x_NS_END_