
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> _entry.cpp
 *
 *  Abstract:
 *
 *    >> xPrj application class entry
 *
 */

#include "../core/Macros.hpp"
#include "../application/HelloTriangle.hpp"

using namespace x_NS;

int main()
{
#if __CODE_START__(DEBUG_X)
    try
#endif __CODE_END__(DEBUG_X)
    {
        app * pApp = new x_NS::appHelloTriangle;
        pApp->initVulkan(800, 600, "HelloTriangle");
        pApp->initApp();
        pApp->mainLoop();
        pApp->cleanup();
    }
#if __CODE_START__(DEBUG_X)
    catch(const std::runtime_error & e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
#endif __CODE_END__(DEBUG_X)

    return 0;
}