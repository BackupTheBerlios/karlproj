#if !defined(MAGNET_SCRIPTINGPLUGIN_H_)
#define MAGNET_SCRIPTINGPLUGIN_H_

#include "scriptingcommon.h"
#include "plugin.h"

namespace magnet {
namespace script {

    class MAG_SCRIPTING_EXPORT IScriptingPlugin : public main::IPlugin
    {
    // Class
    public:
        IScriptingPlugin(const char* name, const float version);
    // Methods
    public:
        virtual void runString(const char* string) = 0;
    };

} // namespace script
} // namespace magnet

#endif // !defined(MAGNET_SCRIPTINGPLUGIN_H_) 
