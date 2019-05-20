
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
#include "../application/_prj_02_loadModel.hpp"

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
        app * pApp = new x_NS::_prj_02_loadModel;

        const char * texturePaths[] = { "../model/chalet/chalet.jpg" };
        pApp->initApp();
        pApp->setData(
            "../shader_spv/_prj_p02_loadModel_vert.spv", 
            "../shader_spv/_prj_p02_loadModel_frag.spv",
            "../model/chalet/chalet.obj",
            texturePaths, 1
        );
        pApp->initVulkan(800, 600, "_prj_p02_loadModel");
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