#include "ogrecmdwindow.h"

#include "core.h"

#include <sstream>

namespace magnet {
namespace graphics {
namespace ogre {

    OgreCmdWindow::OgreCmdWindow(const char* name) : ICmdWindow(name)
    {
		//
		// Build a window
		//
        mainWnd = (CEGUI::FrameWindow*)CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/FrameWindow", name);
        CEGUI::System::getSingleton().getGUISheet()->addChildWindow(mainWnd);
        mainWnd->setPosition(CEGUI::Point(0.3f, 0.5f));
        mainWnd->setSize(CEGUI::Size(0.7f, 0.5f));
		mainWnd->setCloseButtonEnabled(false);
		mainWnd->setText(name);
        mainWnd->setMinimumSize(CEGUI::Size(0.1f, 0.1f));
        if (!CEGUI::FontManager::getSingleton().isFontPresent("Courier"))
        {
            CEGUI::Font* Courier = CEGUI::FontManager::getSingleton().createFont("Courier", "cour.ttf", 10, 0);
        }

        textWnd = (CEGUI::StaticText*)CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", std::string(name) + "/Static");
		mainWnd->addChildWindow(textWnd);
        textWnd->setPosition(CEGUI::Point(0.05f, 0.1f));
        textWnd->setSize(CEGUI::Size(0.9f, 0.74f));
        textWnd->setHorizontalFormatting(CEGUI::StaticText::HorzFormatting::WordWrapLeftAligned);
        textWnd->setVerticalFormatting(CEGUI::StaticText::VertFormatting::BottomAligned);
        textWnd->setVerticalScrollbarEnabled(true);
        textWnd->setFont("Courier");

		// Edit box for text entry
        entryWnd = (CEGUI::Editbox*)CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Editbox", std::string(name) + "/Editbox");
		mainWnd->addChildWindow(entryWnd);
        entryWnd->setPosition(CEGUI::Point(0.05f, 0.86f));
        entryWnd->setSize(CEGUI::Size(0.9f, 0.0f));
        entryWnd->setMaximumSize(CEGUI::Size(1.0f, 0.04f));
        entryWnd->setMinimumSize(CEGUI::Size(0.1f, 0.04f));
        entryWnd->setUserData(this);
        entryWnd->subscribeEvent(CEGUI::Window::EventKeyDown, &entryKeyDownHandler);
        entryWnd->setFont("Courier");

		//
		// Controls are set up.  Install initial settings
		//
        entryWnd->activate();
    }

    bool OgreCmdWindow::entryKeyDownHandler(const CEGUI::EventArgs& e)
    {
        std::stringstream ss;
        if(((const CEGUI::KeyEventArgs&)e).scancode == 28)
        {
            CEGUI::Window * wnd = ((const CEGUI::KeyEventArgs&)e).window;
            ((OgreCmdWindow*)wnd->getUserData())->readString(wnd->getText().c_str());
            wnd->setText("");
        }
        return true;
    }


    OgreCmdWindow::~OgreCmdWindow()
    {
    }

    void OgreCmdWindow::writeString(const char* str)
    {
        textBuffer += str;
        textWnd->setText(textBuffer.c_str());
    }

} // namespace ogre
} // namespace graphics
} // namespace magnet