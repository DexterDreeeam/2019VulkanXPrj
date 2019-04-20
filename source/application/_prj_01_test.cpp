
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

#include "../core/_Macros.hpp"
#include "../application/_prj_01_test.hpp"

#if __CODE_START__(NO_CMD)
    #pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif __CODE_END__(NO_CMD)

using namespace x_NS;

int main()
{
#if __CODE_START__(DEBUG_X)
    try
#endif __CODE_END__(DEBUG_X)
    {
        app * pApp = new x_NS::_prj_01_test;
        pApp->initApp("../shader_spv/_prj_p01_test_vert.spv", "../shader_spv/_prj_p01_test_frag.spv");
        pApp->initVulkan(800, 600, "_prj_p01_test_vert");
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