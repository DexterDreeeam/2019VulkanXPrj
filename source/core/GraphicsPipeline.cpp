
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> GraphicsPipeline.cpp
 *
 *  Abstract:
 *
 *    >> GraphicsPipeline module
 *
 */

#include "../core/GraphicsPipeline.hpp"

_x_NS_START_

void c_vk_pipeline::f_createRenderPass()
{
    VkAttachmentDescription colorAttachment = {};
        colorAttachment.format = p_base->m_swapChainImageFormat;
        #if __CODE_START__(MSAA)
            colorAttachment.samples = p_base->m_msaaSamples;
        #else
            colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        #endif __CODE_END__(MSAA)
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        #if __CODE_START__(MSAA)
            colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        #else
            colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        #endif __CODE_END__(MSAA)

    VkAttachmentReference colorAttachmentRef = {};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depthAttachment = {};
        depthAttachment.format = f_findDepthFormat();
        #if __CODE_START__(MSAA)
            depthAttachment.samples = p_base->m_msaaSamples;
        #else
            depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        #endif __CODE_START__(MSAA)
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef = {};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    #if __CODE_START__(MSAA)
        VkAttachmentDescription colorAttachmentResolve = {};
            colorAttachmentResolve.format = p_base->m_swapChainImageFormat;
            colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
            colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentResolveRef = {};
            colorAttachmentResolveRef.attachment = 2;
            colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    #endif __CODE_END__(MSAA)

    VkSubpassDescription subpass = {}; //subpasses in render pass
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;
        #if __CODE_START__(MSAA)
            subpass.pResolveAttachments = &colorAttachmentResolveRef;
        #endif __CODE_END__(MSAA)

    VkSubpassDependency dependency = {}; //links between subpasses
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    #if __CODE_START__(MSAA)
        ::std::array<VkAttachmentDescription, 3> attachments = { colorAttachment, depthAttachment, colorAttachmentResolve };
    #else
        ::std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
    #endif __CODE_END__(MSAA)

    VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());;
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(p_base->m_device, &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS) 
    {
    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<GraphicsPipeline.cpp> Failed to create render pass!");
    #endif __CODE_END__(DEBUG_X)
    }
}

#if __CODE_START__(MSAA)
    void c_vk_pipeline::f_createColorImage()
    {
        VkFormat colorFormat = p_base->m_swapChainImageFormat;

        p_data->f_createImage(
            p_base->m_swapChainExtent.width, 
            p_base->m_swapChainExtent.height,
            1, p_base->m_msaaSamples, colorFormat, 
            VK_IMAGE_TILING_OPTIMAL, 
            VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, 
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
            m_colorImage, m_colorImageMemory
        );

        VkImageViewCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = m_colorImage;
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = colorFormat;
            //createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            //createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            //createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            //createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;
        if (vkCreateImageView(p_base->m_device, &createInfo, nullptr, &m_colorImageView) != VK_SUCCESS)
        {
        #if __CODE_START__(DEBUG_X)
            throw ::std::runtime_error("<GraphicsPipeline.cpp> Failed to create image views!");
        #endif __CODE_END__(DEBUG_X)
        }

        p_data->f_transitionImageLayout(m_colorImage, colorFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 1);
    }
#endif __CODE_END__(MSAA)

void c_vk_pipeline::f_createDepthImage()
{
    VkFormat depthFormat = f_findDepthFormat();
    p_data->f_createImage(
        p_base->m_swapChainExtent.width, p_base->m_swapChainExtent.height, 1, 
        #if __CODE_START__(MSAA)
        p_base->m_msaaSamples,
        #else
        VK_SAMPLE_COUNT_1_BIT,
        #endif __CODE_START__(MSAA)
        depthFormat, 
        VK_IMAGE_TILING_OPTIMAL, 
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, 
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
        m_depthImage, m_depthImageMemory
    );

    VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = m_depthImage;
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = depthFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;//VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(p_base->m_device, &createInfo, nullptr, &m_depthImageView) != VK_SUCCESS)
    {
    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<GraphicsPipeline.cpp> Failed to create image views!");
    #endif __CODE_END__(DEBUG_X)
    }

    p_data->f_transitionImageLayout(m_depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);

}

void c_vk_pipeline::f_createDescriptorSetLayouts() 
{
    m_descriptorSetLayouts.resize(p_data->m_shapesCount);

    for(int i = 0; i != p_data->m_models.size(); ++i)
    {
        for(int j = 0; j != p_data->m_models[i].shapes.size(); ++j)
        {
            ::std::vector<VkDescriptorSetLayoutBinding> bindings;
            VkDescriptorSetLayoutBinding uboLayoutBinding = {};
                uboLayoutBinding.binding = 0;
                uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                uboLayoutBinding.descriptorCount = 1;
                uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
                uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
            bindings.push_back(uboLayoutBinding);

            for(int k = 0; k != p_data->m_models[i].shapes[j].textures.size(); ++k)
            {
                VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
                    samplerLayoutBinding.binding = 1;
                    samplerLayoutBinding.descriptorCount = 1;
                    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
                    samplerLayoutBinding.pImmutableSamplers = nullptr;
                bindings.push_back(samplerLayoutBinding);
            }
            VkDescriptorSetLayoutCreateInfo layoutInfo = {};
                layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
                layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
                layoutInfo.pBindings = bindings.data();
            if (vkCreateDescriptorSetLayout(p_base->m_device, &layoutInfo, nullptr, &m_descriptorSetLayouts[p_data->m_models[i].shapes[j].id]) != VK_SUCCESS)
            {
                #if __CODE_START__(DEBUG_X)
                throw ::std::runtime_error("<GraphicsPipeline.cpp> Failed to create descriptor set layout!");
                #endif __CODE_END__(DEBUG_X)
            }
        }
    }
}

void c_vk_pipeline::f_createDescriptorPools()
{
    //VkDescriptorPoolSize poolSize = {};
    //    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    //    poolSize.descriptorCount = static_cast<uint32_t>(p_data->m_uniformBuffers.size());//p_base->m_imageCount);
    //::std::array<VkDescriptorPoolSize, 2> poolSizes = {};
    //    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    //    poolSizes[0].descriptorCount = static_cast<uint32_t>(p_base->m_imageCount); //swapChainImages.size());
    //    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    //    poolSizes[1].descriptorCount = static_cast<uint32_t>(p_base->m_imageCount); //swapChainImages.size());

    //VkDescriptorPoolCreateInfo poolInfo = {};
    //    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    //    poolInfo.poolSizeCount = 1;
    //    poolInfo.pPoolSizes = &poolSize;
    //    poolInfo.maxSets = static_cast<uint32_t>(p_data->m_uniformBuffers.size());//p_base->m_imageCount);;

    //if (vkCreateDescriptorPool(p_base->m_device, &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS) 
    //{
    //#if __CODE_START__(DEBUG_X)
    //    throw ::std::runtime_error("<GraphicsPipeline.cpp> Failed to create descriptor pool!");
    //#endif __CODE_END__(DEBUG_X)
    //}

    m_descriptorPools.resize(p_data->m_shapesCount);

    for (int i = 0; i != p_data->m_models.size(); ++i)
    {
        for(int j = 0; j != p_data->m_models[i].shapes.size(); ++j)
        {
            ::std::vector<VkDescriptorPoolSize> poolSizes = {};
            VkDescriptorPoolSize poolSize;

            poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            poolSize.descriptorCount = 1;//static_cast<uint32_t>(p_base->m_imageCount); //swapChainImages.size());
            poolSizes.push_back(poolSize);

            for(int k = 0; k != p_data->m_models[i].shapes[j].textures.size(); ++k)
            {
                poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                poolSize.descriptorCount = 1;//static_cast<uint32_t>(p_base->m_imageCount); //swapChainImages.size());
                poolSizes.push_back(poolSize);
            }

            VkDescriptorPoolCreateInfo poolInfo = {};
                poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
                poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
                poolInfo.pPoolSizes = poolSizes.data();
                poolInfo.maxSets = 1;//static_cast<uint32_t>(p_base->m_imageCount);

            if (vkCreateDescriptorPool(p_base->m_device, &poolInfo, nullptr, &(m_descriptorPools[p_data->m_models[i].shapes[j].id])) != VK_SUCCESS)
            {
                #if __CODE_START__(DEBUG_X)
                throw ::std::runtime_error("<GraphicsPipeline.cpp> Failed to create descriptor pool!");
                #endif __CODE_END__(DEBUG_X)
            }
        }
    }
}

void c_vk_pipeline::f_createDescriptorSets()
{
    m_descriptorSets.resize(p_data->m_shapesCount);

    for(int i = 0; i != p_data->m_models.size(); ++i)
    {
        for(int j = 0; j != p_data->m_models[i].shapes.size(); ++j)
        {
            VkDescriptorSetLayout layouts[] = { m_descriptorSetLayouts[p_data->m_models[i].shapes[j].id] };
            VkDescriptorSetAllocateInfo allocInfo = {};
                allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
                allocInfo.descriptorPool = m_descriptorPools[p_data->m_models[i].shapes[j].id];
                allocInfo.descriptorSetCount = 1;
                allocInfo.pSetLayouts = layouts;

            if (vkAllocateDescriptorSets(p_base->m_device, &allocInfo, &m_descriptorSets[p_data->m_models[i].shapes[j].id]) != VK_SUCCESS)
            {
                #if __CODE_START__(DEBUG_X)
                throw ::std::runtime_error("<GraphicsPipeline.cpp> Failed to allocate descriptor sets!");
                #endif __CODE_END__(DEBUG_X)
            }

            ::std::vector<VkWriteDescriptorSet> descriptorWrites;
            {
                VkWriteDescriptorSet descriptorWrite;
                VkDescriptorBufferInfo bufferInfo = {};
                    bufferInfo.buffer = p_data->m_uniformBuffers[i][j];
                    bufferInfo.offset = 0;
                    bufferInfo.range = sizeof(c_vk_data::UniformBufferObject);

                descriptorWrite.pNext = nullptr;
                descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrite.dstSet = m_descriptorSets[p_data->m_models[i].shapes[j].id];
                descriptorWrite.dstBinding = 0;
                descriptorWrite.dstArrayElement = 0;
                descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptorWrite.descriptorCount = 1;
                descriptorWrite.pBufferInfo = &bufferInfo;
                descriptorWrite.pImageInfo = nullptr;

                descriptorWrites.push_back(descriptorWrite);
            }

            //::std::vector<VkDescriptorImageInfo> imageInfos(p_data->m_models[i].shapes[j].textures.size());
            for (int k = 0; k != p_data->m_models[i].shapes[j].textures.size(); ++k)
            {
                VkWriteDescriptorSet descriptorWrite;
                VkDescriptorImageInfo imageInfo;
                    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    imageInfo.imageView = p_data->m_textureImageViews[i][j][k];
                    imageInfo.sampler = p_data->m_textureSamplers[i][j][k];

                descriptorWrite.pNext = nullptr;
                descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrite.dstSet = m_descriptorSets[p_data->m_models[i].shapes[j].id];
                descriptorWrite.dstBinding = k + 1;
                descriptorWrite.dstArrayElement = 0;
                descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                descriptorWrite.descriptorCount = 1;//p_data->m_models[i].shapes[j].textures.size();
                descriptorWrite.pBufferInfo = nullptr;
                descriptorWrite.pImageInfo = &imageInfo;//imageInfos.data();

                descriptorWrites.push_back(descriptorWrite);
            }
            vkUpdateDescriptorSets(p_base->m_device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        }
    }

    /*
    for (size_t i = 0; i != p_base->m_imageCount; ++i)
    {
        VkDescriptorBufferInfo bufferInfo = {};
            bufferInfo.buffer = p_data->m_uniformBuffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(c_vk_data::UniformBufferObject);

        VkWriteDescriptorSet descriptorWrite = {};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = m_descriptorSets[i];
            descriptorWrite.dstBinding = 0;
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pBufferInfo = &bufferInfo;
            descriptorWrite.pImageInfo = nullptr; // Optional
            descriptorWrite.pTexelBufferView = nullptr; // Optional

        vkUpdateDescriptorSets(p_base->m_device, 1, &descriptorWrite, 0, nullptr);
    }
    */
}

void c_vk_pipeline::f_createPipelineLayout()
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = m_descriptorSetLayouts.size();
        pipelineLayoutInfo.pSetLayouts = m_descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
        pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

    if (vkCreatePipelineLayout(p_base->m_device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
    {
    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<GraphicsPipeline.cpp> Failed to create pipeline layout!");
    #endif __CODE_END__(DEBUG_X)
    }
}

VkShaderModule c_vk_pipeline::f_createShaderModule(const ::std::vector<char> & code)
{
    VkShaderModule shaderModule;
    VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const t_U32*>(code.data());

    if (vkCreateShaderModule(p_base->m_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
    {
    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<GraphicsPipeline.cpp> Failed to create shader module!");
    #endif __CODE_END__(DEBUG_X)
    }

    return shaderModule;
}

void c_vk_pipeline::f_createGraphicsPipeline()
{
    ::std::vector<char> vertShaderCode = readFile(p_base->m_vertPath);
    ::std::vector<char> fragShaderCode = readFile(p_base->m_fragPath);
    VkShaderModule vertShaderModule = f_createShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = f_createShaderModule(fragShaderCode);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";
    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";
    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    VkVertexInputBindingDescription bindingDescription = t_Vertex::getBindingDescription();
    auto attributeDescription = t_Vertex::getAttributeDescriptions();
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<t_U32>(attributeDescription.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescription.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)p_base->m_swapChainExtent.width;
        viewport.height = (float)p_base->m_swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

    VkRect2D scissor = {};
        scissor.offset = { 0, 0 };
        scissor.extent = p_base->m_swapChainExtent;

    VkPipelineViewportStateCreateInfo viewportState = {};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE; //VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f; // Optional
        rasterizer.depthBiasClamp = 0.0f; // Optional
        rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

    VkPipelineMultisampleStateCreateInfo multisampling = {};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        #if __CODE_START__(MSAA)
            multisampling.rasterizationSamples = p_base->m_msaaSamples;
            multisampling.sampleShadingEnable = VK_TRUE;
            multisampling.minSampleShading = 0.2f;
        #else
            multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
            multisampling.sampleShadingEnable = VK_FALSE;
            multisampling.minSampleShading = 1.0f; // Optional
        #endif __CODE_END__(MSAA)
        multisampling.pSampleMask = nullptr; // Optional
        multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
        multisampling.alphaToOneEnable = VK_FALSE; // Optional

    /* two ways of blending, alpha blending | bitwise blending */
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;             // Optional
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;             // Optional
        //colorBlendAttachment.blendEnable = VK_TRUE;
        //colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        //colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        //colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        //colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        //colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        //colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f; // Optional
        colorBlending.blendConstants[1] = 0.0f; // Optional
        colorBlending.blendConstants[2] = 0.0f; // Optional
        colorBlending.blendConstants[3] = 0.0f; // Optional

    VkPipelineDepthStencilStateCreateInfo depthStencil = {};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f; // Optional
        depthStencil.maxDepthBounds = 1.0f; // Optional
        depthStencil.stencilTestEnable = VK_FALSE;
        depthStencil.front = {}; // Optional
        depthStencil.back = {}; // Optional

    VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH };
    VkPipelineDynamicStateCreateInfo dynamicState = {};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = 2;
        dynamicState.pDynamicStates = dynamicStates;

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = nullptr; // Optional
        pipelineInfo.renderPass = m_renderPass;
        pipelineInfo.layout = m_pipelineLayout;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipelineInfo.basePipelineIndex = -1; // Optional

    if (vkCreateGraphicsPipelines(p_base->m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline) != VK_SUCCESS)
    {
    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<GraphicsPipeline.cpp> Failed to create graphics pipeline!");
    #endif __CODE_END__(DEBUG_X)
    }

    f_destroyShaderModule(vertShaderModule);
    f_destroyShaderModule(fragShaderModule);
}

VkFormat c_vk_pipeline::f_findDepthFormat() 
{
    return f_findSupportedFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

VkFormat c_vk_pipeline::f_findSupportedFormat(
    const std::vector<VkFormat> & candidates, 
    VkImageTiling tiling,
    VkFormatFeatureFlags features
) 
{
    for (VkFormat format : candidates) 
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(p_base->m_physicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) 
        {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) 
        {
            return format;
        }
    }
    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<GraphicsPipeline.cpp> Failed to find supported format!");
    #endif __CODE_END__(DEBUG_X)
}

::std::vector<char> readFile(const ::std::string & filename)
{
    ::std::ifstream file(filename, ::std::ios::ate | ::std::ios::binary);
    
    #if __CODE_START__(DEBUG_X)
        if(!file.is_open())
        {
            throw ::std::runtime_error("<GraphicsPipeline.cpp> Failed to open file.");
        }
    #endif __CODE_END__(DEBUG_X)

    size_t fileSize = (size_t) file.tellg();
    ::std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

_x_NS_END_