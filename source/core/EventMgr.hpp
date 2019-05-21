
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

#include <vector>

_x_NS_START_

class c_eventMgr
{
public_def:

public_fun:
    c_eventMgr() { }
    ~c_eventMgr() { }
    void f_setVk(c_vk * vk) { p_vk = vk; }

    t_S32 f_createNewModel()
    {
        c_vk_model::t_Model model;
        p_vk->m_data.m_models.push_back(model);
        return p_vk->m_data.m_models.size() - 1;
    }
    void f_setVertShader(const char * str, t_S32 index);
    void f_setFragShader(const char * str, t_S32 index);
    void f_setTextures(::std::vector<::std::vector<const char *>> & strs, t_S32 index);
    void f_setModel(const char * str, t_S32 index);

    void f_updateUniform();

private_mem:
    c_vk * p_vk;
};

_x_NS_END_

#endif