#if !defined(MAGNET_PLATFORM_H_) 
#define MAGNET_PLATFORM_H_

#include "common.h"

namespace magnet {
namespace main {

    class MAG_MAIN_EXPORT Platform {
    public:
        static void* loadSharedLibrary(const char* name);
        static void unloadSharedLibrary(void* handle);
        static void setSharedLibraryPath(const char* path);
        static void* getSharedLibrarySymbolAddress(void* handle, const char* name);
        static bool update();
    };

} // namespace main
} // namespace magnet

#endif // !defined(MAGNET_PLATFORM_H_)