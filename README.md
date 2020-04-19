# BTUI - Bruce's Text User Interface Library

BTUI is a minimal, embeddable single header file alternative to bloatware like
ncurses. BTUI aims to be under 1/1,000th the size of ncurses, while also
providing a more usable and modern API.

## Language Bindings

BTUI comes with bindings for C and Lua, with plans to add Python bindings.

### C API

BTUI has the following C function definitions, as well as definitions for some
constants, including terminal escape values and keycodes.

    btui_t* btui_enable(void);
    void btui_disable(btui_t *bt);
    int btui_getkey(btui_t *bt, int timeout, int *mouse_x, int *mouse_y);
    int btui_move_cursor(btui_t *bt, int x, int y);
    char *btui_keyname(int key, char *buf);
    int btui_keynamed(const char *name);
    int btui_set_attributes(btui_t *bt, attr_t attrs);
    int btui_set_fg_rgb(btui_t *bt, unsigned char r, unsigned char g, unsigned char b);
    int btui_set_bg_rgb(btui_t *bt, unsigned char r, unsigned char g, unsigned char b);
    int btui_set_fg_hex(btui_t *bt, int hex);
    int btui_set_bg_hex(btui_t *bt, int hex);
    #define btui_printf(bt, ...) fprintf((bt)->out, __VA_ARGS__)
    #define btui_puts(bt, s) fputs(s, (bt)->out)
    #define btui_flush(bt) fflush((bt)->out)
    #define btui_clear_below(bt) fputs("\033[J", (bt)->out)
    #define btui_clear_above(bt) fputs("\033[1J", (bt)->out)
    #define btui_clear_screen(bt) fputs("\033[2J", (bt)->out)
    #define btui_clear_right(bt) fputs("\033[K", (bt)->out)
    #define btui_clear_left(bt) fputs("\033[1K", (bt)->out)
    #define btui_clear_line(bt) fputs("\033[2K", (bt)->out)

See [C/test.c](C/test.c) for example usage.

### Lua API

The Lua library returns a function that takes one argument: a function that will
be called with a `BTUI` object, which can be used to do TUI actions. Errors will
be propagated out of the function, but the terminal will be cleaned up nicely
before the error is printed. Here's a simple example program:

    require("btui")(function(bt)
        ...
    end)

    bt:enable() -- Enables btui (if previously disabled)
    bt:disable() -- Disables btui
    bt:withdisabled(fn) -- Calls "fn" with btui disabled, then re-enables btui
    bt:getkey(timeout=-1) -- Returns a keypress (and optionally, mouse x and y coordinates). The optional timeout argument specifies how long, in tenths of a second, to wait for the next keypress.
    bt:print() -- Print text to the terminal
    bt:clear(type="screen") -- Clear the terminal. Options are: "screen", "right", "left", "above", "below", "line"
    bt:flush() -- Flush the terminal output. Most operations do this anyways.
    bt:move(x, y) -- Move the cursor to the given position. (0,0) is the top left corner.
    bt:withfg(r,g,b, fn) -- Set the foreground color to (r,g,b), call fn, then reset the foreground color to default
    bt:withbg(r,g,b, fn) -- Set the background color to (r,g,b), call fn, then reset the background color to default
    bt:withattributes(attrs..., fn) -- Set the given attributes, call fn, then unset them
    bt:setattributes(attrs...) -- Set the given attributes
    bt:unsetattributes(attrs...) -- Unset the given attributes
    bt:width() -- Return the scren width
    bt:height() -- Return the screen height

See [Lua/test.lua](Lua/test.lua) for example usage.

### Python API

Coming soon.
