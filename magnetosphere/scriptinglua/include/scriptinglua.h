#if !defined(MAGNET_SCRIPTING_LUA_H_)
#define MAGNET_SCRIPTING_LUA_H_

#include "common.h"
#include "scripting.h"
#include "scriptingplugin.h"

#define LUA_API extern "C"
#include "lua.h"

namespace magnet {
namespace script {
namespace lua {

    class ScriptingLua : public IScriptingPlugin {
    // Class
    public:
        ScriptingLua();

    // Methods
    public:
        void initialise();
        void shutdown();
        void runString(const char* string);
    private:
        int load_string();

    // Data
    private:
        Scripting* mInterface;
        lua_State *mLuaState;
        std::string mCurScriptLine;
    };

} // namespace ogre
} // namespace graphics
} // namespace magnet

#endif // !defined(MAGNET_SCRIPTING_LUA_H_)