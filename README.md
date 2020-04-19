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
    int btui_getkey(btui_t *bt, int *mouse_x, int *mouse_y);
    int btui_move_cursor(btui_t *bt, int x, int y);
    char *btui_keyname(int key, char *buf);
    int btui_keynamed(const char *name);
    int btui_set_attributes(btui_t *bt, unsigned long attrs);
    int btui_set_fg_rgb(btui_t *bt, unsigned char r, unsigned char g, unsigned char b);
    int btui_set_bg_rgb(btui_t *bt, unsigned char r, unsigned char g, unsigned char b);
    int btui_set_fg_hex(btui_t *bt, int hex);
    int btui_set_bg_hex(btui_t *bt, int hex);
    #define btui_printf(bt, ...) fprintf((bt)->out, __VA_ARGS__)
    #define btui_puts(bt, s) fputs(s, (bt)->out)
    #define btui_flush(bt) fflush((bt)->out)
    #define btui_clear(bt) fputs("\033[2J", (bt)->out)
    #define btui_clear_below(bt) fputs("\033[J", (bt)->out)
    #define btui_clear_above(bt) fputs("\033[1J", (bt)->out)
    #define btui_clear_eol(bt) fputs("\033[K", (bt)->out)
    #define btui_clear_line(bt) fputs("\033[2K", (bt)->out)

### Lua API

The Lua library returns a function that takes one argument: a function that will
be called with a `BTUI` object, which can be used to do TUI actions. Errors will
be propagated out of the function, but the terminal will be cleaned up nicely
before the error is printed. Here's a simple example program:

    local btui = require("btui")

    btui(function(bt)
        local key = nil
        local x, y = 0, 0
        while key ~= "q" and key ~= "Ctrl-c" do
            bt:clear()
            bt:move(x, y)
            bt:print("Pressed: ", key)
            if key == "e" then error("ERR MESSAGE") end
            local s = ("Size: (%dx%d)"):format(bt:width(), bt:height())
            bt:move(bt:width()-#s, bt:height()-1)
            bt:print(s)

            local mouse_x, mouse_y
            key, mouse_x, mouse_y = bt:getkey()
            if mouse_x then x, y = mouse_x, mouse_y end
        end
        if key == "Ctrl-c" then
            error("Interrupt received!")
        end
    end)
