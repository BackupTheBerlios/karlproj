#include "networkpcap.h"
#include "core.h"

#include "pcap.h"

#include <sstream>

#define PLUGIN_NAME "Standard Pcap Network Packet Manager"
#define PLUGIN_VERS (float)0.01

namespace magnet {
namespace network {
namespace pcap {

    INITIALISE_MAGNET_PLUGIN(NetworkPcap)

    NetworkPcap::NetworkPcap() : INetworkPlugin(PLUGIN_NAME, PLUGIN_VERS), mInterface(0), mDevices(0)
    {
        setOptionType("Device", Option::OT_MULTICHOICE);
    }

    void NetworkPcap::initialise()
    {
        // Check to make sure it's safe to load up
        mInterface = static_cast<Network*>(main::Core::getSingleton().getNetwork());
        if (mInterface == 0)
            MAG_EXCEPT_DESCR(main::EXCEPTION_CONFIG, "Error initialising network packet manager: no network interface loaded");
        if (std::string(NETWORK_INTERFACE_PLUGIN_NAME) != mInterface->getName()
            || mInterface->getVersion() != NETWORK_INTERFACE_PLUGIN_VERSION)
        {
            MAG_EXCEPT_DESCR(main::EXCEPTION_CONFIG, "Incorrect network interface for packet manager '" PLUGIN_NAME "'");
        }

        // Acquire a list of working network devices
        pcap_if* devList,* curDev;
        PcapDevice tempDevice;
        if (pcap_findalldevs(&devList, mErrMsg) == -1)
        {
            main::Core::getSingleton().writeString(mErrMsg);
            MAG_EXCEPT_DESCR(main::EXCEPTION_INTERNAL, "Failed to retrieve pcap device list!");
        }
        for (curDev = devList; curDev != 0; curDev = curDev->next)
        {
            if (curDev->addresses != 0)
            {
                tempDevice.name = curDev->name;
                tempDevice.description = curDev->description;
                tempDevice.handle = pcap_open_live(curDev->name, 65536, true, -1, mErrMsg);
                mDevices.push_back(tempDevice);
                std::string opnam = tempDevice.description + ":" + tempDevice.name;
                addOptionChoice("Device", opnam.c_str());
                //main::Core::getSingleton().writeString((std::string("Found device: ") + curDev->name + ":" + curDev->description).c_str());
            }
        }
        pcap_freealldevs(devList);

        // If we have no device setting, fill it
        if (getStringOption("Device")[0] == 0)
        {
            std::string opnam = mDevices[0].description + ":" + mDevices[0].name;
            setOption("Device", opnam.c_str());
        }

        applyOptions();

        // Finalize loading
        main::Core::getSingleton().writeString(pcap_lib_version());
        mInterface->setPacketManager(this);
    }

    bool NetworkPcap::applyOptions()
    {
        // Set current device handle

        mDeviceHandle = 0;

        for (unsigned int i = 0; i < mDevices.size(); i ++)
        {
            if (mDevices[i].description + ":" + mDevices[i].name == getStringOption("Device"))
            {
                mDeviceHandle = mDevices[i].handle;
                break;
            }
        }

        if (mDeviceHandle == 0)
        {
            std::string out = (std::string)"WARNING! Device not found: " + getStringOption("Device");
            main::Core::getSingleton().writeString(out.c_str());
        }

        return true;
    }

    bool NetworkPcap::update()
    {
        int res;
        pcap_pkthdr* header;
        const u_char* data;
        for (unsigned int i = 0; i < mDevices.size(); i ++)
        {
            if (mDevices[i].handle != 0)
            {
                res = pcap_next_ex(mDevices[i].handle, &header, &data);
                if (res == -1)
                {
                    main::Core::getSingleton().writeString(pcap_geterr(mDevices[i].handle));
                }
                else if (res > 0)
                {
                    std::stringstream ss;
                    ss << "Packet: ";
                    ss << header->len;
                    //main::Core::getSingleton().writeString(ss.str().c_str());
                }
            }
        }
        return true;
    }

    void NetworkPcap::sendRawPacket(const char* data, unsigned short len)
    {
        // TODO: remove this 3vil const_cast
        if (pcap_sendpacket(mDeviceHandle, const_cast<u_char*>((const u_char*)data), len))
        {
            main::Core::getSingleton().writeString(pcap_geterr(mDeviceHandle));
        }
    }

    void NetworkPcap::shutdown()
    {
    }

} // namespace pcap
} // namespace network
} // namespace magnet