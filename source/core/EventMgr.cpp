
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

void c_eventMgr::f_setPrinterTextures(const char * str, t_S32 index)
{
    if (index < 0 || index >= p_vk->m_data.m_models.size())
    {
        #if __CODE_START__(DEBUG_X)
        throw ::std::runtime_error("<EventMgr.cpp> Failed to set textures path!");
        #endif __CODE_END__(DEBUG_X)
    }

    p_vk->m_data.m_models[index].shapes.resize(1);
    p_vk->m_data.m_models[index].shapes[0].textures.resize(1);
    p_vk->m_data.m_models[index].shapes[0].textures[0] = str;

    p_vk->m_data.m_models[index].shapes[0].id = p_vk->m_data.m_shapesCount;
    ++(p_vk->m_data.m_shapesCount);
}

void c_eventMgr::f_setPrinterModel(t_S32 index)
{
    p_vk->m_data.m_printerSamplerTable['0'] = { 0.0f / 16.0f , 1.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['1'] = { 1.0f / 16.0f , 1.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['2'] = { 2.0f / 16.0f , 1.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['3'] = { 3.0f / 16.0f , 1.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['4'] = { 4.0f / 16.0f , 1.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['5'] = { 5.0f / 16.0f , 1.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['6'] = { 6.0f / 16.0f , 1.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['7'] = { 7.0f / 16.0f , 1.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['8'] = { 8.0f / 16.0f , 1.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['9'] = { 9.0f / 16.0f , 1.0f / 6.0f };

    p_vk->m_data.m_printerSamplerTable['A'] = { 1.0f / 16.0f , 2.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['B'] = { 2.0f / 16.0f , 2.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['C'] = { 3.0f / 16.0f , 2.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['D'] = { 4.0f / 16.0f , 2.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['E'] = { 5.0f / 16.0f , 2.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['F'] = { 6.0f / 16.0f , 2.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['G'] = { 7.0f / 16.0f , 2.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['H'] = { 8.0f / 16.0f , 2.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['I'] = { 9.0f / 16.0f , 2.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['J'] = { 10.0f / 16.0f, 2.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['K'] = { 11.0f / 16.0f, 2.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['L'] = { 12.0f / 16.0f, 2.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['M'] = { 13.0f / 16.0f, 2.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['N'] = { 14.0f / 16.0f, 2.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['O'] = { 15.0f / 16.0f, 2.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['P'] = { 0.0f / 16.0f , 3.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['Q'] = { 1.0f / 16.0f , 3.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['R'] = { 2.0f / 16.0f , 3.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['S'] = { 3.0f / 16.0f , 3.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['T'] = { 4.0f / 16.0f , 3.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['U'] = { 5.0f / 16.0f , 3.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['V'] = { 6.0f / 16.0f , 3.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['W'] = { 7.0f / 16.0f , 3.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['X'] = { 8.0f / 16.0f , 3.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['Y'] = { 9.0f / 16.0f , 3.0f / 6.0f };
    p_vk->m_data.m_printerSamplerTable['Z'] = { 10.0f / 16.0f, 3.0f / 6.0f };

    p_vk->m_data.m_printerModelIndex = index;

    char c = '0';
    t_Vertex vertexs[4] = {};
    vertexs[0].pos = { p_vk->m_data.m_printerInputColumn * 0.1f + (-0.5), p_vk->m_data.m_printerInputRow * 0.15f + (-0.5), 0.0f };
    vertexs[1].pos = { p_vk->m_data.m_printerInputColumn * 0.1f + (-0.5) + 0.1f, p_vk->m_data.m_printerInputRow * 0.15f + (-0.5), 0.0f };
    vertexs[2].pos = { p_vk->m_data.m_printerInputColumn * 0.1f + (-0.5) + 0.1f, p_vk->m_data.m_printerInputRow * 0.15f + (-0.5) + 0.15f, 0.0f };
    vertexs[3].pos = { p_vk->m_data.m_printerInputColumn * 0.1f + (-0.5), p_vk->m_data.m_printerInputRow * 0.15f + (-0.5) + 0.15f, 0.0f };
    vertexs[0].texCoord = { p_vk->m_data.m_printerSamplerTable[c][0], p_vk->m_data.m_printerSamplerTable[c][1] };
    vertexs[1].texCoord = { p_vk->m_data.m_printerSamplerTable[c][0] + 1.0f / 16.0f, p_vk->m_data.m_printerSamplerTable[c][1] };
    vertexs[2].texCoord = { p_vk->m_data.m_printerSamplerTable[c][0] + 1.0f / 16.0f, p_vk->m_data.m_printerSamplerTable[c][1] + 1.0f / 6.0f };
    vertexs[3].texCoord = { p_vk->m_data.m_printerSamplerTable[c][0], p_vk->m_data.m_printerSamplerTable[c][1] + 1.0f / 6.0f };

    p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].vertice.push_back(vertexs[0]);
    p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].vertice.push_back(vertexs[1]);
    p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].vertice.push_back(vertexs[2]);
    p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].vertice.push_back(vertexs[3]);

    t_U32 sz = p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].vertice.size();
    p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].indice.push_back(sz - 4);
    p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].indice.push_back(sz - 3);
    p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].indice.push_back(sz - 2);
    p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].indice.push_back(sz - 2);
    p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].indice.push_back(sz - 1);
    p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].indice.push_back(sz - 4);
}

#define KEY_PRESS(x) (glfwGetKey(p_vk->m_base.m_glfwWindow, x) == GLFW_PRESS)

void c_eventMgr::f_processPrinterInput()
{
    t_Bool shift = false;
    char c;

    if (KEY_PRESS(GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(p_vk->m_base.m_glfwWindow, true);

    if (KEY_PRESS(GLFW_KEY_LEFT_SHIFT) || KEY_PRESS(GLFW_KEY_RIGHT_SHIFT))
        shift = true;

    if (KEY_PRESS(GLFW_KEY_0))
    { c = '0'; goto label_next; }
    if (KEY_PRESS(GLFW_KEY_1))
    { c = '1'; goto label_next; }
    if (KEY_PRESS(GLFW_KEY_2))
    { c = '2'; goto label_next; }
    if (KEY_PRESS(GLFW_KEY_3))
    { c = '3'; goto label_next; }
    if (KEY_PRESS(GLFW_KEY_4))
    { c = '4'; goto label_next; }
    if (KEY_PRESS(GLFW_KEY_5))
    { c = '5'; goto label_next; }
    if (KEY_PRESS(GLFW_KEY_6))
    { c = '6'; goto label_next; }
    if (KEY_PRESS(GLFW_KEY_7))
    { c = '7'; goto label_next; }
    if (KEY_PRESS(GLFW_KEY_8))
    { c = '8'; goto label_next; }
    if (KEY_PRESS(GLFW_KEY_9))
    { c = '9'; goto label_next; }
    if (KEY_PRESS(GLFW_KEY_A))
    { c = 'A' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_B))
    { c = 'B' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_C))
    { c = 'C' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_D))
    { c = 'D' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_E))
    { c = 'E' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_F))
    { c = 'F' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_G))
    { c = 'G' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_H))
    { c = 'H' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_I))
    { c = 'I' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_J))
    { c = 'J' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_K))
    { c = 'K' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_L))
    { c = 'L' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_M))
    { c = 'M' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_N))
    { c = 'N' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_O))
    { c = 'O' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_P))
    { c = 'P' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_Q))
    { c = 'Q' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_R))
    { c = 'R' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_S))
    { c = 'S' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_T))
    { c = 'T' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_U))
    { c = 'U' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_V))
    { c = 'V' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_W))
    { c = 'W' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_X))
    { c = 'X' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_Y))
    { c = 'Y' + (shift ? 0 : 32); goto label_next; }
    if (KEY_PRESS(GLFW_KEY_Z))
    { c = 'Z' + (shift ? 0 : 32); goto label_next; }
    return;

label_next:
    t_Vertex vertexs[4] = {};
    vertexs[0].pos = { p_vk->m_data.m_printerInputColumn * 0.1f + (-0.5), p_vk->m_data.m_printerInputRow * 0.15f + (-0.5), 0.0f };
    vertexs[1].pos = { p_vk->m_data.m_printerInputColumn * 0.1f + (-0.5) + 0.1f, p_vk->m_data.m_printerInputRow * 0.15f + (-0.5), 0.0f };
    vertexs[2].pos = { p_vk->m_data.m_printerInputColumn * 0.1f + (-0.5) + 0.1f, p_vk->m_data.m_printerInputRow * 0.15f + (-0.5) + 0.15f, 0.0f };
    vertexs[3].pos = { p_vk->m_data.m_printerInputColumn * 0.1f + (-0.5), p_vk->m_data.m_printerInputRow * 0.15f + (-0.5) + 0.15f, 0.0f };
    vertexs[0].texCoord = { p_vk->m_data.m_printerSamplerTable[c][0], p_vk->m_data.m_printerSamplerTable[c][1] };
    vertexs[1].texCoord = { p_vk->m_data.m_printerSamplerTable[c][0] + 1.0f / 16.0f, p_vk->m_data.m_printerSamplerTable[c][1] };
    vertexs[2].texCoord = { p_vk->m_data.m_printerSamplerTable[c][0] + 1.0f / 16.0f, p_vk->m_data.m_printerSamplerTable[c][1] + 1.0f / 6.0f };
    vertexs[3].texCoord = { p_vk->m_data.m_printerSamplerTable[c][0], p_vk->m_data.m_printerSamplerTable[c][1] + 1.0f / 6.0f };
    
    p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].vertice.push_back(vertexs[0]);
    p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].vertice.push_back(vertexs[1]);
    p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].vertice.push_back(vertexs[2]);
    p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].vertice.push_back(vertexs[3]);

    t_U32 sz = p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].vertice.size();
    p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].indice.push_back(sz - 4);
    p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].indice.push_back(sz - 3);
    p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].indice.push_back(sz - 2);
    p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].indice.push_back(sz - 2);
    p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].indice.push_back(sz - 1);
    p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].indice.push_back(sz - 4);

    f_updatePrinterData();
}

void c_eventMgr::f_updatePrinterData()
{
    {
        t_U32 vertexCount = static_cast<t_U32>(p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].vertice.size());
        VkDeviceSize bufferSize = sizeof(t_Vertex) * vertexCount;
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        p_vk->m_data.f_createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, //VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory
        );

        void * data;
        vkMapMemory(p_vk->m_base.m_device, stagingBufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].vertice.data(), (size_t)bufferSize);
        vkUnmapMemory(p_vk->m_base.m_device, stagingBufferMemory);

        p_vk->m_data.f_copyBuffer(stagingBuffer, p_vk->m_data.m_vertexBuffers[p_vk->m_data.m_printerModelIndex][0], bufferSize);

        vkDestroyBuffer(p_vk->m_base.m_device, stagingBuffer, nullptr);
        vkFreeMemory(p_vk->m_base.m_device, stagingBufferMemory, nullptr);
    }

    {
        VkDeviceSize bufferSize = sizeof(t_U32) * p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].indice.size();
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        p_vk->m_data.f_createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory
        );

        void * data;
        vkMapMemory(p_vk->m_base.m_device, stagingBufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, p_vk->m_data.m_models[p_vk->m_data.m_printerModelIndex].shapes[0].indice.data(), (size_t)bufferSize);
        vkUnmapMemory(p_vk->m_base.m_device, stagingBufferMemory);

        p_vk->m_data.f_copyBuffer(stagingBuffer, p_vk->m_data.m_indexBuffers[p_vk->m_data.m_printerModelIndex][0], bufferSize);

        vkDestroyBuffer(p_vk->m_base.m_device, stagingBuffer, nullptr);
        vkFreeMemory(p_vk->m_base.m_device, stagingBufferMemory, nullptr);
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
    memcpy((void *)&(p_vk->m_data.m_uniformObjs[0][0]), (void *)&ubo, sizeof(c_vk_data::UniformBufferObject));
    memcpy((void *)&(p_vk->m_data.m_uniformObjs[0][1]), (void *)&ubo, sizeof(c_vk_data::UniformBufferObject));

    c_vk_data::UniformBufferObject uboPrinter = {};
        uboPrinter.model = glm::mat4(1.0f);
        ubo.view = glm::mat4(1.0f);
        ubo.proj = glm::mat4(1.0f);
    memcpy((void *)&(p_vk->m_data.m_uniformObjs[p_vk->m_data.m_printerModelIndex][0]), (void *)&uboPrinter, sizeof(c_vk_data::UniformBufferObject));
}

_x_NS_END_