#if !defined(MAGNET_OGRECMDWINDOW_H_)
#define MAGNET_OGRECMDWINDOW_H_

#include "commandwindow.h"

#include "CEGUI.h"

namespace magnet {
namespace graphics {
namespace ogre {

    class OgreCmdWindow : public ICmdWindow
    {
    // Class
    public:
        OgreCmdWindow(const char* name);
        ~OgreCmdWindow();

    // Methods
    public:
        void writeString(const char* str);
    private:
        static bool entryKeyDownHandler(const CEGUI::EventArgs& e);

    // Data
    private:
        CEGUI::FrameWindow* mainWnd;
        CEGUI::StaticText* textWnd;
        CEGUI::Editbox* entryWnd;
        std::string textBuffer;
    };

} // namespace ogre
} // namespace graphics
} // namespace magnet

#endif // !defined(MAGNET_OGRECMDWINDOW_H_)