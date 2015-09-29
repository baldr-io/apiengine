#include "scriptapi.hpp"
#include <string>
#include <iostream>

using namespace std;

ScriptAPI::ScriptAPI() {
}

ScriptAPI::~ScriptAPI() {
}

string ScriptAPI::Execute(std::string epName) {
    string luaResponse;
    internalLuaState = luaL_newstate();
    luaL_openlibs(internalLuaState);
    if(luaL_loadfile(internalLuaState, string("." + epName + ".lua").c_str())) {
        cerr << "Could not load " << epName << endl;
    }

    if(lua_pcall(internalLuaState,0,0,0)) {
        cerr << "Could not prime LUA script" << endl;
    }

    lua_getglobal(internalLuaState, "runapi");

    if(lua_pcall(internalLuaState,0,1,0)) {
        cerr << "Could not find runapi method in LUA" << endl;
    }
    
    luaResponse = string(lua_tostring(internalLuaState, -1));

    cout << "Value returned from LUA: " << luaResponse << endl;

    lua_close(internalLuaState);

    return luaResponse;
}
