#include <stdio.h>
#include "btui.h"

int main(void)
{
    btui_t *bt = btui_enable();
    if (!bt) return 1;
    int done = 0;
    int x = 1, y = 1;
    while (!done) {
        const char *title = "BTUI C Demo";
        int w = (int)strlen(title);
        int center = (bt->width - w) / 2;

        btui_set_attributes(bt, BTUI_FG_BLUE | BTUI_BG_NORMAL);
        btui_draw_shadow(bt, center-2, 0, w+4, 3);

        btui_set_attributes(bt, BTUI_BG_BLUE | BTUI_FG_BLACK);
        btui_fill_box(bt, center-2, 0, w+4, 3);

        btui_move_cursor(bt, center, 1);
        btui_printf(bt, "%s", title);
        btui_set_attributes(bt, BTUI_NORMAL);

        btui_set_attributes(bt, BTUI_BOLD);
        btui_move_cursor(bt, 0, bt->height-2);
        btui_printf(bt, "Press 'q' to quit");
        btui_set_attributes(bt, BTUI_NORMAL);

        btui_set_attributes(bt, BTUI_FAINT);
        btui_move_cursor(bt, bt->width-16, bt->height-2);
        btui_printf(bt, "Size = %dx%d", bt->width, bt->height);
        btui_set_attributes(bt, BTUI_NORMAL);

        btui_flush(bt);

        int mouse_x = -1, mouse_y = -1;
        int key = btui_getkey(bt, -1, &mouse_x, &mouse_y);
        btui_clear(bt, BTUI_CLEAR_SCREEN);
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
                    x = mouse_x - 15;
                    y = mouse_y;
                }
                char buf[256] = {0};
                btui_keyname(key, buf);

                btui_set_fg_hex(bt, 0xacff40);
                btui_draw_linebox(bt, x, y, 30, 1);

                btui_move_cursor(bt, x, y);
                btui_set_fg_hex(bt, 0xacff40);
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
