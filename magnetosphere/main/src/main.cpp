#include "core.h"

#include <strstream>

// Application entry-point
#if defined (_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
    #include "windows.h"
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    #include <iostream>
    int main(int argc, char* argv[])
#endif
{
    try {
        magnet::main::Core myCore;
        myCore.go();
        myCore.shutdown();
    } catch(const char * e) {
    #if defined (_WINDOWS)
        MessageBox(NULL, e, "Magnetosphere Fatal Exception", MB_OK | MB_ICONERROR | MB_TASKMODAL);
    #else
        std::cerr << "An exception has occurred:\n  " << e << std::endl;
    #endif
    }
    return 0;
}
