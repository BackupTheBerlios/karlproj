#include "commandwindow.h"
#include "core.h"
#include "graphics.h"

namespace magnet {
namespace graphics {

    ICmdWindow::ICmdWindow(const char* name) : mName(name)
    {
    }
    ICmdWindow::~ICmdWindow()
    {
    }

    void ICmdWindow::readString(const char* string)
    {
        ((Graphics*)magnet::main::Core::getSingleton().getGraphics())
            ->readString(string, mName.c_str());
    }

}
}