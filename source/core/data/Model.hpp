
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> _appData.hpp
 *
 *  Abstract:
 *
 *    >> header for _appData.cpp
 *
 */

#if !defined(__MODEL_HPP__)
#define __MODEL_HPP__

#include "../../core/VkBase.hpp"
#include "../../core/data/Vertex.hpp"

#include <string>

_x_NS_START_

class c_vk_model
{
public_def:
    typedef struct t_Shape
    {
        ::std::vector<t_Vertex> vertice;
        ::std::vector<t_U32> indice;
        ::std::vector<const char *> textures;
        t_U32 id;
    };

    typedef struct t_Model
    {
        ::std::vector<t_Shape> shapes;
        const char * vertPath;
        const char * fragPath;
    };

public_fun:

private_mem:

private_fun:

};

_x_NS_END_

#endif