#if !defined(MAGNET_SCRIPTING_H_)
#define MAGNET_SCRIPTING_H_

#include "scriptingcommon.h"
#include "plugininterfaces.h"
#include "scriptingplugin.h"

namespace magnet {
namespace script {

    class MAG_SCRIPTING_EXPORT Scripting : public main::IScriptingInterface {
    // Class
    public:
        Scripting();
    
    // Methods
    public:
        void initialise();
        void runString(const char*);
        void shutdown();
        void setInterpreter(IScriptingPlugin *interpreter);
    // Data
    private:
        IScriptingPlugin* mInterpreter;
    };

} // namespace scripting
} // namespace magnet

#endif // !defined(MAGNET_SCRIPTING_H_) 
