#if !defined(MAGNET_GRAPHICS_PLUGIN_H_)
#define MAGNET_GRAPHICS_PLUGIN_H_

#include "graphicscommon.h"
#include "plugin.h"
#include "commandwindow.h"

namespace magnet {
namespace graphics {

    class MAG_GRAPHICS_EXPORT IGraphicsPlugin : public main::IPlugin
    {
    // Class
    public:
        IGraphicsPlugin(const char* name, const float version);

    // Methods
    public:
        virtual ICmdWindow* createCommandWindow(const char* name) = 0;
        virtual bool update() = 0;
    };

} // namespace graphics
} // namespace magnet

#endif // !defined(MAGNET_GRAPHICS_PLUGIN_H_)