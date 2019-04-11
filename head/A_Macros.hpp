
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> A_Macros.hpp
 *
 *  Abstract:
 *
 *    >> core macros
 *
 */

//#define DEBUG_MODE_X
//#define VALIDATION_MODE_X

/*
#if __CODE_START__(DEBUG_X)
  ...
#endif __CODE_END__(DEBUG_X)

#if __CODE_START__(VALIDATION_X)
  ...
#endif __CODE_END__(VALIDATION_X)
*/

#ifndef ___A_MACROS_HPP___
#define ___A_MACROS_HPP___

//------------------------------------

#define private_def private
#define private_mem private
#define private_fun private
#define public_def public
#define public_mem public
#define public_fun public

//------------------------------------

#ifdef DEBUG_MODE_X
  #define RUNTIME_DEBUG_MODE_X
  #define RUNTIME_MODE_X 2
#endif

#ifdef VALIDATION_MODE_X
  #define RUNTIME_VALIDATION_MODE_X
  #define RUNTIME_MODE_X 1
#endif

#ifndef RUNTIME_MODE_X
  #define RUNTIME_NORMAL_MODE_X
  #define RUNTIME_MODE_X 0
#endif

#define __CODE_START_OF_DEBUG_X__ (RUNTIME_MODE_X == 2)
#define __CODE_END_OF_DEBUG_X__
#define __CODE_START_OF_VALIDATION_X__ (RUNTIME_MODE_X > 0)
#define __CODE_END_OF_VALIDATION_X__

#define __CODE_START__(x) __CODE_START_OF_##x##__
#define __CODE_END__(x) __CODE_END_OF_##x##__

//------------------------------------

typedef bool tBool;
typedef signed char tS8;
typedef unsigned char tU8;
typedef signed short tS16;
typedef unsigned short tU16;
typedef signed int tS32;
typedef unsigned int tU32;
typedef signed long int tS64;
typedef unsigned long int tU64;
typedef signed long long int tS128;
typedef unsigned long long int tU128;
typedef float tF64;
typedef float tFloat;
typedef double tF128;
typedef double tDouble;

//------------------------------------

#endif