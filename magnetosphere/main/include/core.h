#if !defined(MAGNET_CORE_H_)
#define MAGNET_CORE_H_

#include "common.h"
#include "plugin.h"
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

        // Automatically called by plugins
        void setLibraryObject(const char* name, IPlugin* object);

    // Methods
    public:
        IPlugin* loadPlugin(const char* name);
            
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
        PluginMap* mPlugins;
        ConfigFile mConfiguration;
    };

} // namespace main
} // namespace magnet

#endif // !defined(MAGNET_CORE_H_)