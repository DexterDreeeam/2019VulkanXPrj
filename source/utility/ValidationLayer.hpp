
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> Validation.hpp
 *
 *  Abstract:
 *
 *    >> Insert Validation Layer in Vk
 *
 */

//------------------------------------

#if !defined(__VALIDATION_LAYER_HPP__)
#define __VALIDATION_LAYER_HPP__

#define GLFW_INCLUDE_VULKAN
#include "../../_lib_external/glfw/inc/glfw3.h"
//#include "../../_lib_external/LunargVulkan_1_1_92_1/inc/vulkan/vulkan.h"

#include "../core/Macros.hpp"
#include "../core/CoreGlobal.hpp"
#include <vector>

#if __CODE_START__(DEBUG_X)
    #include <iostream>
#endif __CODE_END__(DEBUG_X)

_x_NS_START_

t_Bool ValidationLayerSupport();

::std::vector<const char *> ValidationRequiredExtensions();

VKAPI_ATTR VkBool32 VKAPI_CALL ValidationDebugCallback(
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT objType,
    uint64_t obj,
    size_t location,
    int32_t code,
    const char * layerPrefix,
    const char * msg,
    void * userData
    );

VkResult ValidationCreateDebugReportCallbackEXT(
    VkInstance instance,
    const VkDebugReportCallbackCreateInfoEXT * pCreateInfo,
    const VkAllocationCallbacks * pAllocator,
    VkDebugReportCallbackEXT * pCallback
    );

void ValidationDestroyDebugReportCallbackEXT(
    VkInstance instance,
    VkDebugReportCallbackEXT callback,
    const VkAllocationCallbacks * pAllocator
    );

_x_NS_END_

//------------------------------------

#endif