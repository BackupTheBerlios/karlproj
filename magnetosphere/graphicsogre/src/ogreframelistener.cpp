#include "ogreframelistener.h"

namespace magnet {
namespace graphics {
namespace ogre {

    bool CEGUIOgre_FrameListener::frameStarted(const FrameEvent& evt)
    {
        if (mLastKey != 0)
        {
            if (!mRepeating && mTimer->getMilliseconds() - mLastKeyTime > 200)
                mRepeating = true;
            if (mRepeating && mTimer->getMilliseconds() > 10)
            {
                mTimer->reset();
                CEGUI::System::getSingleton().injectChar(mLastKeyChar);
                CEGUI::System::getSingleton().injectKeyDown(mLastKey);
            }
        }
        return !mQuit;
    }

} // namespace ogre
} // namespace graphics
} // namespace magnet