#if !defined(MAGNET_NETWORK_PCAP_H_)
#define MAGNET_NETWORK_PCAP_H_

#include "common.h"
#include "network.h"
#include "networkplugin.h"

namespace magnet {
namespace network {
namespace pcap {

    class NetworkPcap : public INetworkPlugin {
    // Class
    public:
        NetworkPcap();

    // Methods
    public:
        void initialise();
        void shutdown();

    // Data
    private:
        Network* mInterface;
    };

} // namespace pcap
} // namespace network
} // namespace magnet

#endif // !defined(MAGNET_NETWORK_PCAP_H_