#if !defined(MAGNET_GRAPHICS_COMMON_H_)
#define MAGNET_GRAPHICS_COMMON_H_

#include "common.h"

#if defined(GRAPHICS_EXPORTS)
#   define MAG_GRAPHICS_EXPORT MAGNET_EXPORT
#else
#   define MAG_GRAPHICS_EXPORT MAGNET_IMPORT
#endif

#define GRAPHICS_INTERFACE_PLUGIN_NAME "Standard Graphics Interface"
#define GRAPHICS_INTERFACE_PLUGIN_VERSION 0.01


#endif // !defined(MAGNET_GRAPHICS_COMMON_H_)