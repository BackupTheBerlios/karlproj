#include "core.h"
#include "platform.h"

#include <iostream>
#include <sstream>

namespace magnet {
namespace main {

    Core* singletonCore = 0;

    Core::Core(const char* cfgFile) : mPlugins(0), mGraphics(0), mNetwork(0)
    {
        if (singletonCore != 0)
            throw("Core object constructed twice!");
        singletonCore = this;
        mQueuedPlugins = new std::vector<std::string>();
        mPlugins = new PluginMap();
        mConfiguration.setFileName(cfgFile);
        mConfiguration.read();
        Platform::setSharedLibraryPath(
            mConfiguration.getValue("Core", "Plugin Folder", "plugins") );
        mGraphics = static_cast<IGraphicsInterface*>(loadPlugin(
            mConfiguration.getValue("Core", "Graphics Interface", "graphics.mpi") ));
        update();
        mScripting = static_cast<IScriptingInterface*>(loadPlugin(
            mConfiguration.getValue("Core", "Scripting Interface", "scripting.mpi") ));
        update();
        mNetwork = static_cast<INetworkInterface*>(loadPlugin(
            mConfiguration.getValue("Core", "Network Interface", "network.mpi") ));
        update();
        mConfiguration.write();
    }

    void Core::go()
    {
        while (update());
    }

    bool Core::update()
    {
        bool retval = Platform::update();
        if (mGraphics != 0)
            retval &= mGraphics->update();
        if (mNetwork != 0)
            retval &= mNetwork->update();
        emptyPluginQueue();
        return retval;
    }

    void Core::writeString(const char* string)
    {
        if (mGraphics == 0)
        {
            mLogBuf += string;
            mLogBuf += "\n";
        } else {
            if (mLogBuf.size() != 0)
            {
                mLogBuf += string;
                mGraphics->writeString(mLogBuf.c_str());
                mLogBuf.clear();
            } else {
                mGraphics->writeString(string);
            }
        }
        std::cout << string << std::endl;
    }
    void Core::runString(const char* string)
    {
        if (mScripting == 0)
        {
            mRunBuf += string;
            mRunBuf += "\n";
        } else {
            if (mRunBuf.size() != 0)
            {
                mRunBuf += string;
                mScripting->runString(mRunBuf.c_str());
                mRunBuf.clear();
            } else {
                mScripting->runString(string);
            }
        }
    }

    void Core::shutdown()
    {
        if (mPlugins)
        {
            PluginMap::iterator iter;
            // Shut down all plugins
            for (iter = mPlugins->begin(); iter != mPlugins->end(); iter ++)
            {
                // This is maybe a good time to copy over any changed or added options we don't have?
                const IPlugin::OptionsMap* options = iter->second.object->getOptions();
                for (IPlugin::OptionsMap::const_iterator i = options->begin(); i != options->end(); i ++)
                {
                    mConfiguration.setValue(iter->second.object->getName(), i->first.c_str(), i->second.value.c_str());
                }
                // Shut down the plugin
                iter->second.object->shutdown();
            }
            // Write any changed or added options
            mConfiguration.write();
            // Unload all plugins
            for (iter = mPlugins->begin(); iter != mPlugins->end(); iter ++)
            {
                Platform::unloadSharedLibrary(iter->second.platformHandle);
            }
            delete mPlugins; mPlugins = 0;
        }
        if (mQueuedPlugins != 0)
        {
            delete mQueuedPlugins;
            mQueuedPlugins = 0;
        }
        if (singletonCore)
        {
            singletonCore = 0;
        }
    }

    IGraphicsInterface* Core::getGraphics()
    {
        return mGraphics;
    }

    IScriptingInterface* Core::getScripting()
    {
        return mScripting;
    }

    INetworkInterface* Core::getNetwork()
    {
        return mNetwork;
    }
    
    Core::~Core()
    {
        shutdown();
    }

    Core& Core::getSingleton()
    {
        return *singletonCore;
    }

    void Core::queuePlugin(const char*name)
    {
        mQueuedPlugins->push_back(name);
    }

    void Core::emptyPluginQueue()
    {
        if (mQueuedPlugins->empty())
            return;
        for (unsigned int a = 0; a < mQueuedPlugins->size(); a ++)
        {
            loadPlugin((*mQueuedPlugins)[a].c_str());
        }
        mQueuedPlugins->clear();
    }

    IPlugin* Core::loadPlugin(const char* name)
    {
        // Load library
        void* handle = Platform::loadSharedLibrary(name);

        // Check that the library exports a plugin object
        IPlugin* (*getModuleObject)() = (IPlugin*(*)())Platform::getSharedLibrarySymbolAddress(handle, "getModuleObject");
        if (getModuleObject == 0)
        {
            std::stringstream ss;
            ss << "Failed to find getModuleObject function in plugin '" << name << "'!";
            writeString(ss.str().c_str());
            return 0;
        }

        // Let the user know what's taking so danged long
        IPlugin* moduleObject = getModuleObject();
        const char* moduleName = moduleObject->getName();
        std::stringstream ss;
        ss << "Loading " << moduleName << " v" << moduleObject->getVersion() << " from " << name;
        writeString(ss.str().c_str());

        // Check to make sure the plugin constructor gave us the same object as the plugin exports
        if ((*mPlugins)[moduleName].object != moduleObject)
        {
            std::stringstream ss;
            ss << "WARNING: Plugin '" << moduleName << "' failed to initialise properly!";
            writeString(ss.str().c_str());
        }

        // Store the platform-dependant handle
        (*mPlugins)[moduleName].platformHandle = handle;

        // Load the plugin options
        const ConfigFile::SettingMap* settings = mConfiguration.getSection(moduleName);
        for (ConfigFile::SettingMap::const_iterator i = settings->begin(); i != settings->end(); i ++)
        {
            moduleObject->setOption(i->first.c_str(), i->second.c_str());
        }

        // Initialise plugin now its loaded up with options
        moduleObject->initialise();

        // Done
        return moduleObject;
    }
    void Core::setLibraryObject(const char* name, IPlugin* object)
    {
        (*mPlugins)[name].object = object;
    }

} // namespace main
} // namespace magnet