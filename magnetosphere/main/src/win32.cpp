#include "platform.h"

#if (MAGNET_COMPILER == MAG_COMPILER_MSVC)

#define _WIN32_WINNT 0x0502
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

namespace magnet {
namespace main {

    void* Platform::LoadSharedLibrary(const char *name)
    {
        return LoadLibrary(name);
    }

    void Platform::UnloadSharedLibrary(void* handle)
    {
        FreeLibrary((HMODULE)handle);
    }

    void Platform::SetSharedLibraryPath(const char* path)
    {
        SetDllDirectory(path);
    }

    void* Platform::GetSharedLibrarySymbolAddress(void* handle, const char* name)
    {
        return GetProcAddress((HMODULE)handle, name);
    }

} // namespace main
} // namespace magnet

#endif // MAGNET_COMPILER == COMPILER_MSVC