#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <stdlib.h>
#include <string.h>
#include "callbacks.h"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

/* Define the module info section */
PSP_MODULE_INFO("LUAINTERP", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

typedef struct {
    int initial, current;
} Memory;
Memory memusg;

int lua_printf(lua_State *L)
{
    int argc = lua_gettop(L);
    int n;
    for (n=1; n <= argc; n++) pspDebugScreenPrintf("%s", lua_tostring(L, n));
    return 0;
}

int lua_waitVblankStart(lua_State *L)
{
    if (lua_gettop(L) != 0) return luaL_error(L, "wrong number of arguments");
    sceDisplayWaitVblankStart();
    return 0;
}
/*
int lua_initializeMemory(lua_State *L)
{
    if (lua_gettop(L) != 0) return luaL_error(L, "wrong number of arguments");
    lua_getglobal(L, "collectgarbage");
    lua_call(L, 0, 1);
    memusg.initial = lua_tointeger(L,-1); 
    lua_pop(L, 1);       
    return 0;
}

int lua_getMemoryUsage(lua_State *L)
{
    if (lua_gettop(L) != 0) return luaL_error(L, "wrong number of arguments");
    lua_getglobal(L, "collectgarbage");
    lua_call(L, 0, 1);
    memusg.current = lua_tointeger(L,-1) - memusg.initial;
    lua_pop(L, 1);     
    lua_pushnumber(L, memusg.current); 
    return 1;
}*/

int main()
{
    pspDebugScreenInit();
    setupcallbacks();

    const char* script = "./script.lua";

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    
    //register stuff
    lua_register(L, "printf", lua_printf);
    lua_register(L, "waitVblankStart", lua_waitVblankStart);
    // register our own functions
    /*
    lua_register(L, "ctrlRead", lua_ctrlRead);
    lua_register(L, "isCtrlSelect", lua_isCtrlSelect);
    lua_register(L, "isCtrlStart", lua_isCtrlStart);
    lua_register(L, "isCtrlUp", lua_isCtrlUp);
    lua_register(L, "isCtrlRight", lua_isCtrlRight);
    lua_register(L, "isCtrlDown", lua_isCtrlDown);
    lua_register(L, "isCtrlLeft", lua_isCtrlLeft);
    lua_register(L, "isCtrlUp", lua_isCtrlUp);
    lua_register(L, "isCtrlLTrigger", lua_isCtrlLTrigger);
    lua_register(L, "isCtrlRTrigger", lua_isCtrlRTrigger);
    lua_register(L, "isCtrlTriangle", lua_isCtrlTriangle);
    lua_register(L, "isCtrlCircle", lua_isCtrlCircle);
    lua_register(L, "isCtrlCross", lua_isCtrlCross);
    lua_register(L, "isCtrlSquare", lua_isCtrlSquare);
    lua_register(L, "isCtrlHome", lua_isCtrlHome);
    lua_register(L, "isCtrlHold", lua_isCtrlHold);
*/
    // load script
    int status = luaL_loadfile(L, script);
    
    // call script
    if (status == 0) status = lua_pcall(L, 0, LUA_MULTRET, 0);
    
    // show error, if any
    if (status != 0) {
        pspDebugScreenPrintf("error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);  // remove error message
        while (1) {}
    }
    
    // cleanup
    lua_close(L);

    sceKernelExitGame();

    return 0;
}
