#include "scripting.h"
#include "core.h"

#include <sstream>

namespace magnet {
namespace script {

    INITIALISE_MAGNET_PLUGIN(Scripting)

    Scripting::Scripting() : IScriptingInterface(SCRIPTING_INTERFACE_PLUGIN_NAME, (float)SCRIPTING_INTERFACE_PLUGIN_VERSION), mInterpreter(0)
    {
        setOptionType("Interpreter", Option::OT_PLUGIN);
        setOption("Interpreter", "scriptinglua.mpi");
    }

    void Scripting::initialise()
    {
        main::Core::getSingleton().queuePlugin(getStringOption("Interpreter"));
    }

    void Scripting::runString(const char* string)
    {
        if (mInterpreter == 0)
        {
            std::stringstream ss;
            ss << "> " << string;
            main::Core::getSingleton().writeString(ss.str().c_str());
            main::Core::getSingleton().writeString("::No script interpreter loaded!");
        }
        else
        {
            mInterpreter->runString(string);
        }
    }

    void Scripting::setInterpreter(IScriptingPlugin* interpreter)
    {
        mInterpreter = interpreter;
    }

    void Scripting::shutdown()
    {
        if (mInterpreter != 0)
        {
            mInterpreter = 0;
        }
    }

} // namespace script
} // namespace magnet
 
