#include "core.h"
#include "platform.h"

#include <iostream>

namespace magnet {
namespace main {

    Core* singletonCore = 0;

    Core::Core(const char* cfgFile) : mConfiguration(cfgFile)
    {
        if (singletonCore != 0)
            throw("Core object constructed twice!");
        singletonCore = this;
        mPlugins = new PluginMap();
        Platform::SetSharedLibraryPath(
            mConfiguration.getValue("Core", "Plugin Folder", "plugins") );
        loadPlugin(
            mConfiguration.getValue("Core", "Graphics Interface", "graphics.mpi") );
        mConfiguration.write();
    }

    Core::~Core()
    {
        delete mPlugins;
        singletonCore = 0;
    }

    Core& Core::getSingleton()
    {
        return *singletonCore;
    }

    IPlugin* Core::loadPlugin(const char* name)
    {
        // Load library
        void* handle = Platform::LoadSharedLibrary(name);

        // Check that the library exports a plugin object
        IPlugin* (*getModuleObject)() = (IPlugin*(*)())Platform::GetSharedLibrarySymbolAddress(handle, "getModuleObject");
        if (getModuleObject == 0)
        {
            std::cout << "Failed to find getModuleObject function in plugin '" << name << "'!" << std::endl;
            return 0;
        }

        // Check to make sure the plugin constructor gave us the same object as the plugin exports
        IPlugin* moduleObject = getModuleObject();
        const char* moduleName = moduleObject->getName();
        if ((*mPlugins)[moduleName].object != moduleObject)
        {
            std::cout << "WARNING: Plugin '" << moduleObject->getName() << "' failed to initialise properly!" << std::endl;
        }

        // Store the platform-dependant handle
        (*mPlugins)[moduleName].platformHandle = handle;

        // Load the plugin options
        const ConfigFile::SettingMap* settings = mConfiguration.getSection(moduleName);
        for (ConfigFile::SettingMap::const_iterator i = settings->begin(); i != settings->end(); i ++)
        {
            moduleObject->setOption(i->first.c_str(), i->second.c_str());
        }
        // This is maybe a good time to copy over any options we don't have?
        // No, it isn't, not at all.
        // TODO: move this
        const IPlugin::OptionsMap* options = moduleObject->getOptions();
        for (IPlugin::OptionsMap::const_iterator i = options->begin(); i != options->end(); i ++)
        {
            mConfiguration.setValue(moduleName, i->first.c_str(), i->second.value.c_str());
        }

        // Initialise plugin now its loaded up with options
        moduleObject->initialise();

        // Done
        std::cout << "Loaded " << moduleName << " v" << moduleObject->getVersion() << std::endl;
        return moduleObject;
    }
    void Core::setLibraryObject(const char* name, IPlugin* object)
    {
        (*mPlugins)[name].object = object;
    }

} // namespace main
} // namespace magnet