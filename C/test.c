#include <stdio.h>
#include "btui.h"

int main(void)
{
    btui_t *bt = btui_enable();
    if (!bt) return 1;
    int done = 0;
    int x = 0, y = 0;
    while (!done) {
        const char *title = "BTUI C Demo";
        btui_set_attributes(bt, BTUI_FG_BLUE | BTUI_BOLD | BTUI_UNDERLINE);
        btui_move_cursor(bt, (bt->width - (int)strlen(title)) / 2, 0);
        btui_printf(bt, "%s", title);
        btui_set_attributes(bt, BTUI_NORMAL);

        btui_set_attributes(bt, BTUI_FG_NORMAL | BTUI_FAINT);
        btui_move_cursor(bt, 0, bt->height-1);
        btui_printf(bt, "Size = %dx%d", bt->width, bt->height);
        btui_set_attributes(bt, BTUI_NORMAL);
        btui_flush(bt);

        int mouse_x = -1, mouse_y = -1;
        int key = btui_getkey(bt, -1, &mouse_x, &mouse_y);
        btui_clear_screen(bt);
        switch (key) {
            case 'q': case KEY_CTRL_C: done = 1; break;
            case KEY_CTRL_Z:
                btui_suspend(bt);
                break;
            case -1: break;
            case KEY_ARROW_DOWN:
                btui_scroll(bt, 1, bt->height-1, +1);
                break;
            case KEY_ARROW_UP:
                btui_scroll(bt, 1, bt->height-1, -1);
                break;
            default: {
                if (mouse_x != -1) {
                    x = mouse_x;
                    y = mouse_y;
                }
                char buf[256] = {0};
                btui_keyname(key, buf);
                btui_move_cursor(bt, x, y);
                btui_set_bg_hex(bt, 0xacff40);
                btui_set_attributes(bt, BTUI_FG_BLACK);
                btui_printf(bt, " Pressed: %s ", buf);
                btui_set_attributes(bt, BTUI_NORMAL);
                fflush(bt->out);
                break;
            }
        }
    }
    btui_disable(bt);
    return 0;
}
