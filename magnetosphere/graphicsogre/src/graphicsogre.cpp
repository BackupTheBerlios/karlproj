#include "graphicsogre.h"
#include "core.h"

#define PLUGIN_NAME "Standard OGRE Renderer"
#define PLUGIN_VERS 0.01

namespace magnet {
namespace graphics {
namespace ogre {

    INITIALISE_MAGNET_PLUGIN(GraphicsOgre)

    GraphicsOgre::GraphicsOgre() : IPlugin(PLUGIN_NAME, (float)PLUGIN_VERS)
    {
    }

    void GraphicsOgre::initialise()
    {
        SET_TERM_HANDLER;
        go();
    }

} // namespace ogre
} // namespace graphics
} // namespace magnet
