#include "networkpcap.h"
#include "core.h"

#define PLUGIN_NAME "Standard Pcap Network Packet Manager"
#define PLUGIN_VERS (float)0.01

namespace magnet {
namespace network {
namespace pcap {

    INITIALISE_MAGNET_PLUGIN(NetworkPcap)

    NetworkPcap::NetworkPcap() : INetworkPlugin(PLUGIN_NAME, PLUGIN_VERS), mInterface(0)
    {
    }

    void NetworkPcap::initialise()
    {
        mInterface = static_cast<Network*>(main::Core::getSingleton().getNetwork());
        if (mInterface == 0)
            throw("Error initialising network packet manager: no network interface loaded");
        if (std::string(NETWORK_INTERFACE_PLUGIN_NAME) != mInterface->getName()
            || mInterface->getVersion() != NETWORK_INTERFACE_PLUGIN_VERSION)
        {
            throw("Incorrect network interface for packet manager '" PLUGIN_NAME "'");
        }
        mInterface->setPacketManager(this);
    }

    void NetworkPcap::shutdown()
    {
    }

} // namespace pcap
} // namespace network
} // namespace magnet