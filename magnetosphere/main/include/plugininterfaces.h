#if !defined(MAGNET_PLUGININTERFACES_H_)
#define MAGNET_PLUGININTERFACES_H_

#include "plugin.h"

namespace magnet {
namespace main {

    class MAG_MAIN_EXPORT IGraphicsInterface : public IPlugin
    {
    // Class
    public:
        IGraphicsInterface(const char* name, float ver) : IPlugin(name, ver) {}

    // Methods
    public:
        virtual bool update() = 0;
        virtual void writeString(const char* string) = 0;
    };

    class MAG_MAIN_EXPORT IScriptingInterface : public IPlugin
    {
    // Class
    public:
        IScriptingInterface(const char* name, float ver) : IPlugin(name, ver) {}

    // Methods
    public:
        virtual void runString(const char* string) = 0;
    };

} // namespace main
} // namespace magnet

#endif // !defined(MAGNET_PLUGININTERFACES_H_)