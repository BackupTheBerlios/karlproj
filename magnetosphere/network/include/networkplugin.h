#if !defined(MAGNET_NETWORK_PLUGIN_H_)
#define MAGNET_NETWORK_PLUGIN_H_

#include "common.h"
#include "networkcommon.h"
#include "plugin.h"

namespace magnet {
namespace network {

    class MAG_NETWORK_EXPORT INetworkPlugin : public main::IPlugin
    {
    // Class
    public:
        INetworkPlugin(const char* name, const float version);

    // Methods
    public:
        virtual bool update() = 0;
    };

} // namespace network
} // namespace magnet

#endif // !defined(MAGNET_NETWORK_PLUGIN_H_) 
