#include "scriptingplugin.h"

namespace magnet {
namespace script {

    IScriptingPlugin::IScriptingPlugin(const char* name, const float version)
        : main::IPlugin(name, version)
    {
    }

} // namespace script
} // namespace magnet 
