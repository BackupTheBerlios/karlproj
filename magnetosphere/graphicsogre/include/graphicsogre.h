#if !defined(MAGNET_GRAPHICSOGRE_H_)
#define MAGNET_GRAPHICSOGRE_H_

#include "common.h"
#include "plugin.h"

#include "ExampleApplication.h"

namespace magnet {
namespace graphics {
namespace ogre {

    class GraphicsOgre : public main::IPlugin, public ExampleApplication {
    public:
        void initialise();
        GraphicsOgre();
        void createScene() { }
    };

} // namespace ogre
} // namespace graphics
} // namespace magnet

#endif // !defined(MAGNET_GRAPHICSOGRE_H_)