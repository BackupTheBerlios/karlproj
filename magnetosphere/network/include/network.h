#if !defined(MAGNET_NETWORK_H_)
#define MAGNET_NETWORK_H_

#include "common.h"
#include "plugininterfaces.h"

#include "networkcommon.h"
#include "networkplugin.h"

namespace magnet {
namespace network {

    class MAG_NETWORK_EXPORT Network : public main::INetworkInterface {
    // Class
    public:
        Network();

    // Methods
    public:
        void initialise();
        void shutdown();
        void setPacketManager(INetworkPlugin *pmanager);
        bool update();
    // Data
    private:
        INetworkPlugin* mPacketManager;
    };

} // namespace network
} // namespace magnet

#endif // !defined(MAGNET_NETWORK_H_)