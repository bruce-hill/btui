/*
* lbtui.c
* A Lua library binding for btui, Bruce's Text User Interface library.
*/

#include "lua.h"
#include "lauxlib.h"
#include "btui.h"

// The C API changed from 5.1 to 5.2, so these shims help the code compile on >=5.2
#if LUA_VERSION_NUM >= 502
#define lua_objlen(L, i) lua_rawlen(L, i)
#define luaL_register(L, _, R) luaL_setfuncs(L, R, 0)
#endif
// Lua 5.3 introduced lua_isinteger, fall back to lua_isnumber
#if LUA_VERSION_NUM < 503
#define lua_isinteger(L, i) lua_isnumber(L, i)
#endif

const int BTUI_METATABLE;

static int Lbtui_enable(lua_State *L)
{
    btui_t **bt = (btui_t**)lua_touserdata(L, 1);
    if (bt == NULL) luaL_error(L, "Not a BTUI object");
    *bt = btui_enable();
    btui_move_cursor(*bt, 0, 0);
    btui_flush(*bt);
    return 0;
}

static int Lbtui_disable(lua_State *L)
{
    btui_t **bt = (btui_t**)lua_touserdata(L, 1);
    if (bt == NULL) luaL_error(L, "Not a BTUI object");
    if (*bt == NULL) luaL_error(L, "BTUI object not initialized");
    btui_disable(*bt);
    return 0;
}

static int Lbtui_getkey(lua_State *L)
{
    btui_t **bt = (btui_t**)lua_touserdata(L, 1);
    if (bt == NULL) luaL_error(L, "Not a BTUI object");
    if (*bt == NULL) luaL_error(L, "BTUI object not initialized");
    int mouse_x = -1, mouse_y = -1;
    int key = btui_getkey(*bt, &mouse_x, &mouse_y);
    if (key == -1) return 0;
    char buf[256] = {0};
    btui_keyname(key, buf);
    lua_pushstring(L, buf);
    if (mouse_x != -1 || mouse_y != -1) {
        lua_pushinteger(L, mouse_x);
        lua_pushinteger(L, mouse_y);
        return 3;
    }
    return 1;
}

static int Lbtui_print(lua_State *L)
{
    btui_t **bt = (btui_t**)lua_touserdata(L, 1);
    if (bt == NULL) luaL_error(L, "Not a BTUI object");
    int n = lua_gettop(L);
    for (int i = 2; i <= n; i++) {
        btui_puts(*bt, luaL_tolstring(L, i, NULL));
        lua_pop(L, 1);
    }
    btui_flush(*bt);
    return 0;
}

static int Lbtui_clear(lua_State *L)
{
    btui_t **bt = (btui_t**)lua_touserdata(L, 1);
    if (bt == NULL) luaL_error(L, "Not a BTUI object");
    btui_clear(*bt);
    btui_flush(*bt);
    return 0;
}

static int Lbtui_move(lua_State *L)
{
    btui_t **bt = (btui_t**)lua_touserdata(L, 1);
    if (bt == NULL) luaL_error(L, "Not a BTUI object");
    if (lua_gettop(L) < 3) luaL_error(L, "Expected x and y values");
    int isnum;
    int x = lua_tointegerx(L, 2, &isnum);
    if (!isnum) luaL_error(L, "Expected integer x value");
    int y = lua_tointegerx(L, 3, &isnum);
    if (!isnum) luaL_error(L, "Expected integer y value");
    btui_move_cursor(*bt, x, y);
    btui_flush(*bt);
    return 0;
}

static int Lbtui_withfg(lua_State *L)
{
    btui_t **bt = (btui_t**)lua_touserdata(L, 1);
    if (bt == NULL) luaL_error(L, "Not a BTUI object");
    if (lua_gettop(L) < 5) luaL_error(L, "Expected r,g,b values and a function");
    int isnum;
    int r = lua_tointegerx(L, 2, &isnum);
    if (!isnum) luaL_error(L, "Expected integer r value");
    int g = lua_tointegerx(L, 3, &isnum);
    if (!isnum) luaL_error(L, "Expected integer g value");
    int b = lua_tointegerx(L, 4, &isnum);
    if (!isnum) luaL_error(L, "Expected integer b value");
    btui_set_fg_rgb(*bt, r, g, b);
    int top = lua_gettop(L);
    int status = lua_pcall(L, 0, LUA_MULTRET, 0);
    btui_set_attributes(*bt, BTUI_FG_NORMAL);
    if (status != LUA_OK)
        lua_error(L);
    return lua_gettop(L) - top;
}

static int Lbtui_withbg(lua_State *L)
{
    btui_t **bt = (btui_t**)lua_touserdata(L, 1);
    if (bt == NULL) luaL_error(L, "Not a BTUI object");
    if (lua_gettop(L) < 5) luaL_error(L, "Expected r,g,b values and a function");
    int isnum;
    int r = lua_tointegerx(L, 2, &isnum);
    if (!isnum) luaL_error(L, "Expected integer r value");
    int g = lua_tointegerx(L, 3, &isnum);
    if (!isnum) luaL_error(L, "Expected integer g value");
    int b = lua_tointegerx(L, 4, &isnum);
    if (!isnum) luaL_error(L, "Expected integer b value");
    btui_set_bg_rgb(*bt, r, g, b);
    int top = lua_gettop(L);
    int status = lua_pcall(L, 0, LUA_MULTRET, 0);
    btui_set_attributes(*bt, BTUI_BG_NORMAL);
    if (status != LUA_OK)
        lua_error(L);
    return lua_gettop(L) - top;
}


static int Lbtui_width(lua_State *L)
{
    btui_t **bt = (btui_t**)lua_touserdata(L, 1);
    if (bt == NULL) luaL_error(L, "Not a BTUI object");
    lua_pushinteger(L, (*bt)->width);
    return 1;
}

static int Lbtui_height(lua_State *L)
{
    btui_t **bt = (btui_t**)lua_touserdata(L, 1);
    if (bt == NULL) luaL_error(L, "Not a BTUI object");
    lua_pushinteger(L, (*bt)->height);
    return 1;
}

static int Lbtui_wrap(lua_State *L)
{
    if (lua_gettop(L) < 1) luaL_error(L, "expected a callable object");
    btui_t **bt = (btui_t**)lua_newuserdata(L, sizeof(btui_t*));
    lua_pushlightuserdata(L, (void*)&BTUI_METATABLE);
    lua_gettable(L, LUA_REGISTRYINDEX);
    lua_setmetatable(L, -2);
    *bt = btui_enable();
    btui_move_cursor(*bt, 0, 0);
    int status = lua_pcall(L, 1, 0, 0);
    btui_disable(*bt);
    if (status != LUA_OK)
        lua_error(L);
    return 0;
}

static int Lbtui_tostring(lua_State *L)
{
    lua_pushliteral(L, "<BTUI>");
    return 1;
}

static const luaL_Reg Rclass_metamethods[] =
{
    { "__tostring", Lbtui_tostring},
    { "enable",     Lbtui_enable},
    { "disable",    Lbtui_disable},
    { "getkey",     Lbtui_getkey},
    { "print",      Lbtui_print},
    { "clear",      Lbtui_clear},
    { "move",       Lbtui_move},
    { "withfg",     Lbtui_withfg},
    { "withbg",     Lbtui_withbg},
    { "width",      Lbtui_width},
    { "height",     Lbtui_height},
    { NULL,         NULL}
};

LUALIB_API int luaopen_btui(lua_State *L)
{
    lua_pushlightuserdata(L, (void*)&BTUI_METATABLE);
    lua_createtable(L, 0, 16);
    luaL_register(L, NULL, Rclass_metamethods);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    lua_settable(L, LUA_REGISTRYINDEX);

    lua_pushcfunction(L, Lbtui_wrap);
    return 1;
}
