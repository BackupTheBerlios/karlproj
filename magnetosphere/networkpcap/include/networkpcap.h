#if !defined(MAGNET_NETWORK_PCAP_H_)
#define MAGNET_NETWORK_PCAP_H_

#include "common.h"
#include "network.h"
#include "networkplugin.h"
#include "networklayer.h"

#include "pcap.h"

namespace magnet {
namespace network {
namespace pcap {

    class NetworkPcap : public INetworkPlugin, public INetworkLayer
    {
    // Class
    public:
        NetworkPcap();

    // Methods
    public:
        void initialise();
        bool applyOptions();
        bool update();
        void shutdown();

        void sendRawPacket(const char* data, unsigned short len);

    // Types
    private:
        struct PcapDevice
        {
            std::string name;
            std::string description;
            pcap_t* handle;
        };
        
    // Data
    private:
        Network* mInterface;
        std::vector<PcapDevice> mDevices;
        char mErrMsg[PCAP_ERRBUF_SIZE];
        pcap_t* mDeviceHandle;
    };

} // namespace pcap
} // namespace network
} // namespace magnet

#endif // !defined(MAGNET_NETWORK_PCAP_H_)