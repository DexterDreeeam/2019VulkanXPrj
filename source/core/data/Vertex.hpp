
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> Vertex.hpp
 *
 *  Abstract:
 *
 *    >> header for Vertex.cpp
 *
 */

#if !defined(__VERTEX_HPP__)
#define __VERTEX_HPP__

#define GLFW_INCLUDE_VULKAN
#include "../../../_lib_external/glfw/inc/glfw3.h"
 //#include "../../../_lib_external/LunargVulkan_1_1_92_1/inc/vulkan/vulkan.h"

#include "../../core/_Macros.hpp"
#include "../../math/math_x.hpp"
#include <vector>
#include <array>

_x_NS_START_

struct t_Vertex
{
    t_Vec3 pos;
    t_Vec3 color;
    t_Vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(t_Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static ::std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
    {
        ::std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(t_Vertex, pos);

        //attributeDescriptions[1].binding = 0;
        //attributeDescriptions[1].location = 1;
        //attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        //attributeDescriptions[1].offset = offsetof(t_Vertex, color);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(t_Vertex, texCoord);

        return attributeDescriptions;
    }

    bool operator==(const t_Vertex & other) const 
    {
        return pos == other.pos /*&& color == other.color*/ && texCoord == other.texCoord;
    }
};

_x_NS_END_

namespace std
{
    template<> struct hash<x_NS::t_Vertex>
    {
        size_t operator()(x_NS::t_Vertex const & vertex) const
        {
            return ((hash<glm::vec3>()(vertex.pos)
                    /*^(hash<glm::vec3>()(vertex.color) << 1)*/) >> 1) ^
                    (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

#endif