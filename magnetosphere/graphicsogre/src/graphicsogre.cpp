#include "graphicsogre.h"
#include "ogrecmdwindow.h"
#include "core.h"
#include "platform.h"

#define PLUGIN_NAME "Standard OGRE Renderer"
#define PLUGIN_VERS 0.01

namespace magnet {
namespace graphics {
namespace ogre {

    INITIALISE_MAGNET_PLUGIN(GraphicsOgre)

    GraphicsOgre::GraphicsOgre() : IGraphicsPlugin(PLUGIN_NAME, (float)PLUGIN_VERS), mGUIRenderer(0)
    {
        setOptionType("Archives", Option::OT_STRING);
        setOption("Archives", "Zip:plugins/ogre/datafiles/OgreCore.zip,FileSystem:plugins/ogre/datafiles,FileSystem:plugins/ogre/datafiles/fonts,FileSystem:plugins/ogre/datafiles/imagesets,FileSystem:plugins/ogre/datafiles/schemes");
        setOptionType("Render System", Option::OT_CHOICE);
        setOptionType("Plugins File", Option::OT_STRING);
        setOption("Plugins File", "plugins/ogre/plugins.cfg");
        setOptionType("Config File", Option::OT_STRING);
        setOption("Config File", "plugins/ogre/video.cfg");
        setOptionType("Log File", Option::OT_STRING);
        setOption("Log File", "plugins/ogre/ogre.log");
        setOptionType("CEGUI Log File", Option::OT_STRING);
        setOption("CEGUI Log File", "plugins/ogre/cegui.log");
    }

    void GraphicsOgre::initialise()
    {
        mInterface = static_cast<Graphics*>(main::Core::getSingleton().getGraphics());
        if (mInterface == 0)
            throw("Failed to initialize graphics renderer: no graphics interface loaded");
        if (std::string(GRAPHICS_INTERFACE_PLUGIN_NAME) != mInterface->getName()
            || mInterface->getVersion() != (float)GRAPHICS_INTERFACE_PLUGIN_VERSION)
        {
            throw("Incorrect graphics interface for graphics renderer '" PLUGIN_NAME "'");
        }

        try
        {
            main::Core::getSingleton().writeString("*-*-* OGRE Initialising");
            main::Core::getSingleton().writeString(("*-*-* Version " +
                StringConverter::toString(OGRE_VERSION_MAJOR) + "." + 
                StringConverter::toString(OGRE_VERSION_MINOR) + "." + 
                StringConverter::toString(OGRE_VERSION_PATCH) + " " + 
                "(" + OGRE_VERSION_NAME + ")").c_str());

            mRoot = new Root(getStringOption("Plugins File"), getStringOption("Config File"), getStringOption("Log File"));

            setupResources();

            if (!configure()) return;

            chooseSceneManager();
            createCamera();
            createViewports();

            // Set default mipmap level (NB some APIs ignore this)
            TextureManager::getSingleton().setDefaultNumMipMaps(5);

            // Create the scene
            createScene();

            createFrameListener();

            // TODO: make our own term handler
            SET_TERM_HANDLER;

            mRoot->getRenderSystem()->_initRenderTargets();

        } catch(Ogre::Exception& e) {
            char a[1024];
            strcpy(a, e.getFullDescription().c_str());
            throw((char*)a);
        } catch(CEGUI::Exception &e) {
            char a[1024];
            strcpy(a, e.getMessage().c_str());
            throw((char*)a);
        }
        mInterface->setRenderer(this);
    }

    bool GraphicsOgre::update()
    {
        return mRoot->renderOneFrame();
    }

    void GraphicsOgre::setupResources()
    {
        std::string archives = getStringOption("Archives");
        std::string type, path;
        size_t off1 = 0, off2 = 0, off3 = 0;
        off2 = archives.find_first_of("=:", off1);
        off3 = archives.find_first_of(";,", off2);
        while (off3 != -1)
        {
            type = std::string(archives, off1, off2 - off1);
            path = std::string(archives, off2 + 1, off3 - off2 - 1);
            off1 = off3 + 1;
            ResourceManager::addCommonArchiveEx(path, type);
            off2 = archives.find_first_of("=:", off1);
            off3 = archives.find_first_of(";,", off2);
        }
        type = std::string(archives, off1, off2 - off1);
        path = std::string(archives, off2 + 1);
        ResourceManager::addCommonArchiveEx(path, type);
    }

    bool GraphicsOgre::configure()
    {
        const RenderSystemList* rendLst = Root::getSingleton().getAvailableRenderers();
        ConfigOptionMap* opts;

        if (getStringOption("Render System")[0] == 0)
            setOption("Render System", (*rendLst->begin())->getName().c_str());
        

        for (RenderSystemList::const_iterator iter = rendLst->begin(); iter != rendLst->end(); iter ++)
        {
            if ((*iter)->getName() == getStringOption("Render System"))
                Root::getSingleton().setRenderSystem(*iter);

            addOptionChoice("Render System", (*iter)->getName().c_str());
            opts = &(*iter)->getConfigOptions();
            for (ConfigOptionMap::iterator optIter = opts->begin(); optIter != opts->end(); optIter ++)
            {
                std::string fullName = (*iter)->getName() + "." + optIter->second.name;
                const char* curopt = getStringOption(fullName.c_str());
                setOptionType(fullName.c_str(), Option::OT_CHOICE);
                if(curopt[0] == 0)
                    setOption(fullName.c_str(), optIter->second.currentValue.c_str());
                else
                    (*iter)->setConfigOption(optIter->second.name, curopt);
                for(StringVector::iterator valIter = optIter->second.possibleValues.begin(); valIter != optIter->second.possibleValues.end(); valIter ++)
                {
                    addOptionChoice(fullName.c_str(), (*valIter).c_str());
                }
            }
        }

        mWindow = mRoot->initialise(true, "Magnetosphere");

        return true;
    }
    
    void GraphicsOgre::createScene()
    {
	    // setup GUI system
	    mGUIRenderer = new CEGUI::OgreRenderer(mWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 1000);
        new CEGUI::System(mGUIRenderer,(CEGUI::utf8*)getStringOption("CEGUI Log File"));

		//Logger::getSingleton().setLoggingLevel(Informative);

		// load scheme and set up defaults
        CEGUI::SchemeManager::getSingleton().loadScheme("TaharezLook.scheme");
        CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
        CEGUI::System::getSingleton().setDefaultFont("Commonwealth-10");

        CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root_wnd");
        CEGUI::System::getSingleton().setGUISheet(sheet);
		//createCrazyPanel();

		//
		// widget initialisation
		//
		// quit button
        CEGUI::PushButton* btn = (CEGUI::PushButton*)CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "QuitButton");
        sheet->addChildWindow(btn);
		btn->setText("Quit");
        btn->setPosition(CEGUI::Point(0.035f, 0.0f));
        btn->setSize(CEGUI::Size(0.1f, 0.036f));
		//btn->subscribeEvent(PushButton::EventClicked, &quitHandler);
		btn->setAlwaysOnTop(true);


    }

    void GraphicsOgre::createFrameListener()
    {
	    mFrameListener= new CEGUIOgre_FrameListener(mWindow, mCamera);
#ifndef MAGNET_DEBUG_BUILD
	    mFrameListener->showDebugOverlay(false);
#endif
	    mRoot->addFrameListener(mFrameListener);
    }

    void GraphicsOgre::shutdown()
    {
	    delete CEGUI::System::getSingletonPtr();

	    if (mGUIRenderer)
	    {
		    delete mGUIRenderer;
		    mGUIRenderer = 0;
	    }
    }

    ICmdWindow* GraphicsOgre::createCommandWindow(const char *name)
    {
        try
        {
            return new OgreCmdWindow(name);
        } catch(Ogre::Exception& e) {
            char a[1024];
            strcpy(a, e.getFullDescription().c_str());
            throw((char*)a);
        } catch(CEGUI::Exception &e) {
            char a[1024];
            strcpy(a, e.getMessage().c_str());
            throw((char*)a);
        }
    }

} // namespace ogre
} // namespace graphics
} // namespace magnet
