#include "graphics.h"
#include "core.h"

namespace magnet {
namespace graphics {

    INITIALISE_MAGNET_PLUGIN(Graphics)

    Graphics::Graphics() : IGraphicsInterface(GRAPHICS_INTERFACE_PLUGIN_NAME, (float)GRAPHICS_INTERFACE_PLUGIN_VERSION), mRenderer(0), mCmdWindow(0)
    {
        setOptionType("Renderer", Option::OT_PLUGIN);
        setOption("Renderer", "graphicsogre.mpi");
    }

    void Graphics::initialise()
    {
        //main::Core::getSingleton().loadPlugin(getStringOption("Renderer"));
        main::Core::getSingleton().queuePlugin(getStringOption("Renderer"));

        //if (mRenderer == 0)
        //    throw("Failed to load graphics renderer!");
    }

    void Graphics::setRenderer(IGraphicsPlugin *renderer)
    {
        mRenderer = renderer;
        mCmdWindow = mRenderer->createCommandWindow("Magnetosphere");
        if (mCmdWindow == 0)
            throw("Failed to create command window!");
    }

    bool Graphics::update()
    {
        if (mRenderer != 0)
            return mRenderer->update();
        return true;
    }

    void Graphics::shutdown()
    {
        if (mRenderer != 0)
        {
            mRenderer = 0;
        }
        if (mCmdWindow != 0)
        {
            delete mCmdWindow;
            mCmdWindow = 0;
        }
    }

    void Graphics::writeString(const char* string)
    {
        static std::string graphicsWritingStringBuf;
        if (mCmdWindow != 0)
        {
            if (!graphicsWritingStringBuf.empty())
            {
                mCmdWindow->writeString(graphicsWritingStringBuf.c_str());
                graphicsWritingStringBuf.clear();
            }
            mCmdWindow->writeString(string);
            mCmdWindow->writeString("\n");
        } else {
            graphicsWritingStringBuf += std::string(string) + "\n";
        }
    }

    void Graphics::readString(const char* string, const char* source)
    {
        main::Core::getSingleton().runString(string);
    }

}
}
