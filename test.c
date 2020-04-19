#include <stdio.h>
#include "btui.h"

int main(void)
{
    btui_t *bt = btui_enable();
    if (!bt) return 1;
    int done = 0;
    int x = 0, y = 0;
    int i = 0;
    while (!done) {
        btui_move_cursor(bt, 0, bt->height-1);
        btui_printf(bt, "Update %d, size = %dx%d", i++, bt->width, bt->height);
        btui_flush(bt);

        int key = btui_getkey(bt, NULL, NULL);
        switch (key) {
            case 'q': case KEY_CTRL_C: done = 1; break;
            case -1: break;
            case KEY_ARROW_DOWN:
                btui_scroll(bt, 1, bt->height-1, +1);
                break;
            case KEY_ARROW_UP:
                btui_scroll(bt, 1, bt->height-1, -1);
                break;
            default: {
                char buf[256] = {0};
                btui_keyname(key, buf);
                btui_move_cursor(bt, x, y++);
                //btui_set_attributes(bt, BTUI_FG_YELLOW | BTUI_BOLD);
                btui_set_fg_hex(bt, 0xacff40);
                btui_printf(bt, "Pressed: %s", buf);
                btui_set_attributes(bt, BTUI_NORMAL);
                fflush(bt->out);
                break;
            }
        }
    }
    btui_disable(bt);
    return 0;
}
