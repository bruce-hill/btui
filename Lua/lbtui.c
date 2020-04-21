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

const int BTUI_METATABLE, BTUI_ATTRIBUTES, BTUI_INVERSE_ATTRIBUTES;

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

static int Lbtui_withdisabled(lua_State *L)
{
    btui_t **bt = (btui_t**)lua_touserdata(L, 1);
    if (bt == NULL) luaL_error(L, "Not a BTUI object");
    int top = lua_gettop(L);
    if (top < 2) luaL_error(L, "No function provided");

    lua_pushcfunction(L, Lbtui_disable);
    lua_pushvalue(L, 1);
    lua_call(L, 1, 0);

    lua_pushvalue(L, 2);
    lua_call(L, 0, LUA_MULTRET);
    int top2 = lua_gettop(L);

    lua_pushcfunction(L, Lbtui_enable);
    lua_pushvalue(L, 1);
    lua_call(L, 1, 0);

    return top2 - top;
}

static int Lbtui_getkey(lua_State *L)
{
    btui_t **bt = (btui_t**)lua_touserdata(L, 1);
    if (bt == NULL) luaL_error(L, "Not a BTUI object");
    if (*bt == NULL) luaL_error(L, "BTUI object not initialized");
    int mouse_x = -1, mouse_y = -1;
    int timeout = lua_gettop(L) <= 1 ? -1 : (int)luaL_checkinteger(L, 2);
    int key = btui_getkey(*bt, timeout, &mouse_x, &mouse_y);
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

static int Lbtui_write(lua_State *L)
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
    const char *cleartype = luaL_optlstring(L, 2, "screen", NULL);
    if (strcmp(cleartype, "screen") == 0) {
        btui_clear(*bt, BTUI_CLEAR_SCREEN);
    } else if (strcmp(cleartype, "below") == 0) {
        btui_clear(*bt, BTUI_CLEAR_BELOW);
    } else if (strcmp(cleartype, "above") == 0) {
        btui_clear(*bt, BTUI_CLEAR_ABOVE);
    } else if (strcmp(cleartype, "right") == 0) {
        btui_clear(*bt, BTUI_CLEAR_RIGHT);
    } else if (strcmp(cleartype, "left") == 0) {
        btui_clear(*bt, BTUI_CLEAR_LEFT);
    } else if (strcmp(cleartype, "line") == 0) {
        btui_clear(*bt, BTUI_CLEAR_LINE);
    } else {
        lua_pushliteral(L, "unknown clear type");
        lua_error(L);
    }
    btui_flush(*bt);
    return 0;
}

static int Lbtui_flush(lua_State *L)
{
    btui_t **bt = (btui_t**)lua_touserdata(L, 1);
    if (bt == NULL) luaL_error(L, "Not a BTUI object");
    btui_flush(*bt);
    return 0;
}

static int Lbtui_move(lua_State *L)
{
    btui_t **bt = (btui_t**)lua_touserdata(L, 1);
    if (bt == NULL) luaL_error(L, "Not a BTUI object");
    if (lua_gettop(L) < 3) luaL_error(L, "Expected x and y values");
    int x = (int)luaL_checkinteger(L, 2);
    int y = (int)luaL_checkinteger(L, 3);
    btui_move_cursor(*bt, x, y);
    btui_flush(*bt);
    return 0;
}

static int Lbtui_withfg(lua_State *L)
{
    btui_t **bt = (btui_t**)lua_touserdata(L, 1);
    if (bt == NULL) luaL_error(L, "Not a BTUI object");
    if (lua_gettop(L) < 5) luaL_error(L, "Expected r,g,b values and a function");
    lua_Number r = luaL_checknumber(L, 2);
    lua_Number g = luaL_checknumber(L, 3);
    lua_Number b = luaL_checknumber(L, 4);
    btui_set_fg(*bt,
                r < 0.0 ? 0 : (r > 1.0 ? 255 : (int)(255.0 * r)),
                g < 0.0 ? 0 : (g > 1.0 ? 255 : (int)(255.0 * g)),
                b < 0.0 ? 0 : (b > 1.0 ? 255 : (int)(255.0 * b)));
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
    lua_Number r = luaL_checknumber(L, 2);
    lua_Number g = luaL_checknumber(L, 3);
    lua_Number b = luaL_checknumber(L, 4);
    btui_set_bg(*bt,
                r < 0.0 ? 0 : (r > 1.0 ? 255 : (int)(255.0 * r)),
                g < 0.0 ? 0 : (g > 1.0 ? 255 : (int)(255.0 * g)),
                b < 0.0 ? 0 : (b > 1.0 ? 255 : (int)(255.0 * b)));
    int top = lua_gettop(L);
    int status = lua_pcall(L, 0, LUA_MULTRET, 0);
    btui_set_attributes(*bt, BTUI_BG_NORMAL);
    if (status != LUA_OK)
        lua_error(L);
    return lua_gettop(L) - top;
}

static int Lbtui_linebox(lua_State *L)
{
    btui_t **bt = (btui_t**)lua_touserdata(L, 1);
    if (bt == NULL) luaL_error(L, "Not a BTUI object");
    int x = (int)luaL_checkinteger(L, 2);
    int y = (int)luaL_checkinteger(L, 3);
    int w = (int)luaL_checkinteger(L, 4);
    int h = (int)luaL_checkinteger(L, 5);
    btui_draw_linebox(*bt, x, y, w, h);
    return 0;
}

static int Lbtui_fillbox(lua_State *L)
{
    btui_t **bt = (btui_t**)lua_touserdata(L, 1);
    if (bt == NULL) luaL_error(L, "Not a BTUI object");
    int x = (int)luaL_checkinteger(L, 2);
    int y = (int)luaL_checkinteger(L, 3);
    int w = (int)luaL_checkinteger(L, 4);
    int h = (int)luaL_checkinteger(L, 5);
    btui_fill_box(*bt, x, y, w, h);
    return 0;
}

static int Lbtui_shadow(lua_State *L)
{
    btui_t **bt = (btui_t**)lua_touserdata(L, 1);
    if (bt == NULL) luaL_error(L, "Not a BTUI object");
    int x = (int)luaL_checkinteger(L, 2);
    int y = (int)luaL_checkinteger(L, 3);
    int w = (int)luaL_checkinteger(L, 4);
    int h = (int)luaL_checkinteger(L, 5);
    btui_draw_shadow(*bt, x, y, w, h);
    return 0;
}

static int Lbtui_scroll(lua_State *L)
{
    btui_t **bt = (btui_t**)lua_touserdata(L, 1);
    if (bt == NULL) luaL_error(L, "Not a BTUI object");
    int firstline = (int)luaL_checkinteger(L, 2);
    int lastline = (int)luaL_checkinteger(L, 3);
    int scroll = (int)luaL_checkinteger(L, 4);
    btui_scroll(*bt, firstline, lastline, scroll);
    return 0;
}

static int Lbtui_setattributes(lua_State *L)
{
    btui_t **bt = (btui_t**)lua_touserdata(L, 1);
    if (bt == NULL) luaL_error(L, "Not a BTUI object");
    int top = lua_gettop(L);
    lua_pushlightuserdata(L, (void*)&BTUI_ATTRIBUTES);
    int attr_table = lua_gettop(L);
    lua_gettable(L, LUA_REGISTRYINDEX);
    lua_Unsigned attrs = 0;
    for (int i = 2; i <= top; i++) {
        lua_pushvalue(L, i);
        lua_gettable(L, attr_table);
        if (lua_isnil(L, -1)) {
            const char *a = lua_tostring(L, i);
            luaL_error(L, "invalid attribute: %s", a);
        }
        attrs |= (lua_Unsigned)lua_tointeger(L, -1);
    }
    btui_set_attributes(*bt, attrs);
    return 0;
}

static int Lbtui_unsetattributes(lua_State *L)
{
    btui_t **bt = (btui_t**)lua_touserdata(L, 1);
    if (bt == NULL) luaL_error(L, "Not a BTUI object");
    int top = lua_gettop(L);
    lua_pushlightuserdata(L, (void*)&BTUI_INVERSE_ATTRIBUTES);
    int attr_table = lua_gettop(L);
    lua_gettable(L, LUA_REGISTRYINDEX);
    lua_Unsigned attrs = 0;
    for (int i = 2; i <= top; i++) {
        lua_pushvalue(L, i);
        lua_gettable(L, attr_table);
        if (lua_isnil(L, -1)) {
            const char *a = lua_tostring(L, i);
            luaL_error(L, "invalid attribute: %s", a);
        }
        attrs |= (lua_Unsigned)lua_tointeger(L, -1);
    }
    btui_set_attributes(*bt, attrs);
    return 0;
}

static int Lbtui_suspend(lua_State *L)
{
    btui_t **bt = (btui_t**)lua_touserdata(L, 1);
    if (bt == NULL) luaL_error(L, "Not a BTUI object");
    btui_suspend(*bt);
    return 0;
}

static int Lbtui_withattributes(lua_State *L)
{
    btui_t **bt = (btui_t**)lua_touserdata(L, 1);
    if (bt == NULL) luaL_error(L, "Not a BTUI object");
    int top = lua_gettop(L);
    lua_pushcfunction(L, Lbtui_setattributes);
    for (int i = 1; i < top; i++) {
        lua_pushvalue(L, i);
    }
    lua_call(L, top - 1, 0);
    lua_pushvalue(L, top);
    int status = lua_pcall(L, 0, LUA_MULTRET, 0);
    int top2 = lua_gettop(L);
    lua_pushcfunction(L, Lbtui_unsetattributes);
    for (int i = 1; i < top; i++) {
        lua_pushvalue(L, i);
    }
    lua_call(L, top - 1, 0);
    if (status != LUA_OK)
        lua_error(L);
    return top2 - top;
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

static struct {
    const char* name;
    lua_Unsigned code;
} btui_attributes[] =
{
    {"normal",               BTUI_NORMAL},
    {"bold",                 BTUI_BOLD},
    {"faint",                BTUI_FAINT},
    {"dim",                  BTUI_FAINT},
    {"italic",               BTUI_ITALIC},
    {"underline",            BTUI_UNDERLINE},
    {"blink_slow",           BTUI_BLINK_SLOW},
    {"blink_fast",           BTUI_BLINK_FAST},
    {"reverse",              BTUI_REVERSE},
    {"conceal",              BTUI_CONCEAL},
    {"strikethrough",        BTUI_STRIKETHROUGH},
    {"fraktur",              BTUI_FRAKTUR},
    {"double_underline",     BTUI_DOUBLE_UNDERLINE},
    {"no_bold_or_faint",     BTUI_NO_BOLD_OR_FAINT},
    {"no_italic_or_fraktur", BTUI_NO_ITALIC_OR_FRAKTUR},
    {"no_underline",         BTUI_NO_UNDERLINE},
    {"no_blink",             BTUI_NO_BLINK},
    {"no_reverse",           BTUI_NO_REVERSE},
    {"no_conceal",           BTUI_NO_CONCEAL},
    {"no_strikethrough",     BTUI_NO_STRIKETHROUGH},
    {"fg_black",             BTUI_FG_BLACK},
    {"fg_red",               BTUI_FG_RED},
    {"fg_green",             BTUI_FG_GREEN},
    {"fg_yellow",            BTUI_FG_YELLOW},
    {"fg_blue",              BTUI_FG_BLUE},
    {"fg_magenta",           BTUI_FG_MAGENTA},
    {"fg_cyan",              BTUI_FG_CYAN},
    {"fg_white",             BTUI_FG_WHITE},
    {"fg_normal",            BTUI_FG_NORMAL},
    {"bg_black",             BTUI_BG_BLACK},
    {"bg_red",               BTUI_BG_RED},
    {"bg_green",             BTUI_BG_GREEN},
    {"bg_yellow",            BTUI_BG_YELLOW},
    {"bg_blue",              BTUI_BG_BLUE},
    {"bg_magenta",           BTUI_BG_MAGENTA},
    {"bg_cyan",              BTUI_BG_CYAN},
    {"bg_white",             BTUI_BG_WHITE},
    {"bg_normal",            BTUI_BG_NORMAL},
    {"framed",               BTUI_FRAMED},
    {"encircled",            BTUI_ENCIRCLED},
    {"overlined",            BTUI_OVERLINED},
    {NULL,                   0}
};

static struct {
    const char* name;
    lua_Unsigned code;
} btui_inverse_attributes[] =
{
    {"normal",               BTUI_NORMAL},
    {"bold",                 BTUI_NO_BOLD_OR_FAINT},
    {"faint",                BTUI_NO_BOLD_OR_FAINT},
    {"dim",                  BTUI_NO_BOLD_OR_FAINT},
    {"italic",               BTUI_NO_ITALIC_OR_FRAKTUR},
    {"underline",            BTUI_NO_UNDERLINE},
    {"blink_slow",           BTUI_NO_BLINK},
    {"blink_fast",           BTUI_NO_BLINK},
    {"reverse",              BTUI_NO_REVERSE},
    {"conceal",              BTUI_NO_CONCEAL},
    {"strikethrough",        BTUI_NO_STRIKETHROUGH},
    {"fraktur",              BTUI_NO_ITALIC_OR_FRAKTUR},
    {"double_underline",     BTUI_NO_UNDERLINE},
    {"fg_black",             BTUI_FG_NORMAL},
    {"fg_red",               BTUI_FG_NORMAL},
    {"fg_green",             BTUI_FG_NORMAL},
    {"fg_yellow",            BTUI_FG_NORMAL},
    {"fg_blue",              BTUI_FG_NORMAL},
    {"fg_magenta",           BTUI_FG_NORMAL},
    {"fg_cyan",              BTUI_FG_NORMAL},
    {"fg_white",             BTUI_FG_NORMAL},
    {"fg_normal",            BTUI_FG_NORMAL},
    {"bg_black",             BTUI_BG_NORMAL},
    {"bg_red",               BTUI_BG_NORMAL},
    {"bg_green",             BTUI_BG_NORMAL},
    {"bg_yellow",            BTUI_BG_NORMAL},
    {"bg_blue",              BTUI_BG_NORMAL},
    {"bg_magenta",           BTUI_BG_NORMAL},
    {"bg_cyan",              BTUI_BG_NORMAL},
    {"bg_white",             BTUI_BG_NORMAL},
    {"bg_normal",            BTUI_BG_NORMAL},
    {"framed",               BTUI_NO_FRAMED_OR_ENCIRCLED},
    {"encircled",            BTUI_NO_FRAMED_OR_ENCIRCLED},
    {"overlined",            BTUI_NO_OVERLINED},
    {NULL,                   0}
};

static const luaL_Reg Rclass_metamethods[] =
{
    {"__tostring",      Lbtui_tostring},
    {"enable",          Lbtui_enable},
    {"disable",         Lbtui_disable},
    {"withdisabled",    Lbtui_withdisabled},
    {"getkey",          Lbtui_getkey},
    {"write",           Lbtui_write},
    {"clear",           Lbtui_clear},
    {"flush",           Lbtui_flush},
    {"move",            Lbtui_move},
    {"withfg",          Lbtui_withfg},
    {"withbg",          Lbtui_withbg},
    {"withattributes",  Lbtui_withattributes},
    {"setattributes",   Lbtui_setattributes},
    {"unsetattributes", Lbtui_unsetattributes},
    {"suspend",         Lbtui_suspend},
    {"linebox",         Lbtui_linebox},
    {"fillbox",         Lbtui_fillbox},
    {"scroll",          Lbtui_scroll},
    {"shadow",          Lbtui_shadow},
    {"width",           Lbtui_width},
    {"height",          Lbtui_height},
    {NULL,              NULL}
};

LUALIB_API int luaopen_btui(lua_State *L)
{
    // Set up attributes
    lua_pushlightuserdata(L, (void*)&BTUI_ATTRIBUTES);
    lua_createtable(L, 0, 50);
    for (int i = 0; btui_attributes[i].name; i++) {
        lua_pushinteger(L, (lua_Integer)btui_attributes[i].code);
        lua_setfield(L, -2, btui_attributes[i].name);
    }
    lua_settable(L, LUA_REGISTRYINDEX);

    // Inverse attributes
    lua_pushlightuserdata(L, (void*)&BTUI_INVERSE_ATTRIBUTES);
    lua_createtable(L, 0, 50);
    for (int i = 0; btui_inverse_attributes[i].name; i++) {
        lua_pushinteger(L, (lua_Integer)btui_inverse_attributes[i].code);
        lua_setfield(L, -2, btui_inverse_attributes[i].name);
    }
    lua_settable(L, LUA_REGISTRYINDEX);

    // Set up BTUI metatable
    lua_pushlightuserdata(L, (void*)&BTUI_METATABLE);
    lua_createtable(L, 0, 16);
    luaL_register(L, NULL, Rclass_metamethods);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    lua_settable(L, LUA_REGISTRYINDEX);

    lua_pushcfunction(L, Lbtui_wrap);
    return 1;
}
