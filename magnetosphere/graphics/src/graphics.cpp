#include "graphics.h"
#include "core.h"

#define PLUGIN_NAME "Standard Graphics Interface"
#define PLUGIN_VERS 0.01

namespace magnet {
namespace graphics {

    INITIALISE_MAGNET_PLUGIN(Graphics)

    Graphics::Graphics() : IPlugin(PLUGIN_NAME, (float)PLUGIN_VERS), mRenderer(0)
    {
        setOptionType("Renderer", Option::OT_PLUGIN);
        setOption("Renderer", "graphicsogre.mpi");
    }

    void Graphics::initialise()
    {
        mRenderer = main::Core::getSingleton().loadPlugin(getStringOption("Renderer"));
        if (mRenderer == 0)
            throw("Failed to load graphics renderer!");
    }

}
}
