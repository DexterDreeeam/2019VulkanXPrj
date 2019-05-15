
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> CoreGlobal.hpp
 *
 *  Abstract:
 *
 *    >> core global variable declaration
 *
 */

#if !defined(___CORE_BLOBAL_HPP___)
#define ___CORE_BLOBAL_HPP___

//#include "../core/VkInstance.hpp"
#include "../core/_Macros.hpp"
#include <vector>

//------------------------------------

_x_NS_START_

extern const ::std::vector<const char * > g_deviceExtensions;
extern const ::std::vector<const char *> g_validationLayers;

class c_vk;
extern c_vk * g_vk;

_x_NS_END_

//------------------------------------

#endif