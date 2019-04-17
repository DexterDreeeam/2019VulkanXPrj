
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> Validation.cpp
 *
 *  Abstract:
 *
 *    >> Insert Validation Layer in Vk
 *
 */

//------------------------------------

#include "../utility/ValidationLayer.hpp"

_x_NS_START_

#if __CODE_START__(DEBUG_X)

const ::std::vector<const char *> g_validationLayers =
{
    "VK_LAYER_LUNARG_standard_validation"
};

t_Bool ValidationLayerSupport()
{
    t_U32 layerCount;
    ::std::vector<VkLayerProperties> availableLayers;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    availableLayers.resize(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char * layerName : g_validationLayers)
    {
        bool layerFound = false;
        for (const auto & layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }
        if (!layerFound) return false;
    }

    return true;
}

::std::vector<const char *> ValidationRequiredExtensions()
{
    t_U32 glfwExtensionCount = 0;
    const char ** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    ::std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

    return extensions;
}

VKAPI_ATTR VkBool32 VKAPI_CALL ValidationDebugCallback(
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT objType,
    uint64_t obj,
    size_t location,
    int32_t code,
    const char * layerPrefix,
    const char * msg,
    void * userData
    )
{
    ::std::cerr << "Validation layer : " << msg << ::std::endl;
    return VK_FALSE;
}

VkResult ValidationCreateDebugReportCallbackEXT(
    VkInstance instance,
    const VkDebugReportCallbackCreateInfoEXT * pCreateInfo,
    const VkAllocationCallbacks * pAllocator,
    VkDebugReportCallbackEXT * pCallback
    )
{
    auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
    if (func != nullptr)
    {
        return func(instance, pCreateInfo, pAllocator, pCallback);
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void ValidationDestroyDebugReportCallbackEXT(
    VkInstance instance,
    VkDebugReportCallbackEXT callback,
    const VkAllocationCallbacks * pAllocator
    )
{
    auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    if (func != nullptr)
    {
        func(instance, callback, pAllocator);
    }
}

#endif __CODE_END__(DEBUG_X)

_x_NS_END_

//------------------------------------