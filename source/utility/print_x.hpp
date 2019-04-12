
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> print_x.hpp
 *
 *  Abstract:
 *
 *    >> Print Message
 *
 */

#if !defined(__PRINT_X_HPP__)
#define __PRINT_X_HPP__

//------------------------------------

//#define DEBUG_MODE_X
#include "../core/Macros.hpp"

_x_NS_START_

#if __CODE_START__(VALIDATION_X)
#include <iostream>
#include <string>

void PRINT(std::string str, const char * p_tip = "Message")
{
    std::cout << p_tip << ": " << str << std::endl;
}

void PRINT(t_U32 n)
{
    std::cout << n << " ";
}



#else

#define PRINT(x, y)

#endif __CODE_END__(VALIDATION_X)

_x_NS_END_

//------------------------------------

#endif