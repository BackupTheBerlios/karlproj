#if !defined(MAGNET_NETWORK_LAYER_H_)
#define MAGNET_NETWORK_LAYER_H_

#include "common.h"
#include "networkcommon.h"

namespace magnet {
namespace network {

    class INetworkLayer
    {
    // Class
    public:

    // Methods
    public:


        //UDP/Datagram

        //TCP/Stream

        //RAW
        virtual void sendRawPacket(const char* data, unsigned short len) = 0;

        //Attach
    };

} // namespace network
} // namespace magnet

#endif // !defined(MAGNET_NETWORK_LAYER_H_) 
