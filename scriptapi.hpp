extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include <string>

class ScriptAPI {
    public:
        ScriptAPI();
        ~ScriptAPI();
        std::string Execute(std::string epName);
    private:
        lua_State* internalLuaState;
};
