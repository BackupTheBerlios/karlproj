#include "graphicsplugin.h"

namespace magnet {
namespace graphics {

    IGraphicsPlugin::IGraphicsPlugin(const char* name, const float version)
        : main::IPlugin(name, version)
    {
    }

} // namespace graphics
} // namespace magnet