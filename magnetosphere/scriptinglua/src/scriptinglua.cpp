#include "scriptinglua.h"
#include "core.h"

#include "lauxlib.h"
#include "lualib.h"

#define PLUGIN_NAME "Standard Lua Script Interpreter"
#define PLUGIN_VERS (float)0.01

namespace magnet {
namespace script {
namespace lua {

    INITIALISE_MAGNET_PLUGIN(ScriptingLua)

        ScriptingLua::ScriptingLua() : IScriptingPlugin(PLUGIN_NAME, PLUGIN_VERS), mLuaState(0), mInterface(0)
    {
    }

lua_State* L;
    void ScriptingLua::initialise()
    {
        mInterface = static_cast<Scripting*>(main::Core::getSingleton().getScripting());
        if (mInterface == 0)
            throw("Error initialising script interpreter: no scripting interface loaded");
        if (std::string(SCRIPTING_INTERFACE_PLUGIN_NAME) != mInterface->getName()
            || mInterface->getVersion() != SCRIPTING_INTERFACE_PLUGIN_VERSION)
        {
            throw("Incorrect scripting interface for script interpreter '" PLUGIN_NAME "'");
        }
        mLuaState = lua_open();
        L = mLuaState; // TODO: REMOVE ME
        if (mLuaState == 0)
        {
            throw("Failed to create environment for script interpreter");
        }
        luaopen_base(mLuaState); lua_settop(mLuaState, 0);
        luaopen_table(mLuaState); lua_settop(mLuaState, 0);
        luaopen_io(mLuaState); lua_settop(mLuaState, 0);
        luaopen_string(mLuaState); lua_settop(mLuaState, 0);
        luaopen_math(mLuaState); lua_settop(mLuaState, 0);
        luaopen_debug(mLuaState); lua_settop(mLuaState, 0);
        luaopen_loadlib(mLuaState); lua_settop(mLuaState, 0);
        main::Core::getSingleton().writeString(LUA_VERSION "  " LUA_COPYRIGHT);
        mInterface->setInterpreter(this);
    }
    
    /*
** this macro defines a function to show the prompt and reads the
** next line for manual input
*/
#ifndef lua_readline
#define lua_readline(L,prompt)		readline(L,prompt)

/* maximum length of an input line */
#ifndef MAXINPUT
#define MAXINPUT	512
#endif


static int readline (lua_State *l, const char *prompt) {
  static char buffer[MAXINPUT];
  if (prompt) {
    fputs(prompt, stdout);
    fflush(stdout);
  }
  if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    return 0;  /* read fails */
  else {
    lua_pushstring(l, buffer);
    return 1;
  }
}

#endif
        const char* progname;
static const char *get_prompt (int firstline) {
  const char *p = NULL;
  lua_pushstring(L, firstline ? "_PROMPT" : "_PROMPT2");
  lua_rawget(L, LUA_GLOBALSINDEX);
  p = lua_tostring(L, -1);
  if (p == NULL) p = (firstline ? "> " : "   ");
  lua_pop(L, 1);  /* remove global */
  return p;
}
static int incomplete (int status) {
  if (status == LUA_ERRSYNTAX &&
         strstr(lua_tostring(L, -1), "near `<eof>'") != NULL) {
    lua_pop(L, 1);
    return 1;
  }
  else
    return 0;
}
static int lcall (int narg, int clear) {
  int status;
  int base = lua_gettop(L) - narg;  /* function index */
  lua_pushliteral(L, "_TRACEBACK");
  lua_rawget(L, LUA_GLOBALSINDEX);  /* get traceback function */
  lua_insert(L, base);  /* put it under chunk and args */
  //signal(SIGINT, laction);
  status = lua_pcall(L, narg, (clear ? 0 : LUA_MULTRET), base);
  //signal(SIGINT, SIG_DFL);
  lua_remove(L, base);  /* remove traceback function */
  return status;
}
static void l_message (const char *pname, const char *msg) {
  //if (pname) fprintf(stderr, "%s: ", pname);
  //fprintf(stderr, "%s\n", msg);
  if (pname)
  {
      main::Core::getSingleton().writeString((std::string(pname) + ": " + msg).c_str());
  }
  else
  {
      main::Core::getSingleton().writeString(msg);
  }
}
static int report (int status) {
  const char *msg;
  if (status) {
    msg = lua_tostring(L, -1);
    if (msg == NULL) msg = "(error with no message)";
    l_message(progname, msg);
    lua_pop(L, 1);
  }
  return status;
}

    int ScriptingLua::load_string()
    {
        int status;
        //lua_settop(mLuaState, 0);
        //if (lua_readline(mLuaState, get_prompt(1)) == 0)  /* no input? */
        //    return -1;
        if (lua_tostring(mLuaState, -1)[0] == '=') {  /* line starts with `=' ? */
            lua_pushfstring(mLuaState, "return %s", lua_tostring(mLuaState, -1)+1);/* `=' -> `return' */
            lua_remove(mLuaState, -2);  /* remove original line */
        }
        for (;;) {  /* repeat until gets a complete line */
            status = luaL_loadbuffer(mLuaState, lua_tostring(mLuaState, 1), lua_strlen(mLuaState, 1), "=stdin");
            if (!incomplete(status)) break;  /* cannot try to add lines? */
            if (lua_readline(mLuaState, get_prompt(0)) == 0)  /* no more input? */
                return -1;
            lua_concat(mLuaState, lua_gettop(mLuaState));  /* join lines */
        }
        //lua_saveline(mLuaState, lua_tostring(mLuaState, 1));
        lua_remove(mLuaState, 1);  /* remove line */
        return status;
    }

    void ScriptingLua::runString(const char* string)
    {
        std::string outInfo = std::string(get_prompt(mCurScriptLine.empty())) + string;
        mCurScriptLine += string;
        mCurScriptLine += "\n";
        lua_settop(mLuaState, 0);
        progname = NULL;
        int status;
        lua_pushstring(L, mCurScriptLine.c_str());

        if (lua_tostring(mLuaState, -1)[0] == '=') {  /* line starts with `=' ? */
            lua_pushfstring(mLuaState, "return %s", lua_tostring(mLuaState, -1)+1);/* `=' -> `return' */
            lua_remove(mLuaState, -2);  /* remove original line */
        //} else if (strncmp("return", lua_tostring(mLuaState, -1), strlen("return")) != 0) {
        //    lua_pushfstring(mLuaState, "return %s", lua_tostring(mLuaState, -1));/* `return' */
        //    lua_remove(mLuaState, -2);  /* remove original line */
        }
        status = luaL_loadbuffer(mLuaState, lua_tostring(mLuaState, 1), lua_strlen(mLuaState, 1), "=stdin");
        if (incomplete(status)) 
        {
            outInfo += " ...";
            main::Core::getSingleton().writeString(outInfo.c_str());
        }
        else
        {
            main::Core::getSingleton().writeString(outInfo.c_str());
            mCurScriptLine.clear();
            lua_remove(mLuaState, 1);  /* remove line */


            if (status == 0) status = lcall(0, 0);
            report(status);
            if (status == 0 && lua_gettop(mLuaState) > 0) {  /* any result to print? */
                lua_getglobal(mLuaState, "print");
                lua_insert(mLuaState, 1);
                if (lua_pcall(mLuaState, lua_gettop(mLuaState)-1, 0, 0) != 0)
                    l_message(progname, lua_pushfstring(mLuaState, "error calling `print' (%s)",
                    lua_tostring(mLuaState, -1)));
            }
        }
        lua_settop(L, 0);  /* clear stack */
        return;
    }

    void ScriptingLua::shutdown()
    {
        if (mLuaState != 0)
        {
            lua_close(mLuaState);
            mLuaState = 0;
        }
    }

    extern "C" int luaB_print (lua_State *L) {
        int n = lua_gettop(L);  /* number of arguments */
        int i;
        lua_getglobal(L, "tostring");
        for (i=1; i<=n; i++) {
            const char *s;
            lua_pushvalue(L, -1);  /* function to be called */
            lua_pushvalue(L, i);   /* value to print */
            lua_call(L, 1, 1);
            s = lua_tostring(L, -1);  /* get result */
            if (s == NULL)
                return luaL_error(L, "`tostring' must return a string to `print'");
            std::string outval;
            if (i>1) outval = "\t";//fputs("\t", stdout);
            //fputs(s, stdout);
            outval += s;
            main::Core::getSingleton().writeString(outval.c_str());
            lua_pop(L, 1);  /* pop result */
        }
        fputs("\n", stdout);
        return 0;
    }


} // namespace lua
} // namespace script
} // namespace magnet