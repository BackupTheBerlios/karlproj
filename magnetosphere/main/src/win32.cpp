#include "platform.h"

#if (MAGNET_PLATFORM == MAG_PLATFORM_WIN32)

#define _WIN32_WINNT 0x0502
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

namespace magnet {
namespace main {

    void* Platform::loadSharedLibrary(const char *name)
    {
        return LoadLibrary(name);
    }

    void Platform::unloadSharedLibrary(void* handle)
    {
        FreeLibrary((HMODULE)handle);
    }

    void Platform::setSharedLibraryPath(const char* path)
    {
        SetDllDirectory(path);
    }

    void* Platform::getSharedLibrarySymbolAddress(void* handle, const char* name)
    {
        return GetProcAddress((HMODULE)handle, name);
    }

    bool Platform::update()
    {
        static MSG msg;
        while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return true;
    }

} // namespace main
} // namespace magnet

#endif // MAGNET_COMPILER == COMPILER_MSVC