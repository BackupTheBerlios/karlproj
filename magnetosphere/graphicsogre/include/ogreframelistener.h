#if !defined(MAGNET_OGREFRAMELISTENER_H_)
#define MAGNET_OGREFRAMELISTENER_H_

#include "CEGUI.h"
#include "ExampleApplication.h"
#include "renderers/OgreGUIRenderer/ogrerenderer.h"

namespace magnet {
namespace graphics {
namespace ogre {

    class CEGUIOgre_FrameListener : public ExampleFrameListener, Ogre::MouseMotionListener, Ogre::MouseListener
    {
    public:
	    CEGUIOgre_FrameListener(RenderWindow* win, Camera* cam) : ExampleFrameListener(win, cam)
	    {
		    mEventProcessor = new EventProcessor();
		    mEventProcessor->initialise(win);
		    mEventProcessor->startProcessingEvents();
		    mEventProcessor->addKeyListener(this);
		    mEventProcessor->addMouseMotionListener(this);
		    mEventProcessor->addMouseListener(this);
		    mInputDevice = mEventProcessor->getInputReader();

            mTimer = PlatformManager::getSingleton().createTimer();
            mTimer->reset();

		    mQuit = false;
		    mSkipCount = 0;
		    mUpdateFreq = 0;
            mLastKey = 0;
	    }


	    virtual void 	mouseMoved (MouseEvent *e)
	    {
		    CEGUI::Renderer* rend = CEGUI::System::getSingleton().getRenderer();
		    CEGUI::System::getSingleton().injectMouseMove(e->getRelX() * rend->getWidth(), e->getRelY() * rend->getHeight());
		    e->consume();
	    }


	    virtual void 	mouseDragged (MouseEvent *e)
	    {
		    mouseMoved(e);
	    }


	    virtual void 	keyPressed (KeyEvent *e)
	    {
		    // give 'quitting' priority
		    if (e->getKey() == KC_ESCAPE)
		    {
			    mQuit = true;
			    e->consume();
			    return;
		    }

		    // do event injection
		    CEGUI::System& cegui = CEGUI::System::getSingleton();

		    // key down
		    cegui.injectKeyDown(e->getKey());

		    // now character
		    cegui.injectChar(e->getKeyChar());

            // remember last key
            mRepeating = false;
            mLastKeyTime = mTimer->getMilliseconds();
            mLastKey = e->getKey();
            mLastKeyChar = e->getKeyChar();

		    e->consume();
	    }


	    virtual void 	keyReleased (KeyEvent *e)
	    {
            mRepeating = false;
            mLastKey = 0;
		    CEGUI::System::getSingleton().injectKeyUp(e->getKey());
	    }



	    virtual void 	mousePressed (MouseEvent *e)
	    {
		    CEGUI::System::getSingleton().injectMouseButtonDown(convertOgreButtonToCegui(e->getButtonID()));
		    e->consume();
	    }


	    virtual void 	mouseReleased (MouseEvent *e)
	    {
		    CEGUI::System::getSingleton().injectMouseButtonUp(convertOgreButtonToCegui(e->getButtonID()));
		    e->consume();
	    }

	    // do-nothing events
	    virtual void 	keyClicked (KeyEvent *e) {}
	    virtual void 	mouseClicked (MouseEvent *e) {}
	    virtual void 	mouseEntered (MouseEvent *e) {}
	    virtual void 	mouseExited (MouseEvent *e) {}


	    bool frameStarted(const FrameEvent& evt);

	    /*************************************************************************
		    Some CEGUI Event handlers
	    *************************************************************************/
	    bool	horzMoveHandler(const CEGUI::EventArgs& e);
	    bool	vertMoveHandler(const CEGUI::EventArgs& e);
	    bool	vscrollHandler(const CEGUI::EventArgs& e);


    protected:
	    CEGUI::MouseButton convertOgreButtonToCegui(int ogre_button_id)
	    {
		    switch (ogre_button_id)
		    {
		    case MouseEvent::BUTTON0_MASK:
			    return CEGUI::LeftButton;
			    break;

		    case MouseEvent::BUTTON1_MASK:
			    return CEGUI::RightButton;
			    break;

		    case MouseEvent::BUTTON2_MASK:
			    return CEGUI::MiddleButton;
			    break;

		    case MouseEvent::BUTTON3_MASK:
			    return CEGUI::X1Button;
			    break;

		    default:
			    return CEGUI::LeftButton;
			    break;
		    }

	    }



	    void updateStats(void)
	    {
		    static CEGUI::String currFps = (CEGUI::utf8*)"Current FPS: ";
		    static CEGUI::String avgFps = (CEGUI::utf8*)"Average FPS: ";
		    static CEGUI::String bestFps = (CEGUI::utf8*)"Best FPS: ";
		    static CEGUI::String worstFps = (CEGUI::utf8*)"Worst FPS: ";
		    static CEGUI::String tris = (CEGUI::utf8*)"Triangle Count: ";

		    if (mSkipCount >= mUpdateFreq)
		    {
			    mSkipCount = 0;

			    try 
			    {
				    CEGUI::Window* guiAvg = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"OPAverageFPS");
				    CEGUI::Window* guiCurr = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"OPCurrentFPS");
				    CEGUI::Window* guiBest = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"OPBestFPS");
				    CEGUI::Window* guiWorst = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"OPWorstFPS");

				    const RenderTarget::FrameStats& stats = mWindow->getStatistics();

				    guiAvg->setText(avgFps + StringConverter::toString(stats.avgFPS));
				    guiCurr->setText(currFps + StringConverter::toString(stats.lastFPS));
				    guiBest->setText(bestFps + StringConverter::toString(stats.bestFPS)
					    +" "+StringConverter::toString(stats.bestFrameTime)+" ms");
				    guiWorst->setText(worstFps + StringConverter::toString(stats.worstFPS)
					    +" "+StringConverter::toString(stats.worstFrameTime)+" ms");

				    CEGUI::Window* guiTris = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"OPTriCount");
				    guiTris->setText(tris + StringConverter::toString(stats.triangleCount));

				    CEGUI::Window* guiDbg = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"OPDebugMsg");
				    guiDbg->setText(mWindow->getDebugText());
			    }
			    catch (CEGUI::UnknownObjectException x)
			    {
				    // just skip if windows are missing
			    }

		    }
		    else
		    {
			    mSkipCount++;
		    }

	    }


    protected:
	    float	mSkipCount;
	    float	mUpdateFreq;

        bool mRepeating;
        Ogre::Timer* mTimer;
        char mLastKeyChar;
        CEGUI::uint mLastKey;
        unsigned long mLastKeyTime;

	    bool mQuit;
    };

} // namespace ogre
} // namespace graphics
} // namespace magnet

#endif // !defined(MAGNET_OGREFRAMELISTENER_H_)