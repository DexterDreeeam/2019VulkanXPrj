
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
    app * pApp = new x_NS::appHelloTriangle;
    pApp->initVulkan(800, 600, "HelloTriangle");
    pApp->initApp();
    pApp->mainLoop();
    pApp->cleanup();

    return 0;
}