
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> EventMgr.hpp
 *
 *  Abstract:
 *
 *    >> 
 *
 */

#if !defined(__EVENT_MGR_HPP___)
#define __EVENT_MGR_HPP___

#include "../core/VkInstance.hpp"
//#include "../core/data/Data.hpp"

_x_NS_START_

class c_eventMgr
{
public_def:

public_fun:
    c_eventMgr() { }
    ~c_eventMgr() { }
    void f_setVk(c_vk * vk) { p_vk = vk; }
    void f_setVertShader(const char * str) { p_vk->m_base.m_vertPath = str; }
    void f_setFragShader(const char * str) { p_vk->m_base.m_fragPath = str; }

    void f_updateUniform();

private_mem:
    c_vk * p_vk;
};

_x_NS_END_

#endif