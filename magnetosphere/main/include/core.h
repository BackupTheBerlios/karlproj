#if !defined(MAGNET_CORE_H_)
#define MAGNET_CORE_H_

#include "common.h"
#include "plugin.h"
#include "plugininterfaces.h"
#include "cfgfile.h"

#include <map>
#include <string>

namespace magnet {
namespace main {

    class MAG_MAIN_EXPORT Core
    {
    // Class
    public:
        Core(const char* cfgFile = "magnet.cfg");
        ~Core();
        static Core& getSingleton();

    // Methods
    public:
        void go();
        bool update();

        // Output an informative message to any available of the main Magnetosphere window, log, and stdout
        void writeString(const char* String);
        // Interpret a typed instruction from the user via the scripting system
        void runString(const char* String);
        
        // Acquire plugin objects
        IGraphicsInterface* getGraphics();
        IScriptingInterface* getScripting();
        INetworkInterface* getNetwork();

        // Immediately loads and initialises a new plugin
        IPlugin* loadPlugin(const char* name);
        // Queues a plugin for loading when done with the current task
        void queuePlugin(const char* name);

        // Automatically called by plugins
        void setLibraryObject(const char* name, IPlugin* object);

        // Can be called once prior to destructor to unload everything
        void shutdown();
    
    private:
        void emptyPluginQueue();

    // Types
    private:
        struct PluginHandle
        {
            void* platformHandle;
            IPlugin* object;
        };
        typedef std::map<std::string,PluginHandle> PluginMap;

    // Data
    private:
        IGraphicsInterface* mGraphics;
        IScriptingInterface* mScripting;
        INetworkInterface* mNetwork;
        PluginMap* mPlugins;
        std::vector<std::string>* mQueuedPlugins;
        ConfigFile mConfiguration;
        std::string mLogBuf;
        std::string mRunBuf;
    };

} // namespace main
} // namespace magnet

#endif // !defined(MAGNET_CORE_H_)