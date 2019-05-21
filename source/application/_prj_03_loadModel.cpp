
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
#include "../application/_prj_03_loadModel.hpp"
#include <vector>

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

        ::std::vector<::std::vector<const char *>> texturePaths = 
        {
            { 
                "../model/earth/earth_diffuse.jpg", 
            },
            {
                "../model/earth/earth_night_lights.jpg",
            },
            //{
            //    "../model/earth/earth_clouds.jpg",
            //},
        };
        pApp->initApp();
        pApp->setData(
            "../shader_spv/_prj_p03_loadModel2_vert.spv", 
            "../shader_spv/_prj_p03_loadModel2_frag.spv",
            "../model/earth/earth.obj",
            texturePaths
        );

        pApp->initVulkan(800, 600, "_prj_p03_loadModel2");
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