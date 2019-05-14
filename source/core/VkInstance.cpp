
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

c_vk * g_vk = nullptr;

const ::std::vector<const char * > g_deviceExtensions =
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

void c_vk::f_setupVk(c_vk_xdesc * vkDesc)
{
    f_glfwInit(&(vkDesc->glfw_xdesc));
    f_createVkInstance(&(vkDesc->vkInstance_xdesc));
    #if __CODE_START__(DEBUG_X)
        f_setupDebugCallback();
    #endif __CODE_END__(DEBUG_X)
    
    m_rendering.m_presentation.f_createSurface(m_base.m_vkInstance, m_base.m_glfwWindow);
    
    f_selectPhysicalDevice();
    f_findQueueFamilies();
    f_createLogicalDevice();

    m_rendering.m_presentation.f_createSwapChain();
    m_rendering.m_presentation.f_createImages();
    m_rendering.m_presentation.f_createImageViews();
    
    m_rendering.m_pipeline.f_createRenderPass();
    m_rendering.m_pipeline.f_createDescriptorSetLayout();
    m_rendering.m_pipeline.f_createPipelineLayout();
    m_rendering.m_pipeline.f_createGraphicsPipeline();
    
    m_rendering.f_createFramebuffers();
    m_rendering.f_createCommandPool();

    m_data.f_createVertexBuffer();
    m_data.f_createIndexBuffer();
    m_data.f_createUniformObjs();
    m_data.f_createUniformBuffer();

    m_rendering.m_pipeline.f_createDescriptorPool();
    m_rendering.m_pipeline.f_createDescriptorSets();

    m_rendering.f_createCommandBuffers();

    m_link.f_createSemaphores();
    m_link.f_createFences();

    g_vk = this;
    //m_link.p_mgr = vkDesc->eventMgr;
}

c_vk::~c_vk()
{
    m_link.f_destroyFences();
    m_link.f_destroySemaphores();

    m_rendering.m_pipeline.f_destroyDescriptorSets();
    m_rendering.m_pipeline.f_destroyDescriptorPool();

    m_data.f_destroyUniformBuffer();
    m_data.f_destroyIndexBuffer();
    m_data.f_destroyVertexBuffer();

    m_rendering.f_destroyCommandPool();
    m_rendering.f_destroyFramebuffers();
    
    m_rendering.m_pipeline.f_destroyGraphicsPipeline();
    m_rendering.m_pipeline.f_destroyPipelineLayout();
    m_rendering.m_pipeline.f_destroyDescriptorSetLayout();
    m_rendering.m_pipeline.f_destroyRenderPass();

    m_data.f_destroyUniformBuffer();
    m_data.f_destroyIndexBuffer();
    m_data.f_destroyVertexBuffer();
    
    m_rendering.m_presentation.f_destroyImageViews();
    m_rendering.m_presentation.f_destroySwapChain();
    
    f_destroyLogicalDevice();
    #if __CODE_START__(DEBUG_X)
        ValidationDestroyDebugReportCallbackEXT(m_base.m_vkInstance, m_callback, nullptr);
    #endif __CODE_END__(DEBUG_X)
    
    m_rendering.m_presentation.f_destroySurface(m_base.m_vkInstance);
    
    f_destroyVkInstance();
    f_glfwDestroy();
}

void c_vk::f_glfwInit(class c_vk_xdesc::glfw_xdesc * glfw_xdesc)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    #if __CODE_START__(WINDOW_RESIZE)
        m_base.m_glfwWindow = glfwCreateWindow(glfw_xdesc->width, glfw_xdesc->height, glfw_xdesc->title, nullptr, nullptr);
        glfwSetFramebufferSizeCallback(m_base.m_glfwWindow, g_windowResizeCallback);
    #else
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        m_base.m_glfwWindow = glfwCreateWindow(glfw_xdesc->width, glfw_xdesc->height, glfw_xdesc->title, nullptr, nullptr);
        glfwSetWindowPos(m_base.m_glfwWindow, (mode->width - glfw_xdesc->width) / 2, (mode->height - glfw_xdesc->height) / 2);
    #endif __CODE_END__(WINDOW_RESIZE)

    m_base.m_swapChainExtent.width = glfw_xdesc->width;
    m_base.m_swapChainExtent.height = glfw_xdesc->height;
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
    {
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = vkInstance_xdesc->applicationName;
        appInfo.applicationVersion = __XVK_VERSION__;
        appInfo.pEngineName = vkInstance_xdesc->engineName;
        appInfo.engineVersion = __XVK_VERSION__;
        appInfo.apiVersion = __XVK_API_VERSION__;
    }

    VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        #if __CODE_START__(DEBUG_X)
            ::std::vector<const char *> extensions = ValidationRequiredExtensions();
            createInfo.enabledExtensionCount = static_cast<t_U32>(extensions.size());
            createInfo.ppEnabledExtensionNames = extensions.data();
            createInfo.enabledLayerCount = static_cast<t_U32>(g_validationLayers.size());
            createInfo.ppEnabledLayerNames = g_validationLayers.data();
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

    if(vkCreateInstance(&createInfo, nullptr, &m_base.m_vkInstance) != VK_SUCCESS)
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
        {
            createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
            createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
            createInfo.pfnCallback = ValidationDebugCallback;
        }
        if (ValidationCreateDebugReportCallbackEXT(m_base.m_vkInstance, &createInfo, nullptr, &m_callback) != VK_SUCCESS)
        {
            throw ::std::runtime_error("<VkInstance.cpp> Failed to set up debug callbac.");
        }
    }

#endif __CODE_END__(DEBUG_X)

void c_vk::f_selectPhysicalDevice()
{
    t_U32 deviceCount = 0;
    vkEnumeratePhysicalDevices(m_base.m_vkInstance, &deviceCount, nullptr);
    #if __CODE_START__(DEBUG_X)
        if (deviceCount == 0) 
        {
            throw ::std::runtime_error("<VkInstance.cpp> Failed to find GPUs with Vulkan support.");
        }
    #endif __CODE_END__(DEBUG_X)
    ::std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_base.m_vkInstance, &deviceCount, devices.data());
    
    ::std::multimap<int, VkPhysicalDevice> candidates;
    for (const auto & device : devices) 
    {
        int score = f_deviceSuitabilityRate(device);
        candidates.insert(::std::make_pair(score, device));
    }
    #if __CODE_START__(DEBUG_X)
        if (candidates.rbegin()->first > 0) 
        {
            m_base.m_physicalDevice = candidates.rbegin()->second;
        }
        else
        {
            throw ::std::runtime_error("<VkInstance.cpp> Failed to find a suitable GPU!");
        }
    #else
        m_base.m_physicalDevice = candidates.rbegin()->second;
    #endif __CODE_END__(DEBUG_X)
}

t_U32 c_vk::f_deviceSuitabilityRate(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    int score = 0;
    if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    {
        score += 1000;
    }
    score += deviceProperties.limits.maxImageDimension2D;
    if (!deviceFeatures.geometryShader)
    {
        return 0;
    }

    return score;
}

#if __CODE_START__(DEBUG_X)
t_Bool c_vk::is_deviceQueueFamilySuitable(VkPhysicalDevice device)
{
    QueueFamilyIndices indices = m_base.m_familyIndices;
    return indices.isComplete();
}

t_Bool c_vk::is_deviceExtensionSuitable(VkPhysicalDevice device) 
{
    t_U32 extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    ::std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    ::std::set<::std::string> requiredExtensions(g_deviceExtensions.begin(), g_deviceExtensions.end());
    for (const auto & extension : availableExtensions) 
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}
#endif __CODE_END__(DEBUG_X)

void c_vk::f_createLogicalDevice()
{
    QueueFamilyIndices indices = m_base.m_familyIndices;
    t_F32 queuePriority = 1.0f;
    ::std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    ::std::set<t_U32> uniqueQueueFamilies = 
    { 
        static_cast<t_U32>(indices.graphicsFamily), 
        static_cast<t_U32>(indices.presentFamily) 
    };
    for (uint32_t queueFamily : uniqueQueueFamilies) 
    {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        {
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};
        //deviceFeatures.samplerAnisotropy = true;

    VkDeviceCreateInfo createInfo = {};
    {
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = static_cast<t_U32>(queueCreateInfos.size());
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<t_U32>(g_deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = g_deviceExtensions.data();
        #if __CODE_START__(DEBUG_X)
            createInfo.enabledLayerCount = static_cast<t_U32>(g_validationLayers.size());
            createInfo.ppEnabledLayerNames = g_validationLayers.data();
        #else
            createInfo.enabledLayerCount = 0;
        #endif __CODE_END__(DEBUG_X)

        if (vkCreateDevice(m_base.m_physicalDevice, &createInfo, nullptr, &m_base.m_device) != VK_SUCCESS)
        {
        #if __CODE_START__(DEBUG_X)
            throw ::std::runtime_error("<VkInstance.cpp> Failed to create logical device!");
        #endif __CODE_END__(DEBUG_X)
        }
    }

    vkGetDeviceQueue(m_base.m_device, indices.graphicsFamily, 0, &(m_base.m_graphicsQueue));
    vkGetDeviceQueue(m_base.m_device, indices.presentFamily, 0, &(m_base.m_presentQueue));
}

void c_vk::f_findQueueFamilies()
{
    c_vk_base::QueueFamilyIndices indices;
    t_U32 queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_base.m_physicalDevice, &queueFamilyCount, nullptr);

    ::std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_base.m_physicalDevice, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto & queueFamily : queueFamilies)
    {
        //check if the gpu device support the queueFamily contains graphics queue
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }
        //check if the gpu device support the queueFamily contains present queue
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(m_base.m_physicalDevice, i, m_rendering.m_presentation.m_surface, &presentSupport);
        if (queueFamily.queueCount > 0 && presentSupport)
        {
            indices.presentFamily = i;
        }

        if (indices.isComplete())
        {
            break;
        }
        ++i;
    }

    m_base.m_familyIndices = indices;
}

_x_NS_END_