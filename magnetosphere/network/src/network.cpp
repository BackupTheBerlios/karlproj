#include "network.h"
#include "core.h"

namespace magnet {
namespace network {

    INITIALISE_MAGNET_PLUGIN(Network)
    
    Network::Network() : INetworkInterface(NETWORK_INTERFACE_PLUGIN_NAME, NETWORK_INTERFACE_PLUGIN_VERSION)
    {
        setOptionType("Packet Manager", Option::OT_PLUGIN);
        setOption("Packet Manager", "networkpcap.mpi");
    }

    void Network::initialise()
    {
        main::Core::getSingleton().queuePlugin(getStringOption("Packet Manager"));
    }

    void Network::setPacketManager(INetworkPlugin *pmanager)
    {
        mPacketManager = pmanager;
    }

    bool Network::update()
    {
        if (mPacketManager != 0)
        {
            return mPacketManager->update();
        }
        return true;
    }

    void Network::shutdown()
    {
    }

} // namespace network
} // namespace magnet