
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

#define TINYOBJLOADER_IMPLEMENTATION
#include "../tool/tiny_obj_loader.h"
#include <unordered_map>

_x_NS_START_

void c_eventMgr::f_setVertShader(const char * str, t_S32 index)
{
    if (index < 0 || index >= p_vk->m_data.m_models.size())
    {
    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<EventMgr.cpp> Failed to set vertex shader path!");
    #endif __CODE_END__(DEBUG_X)
    }

    p_vk->m_data.m_models[index].vertPath = str;
    p_vk->m_base.m_vertPath = str;
}

void c_eventMgr::f_setFragShader(const char * str, t_S32 index)
{
    if (index < 0 || index >= p_vk->m_data.m_models.size())
    {
    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<EventMgr.cpp> Failed to set fragment shader path!");
    #endif __CODE_END__(DEBUG_X)
    }

    p_vk->m_data.m_models[index].fragPath = str;
    p_vk->m_base.m_fragPath = str;
}

void c_eventMgr::f_setTextures(::std::vector<::std::vector<const char *>> & strs, t_S32 index)
{
    if (index < 0 || index >= p_vk->m_data.m_models.size())
    {
    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<EventMgr.cpp> Failed to set textures path!");
    #endif __CODE_END__(DEBUG_X)
    }

    p_vk->m_data.m_models[index].shapes.resize(strs.size());

    for(int i = 0; i != strs.size(); ++i)
    {
        p_vk->m_data.m_models[index].shapes[i].textures.resize(strs[i].size());

        for(int j = 0; j != strs[i].size(); ++j)
        {
            p_vk->m_data.m_models[index].shapes[i].textures[j] = strs[i][j];
            p_vk->m_data.m_models[index].shapes[i].id = p_vk->m_data.m_shapesCount;
            ++ (p_vk->m_data.m_shapesCount);
        }
    }
}

void c_eventMgr::f_setModel(const char * str, t_S32 index)
{
    tinyobj::attrib_t attrib;
    ::std::vector<tinyobj::shape_t> shapes;
    ::std::vector<tinyobj::material_t> materials;
    ::std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, str)) 
    {
    #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error(warn + err);
    #endif __CODE_END__(DEBUG_X)
    }

    for (int i = 0; i != shapes.size(); ++i)
    { 
        ::std::unordered_map<t_Vertex, t_U32> uniqueVertices = {};
        for (int j = 0; j != shapes[i].mesh.indices.size(); ++j)
        {
            t_Vertex vertex = {};
            vertex.pos = {
                attrib.vertices[3 * shapes[i].mesh.indices[j].vertex_index + 0],
                attrib.vertices[3 * shapes[i].mesh.indices[j].vertex_index + 1],
                attrib.vertices[3 * shapes[i].mesh.indices[j].vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * shapes[i].mesh.indices[j].texcoord_index + 0],
                1.0f - attrib.texcoords[2 * shapes[i].mesh.indices[j].texcoord_index + 1]
            };

            if (uniqueVertices.count(vertex) == 0)
            {
                uniqueVertices[vertex] = static_cast<uint32_t>(p_vk->m_data.m_models[index].shapes[i].vertice.size());
                p_vk->m_data.m_models[index].shapes[i].vertice.push_back(vertex);
            }
            p_vk->m_data.m_models[index].shapes[i].indice.push_back(uniqueVertices[vertex]);
        }
    }
}

void c_eventMgr::f_updateUniform()
{
    static auto startTime = ::std::chrono::high_resolution_clock::now();
    auto currentTime = ::std::chrono::high_resolution_clock::now();
    float time = ::std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    //c_vk_data::UniformBufferObject ubo = {};
    //    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //    ubo.proj = glm::perspective(glm::radians(45.0f), (float)(p_vk->m_base.m_swapChainExtent.width) / (float)(p_vk->m_base.m_swapChainExtent.height), 0.1f, 1000.0f);
    //    ubo.proj[1][1] *= -1;
    //memcpy((void *)&(p_vk->m_data.m_uniformObjs[0][0]), (void *)&ubo, sizeof(c_vk_data::UniformBufferObject));

    c_vk_data::UniformBufferObject ubo = {};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.model = glm::rotate(ubo.model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        
        ubo.view = glm::lookAt(glm::vec3(800.0, 800.0f, 400.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), (float)(p_vk->m_base.m_swapChainExtent.width) / (float)(p_vk->m_base.m_swapChainExtent.height), 0.1f, 1000.0f);
        ubo.proj[1][1] *= -1;
 
    memcpy((void *)&(p_vk->m_data.m_uniformObjs[0][1]), (void *)&ubo, sizeof(c_vk_data::UniformBufferObject));
    memcpy((void *)&(p_vk->m_data.m_uniformObjs[0][1]), (void *)&ubo, sizeof(c_vk_data::UniformBufferObject));
}

_x_NS_END_