#if !defined(MAGNET_PLATFORM_H_) 
#define MAGNET_PLATFORM_H_

#include "common.h"

namespace magnet {
namespace main {

    class Platform {
    public:
        static void* LoadSharedLibrary(const char* name);
        static void UnloadSharedLibrary(void* handle);
        static void SetSharedLibraryPath(const char* path);
        static void* GetSharedLibrarySymbolAddress(void* handle, const char* name);
    };

} // namespace main
} // namespace magnet

#endif // !defined(MAGNET_PLATFORM_H_)