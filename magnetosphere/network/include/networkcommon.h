#if !defined(MAGNET_NETWORK_COMMON_H_)
#define MAGNET_NETWORK_COMMON_H_

#include "common.h"

#if defined(NETWORK_EXPORTS)
#   define MAG_NETWORK_EXPORT MAGNET_EXPORT
#else
#   define MAG_NETWORK_EXPORT MAGNET_IMPORT
#endif

#define NETWORK_INTERFACE_PLUGIN_NAME "Standard Network Interface"
#define NETWORK_INTERFACE_PLUGIN_VERSION float(0.01)

#endif // !defined(MAGNET_NETWORK_COMMON_H_) 
