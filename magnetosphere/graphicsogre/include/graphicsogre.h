#if !defined(MAGNET_GRAPHICSOGRE_H_)
#define MAGNET_GRAPHICSOGRE_H_

#include "common.h"
#include "graphics.h"
#include "graphicsplugin.h"
#include "ogreframelistener.h"

#include "CEGUI.h"
#include "ExampleApplication.h"
#include "OgreCEGUIRenderer.h"

namespace magnet {
namespace graphics {
namespace ogre {

    class GraphicsOgre : public IGraphicsPlugin, public ExampleApplication {
    // Class
    public:
        GraphicsOgre();

    // Methods
    public:
        void initialise();
        bool update();
        void shutdown();

        ICmdWindow* createCommandWindow(const char *name);
    private:
	    void createScene(void);
	    void createFrameListener(void);
        void setupResources();
        bool configure();

    // Data
    private:
        CEGUI::OgreCEGUIRenderer* mGUIRenderer;
        Graphics* mInterface;
    };

} // namespace ogre
} // namespace graphics
} // namespace magnet

#endif // !defined(MAGNET_GRAPHICSOGRE_H_)