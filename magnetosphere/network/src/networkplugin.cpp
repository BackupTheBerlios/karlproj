#include "networkplugin.h"

namespace magnet {
namespace network {

    INetworkPlugin::INetworkPlugin(const char* name, const float version)
        : IPlugin(name, version)
    {
    }

} // namespace network
} // namespace magnet