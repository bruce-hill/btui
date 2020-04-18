#include <stdio.h>
#include "btui.h"

int main(void)
{
    FILE *tty_in = fopen("/dev/tty", "r");
    FILE *tty_out = fopen("/dev/tty", "w");
    btui_enable(tty_out);
    int done = 0;
    int x = 0, y = 0;
    int i = 0;
    while (!done) {
        move_cursor(tty_out, 0, winsize.ws_row-1);
        fprintf(tty_out, "Update %d, size = %dx%d", i++, winsize.ws_col, winsize.ws_row);
        fflush(tty_out);

        int key = btui_getkey(tty_in, NULL, NULL);
        switch (key) {
            case 'q': case KEY_CTRL_C: done = 1; break;
            case -1: break;
            default: {
                char buf[256] = {0};
                btui_keyname(key, buf);
                move_cursor(tty_out, x, y++);
                fprintf(tty_out, "Pressed: %s", buf);
                fflush(tty_out);
                break;
            }
        }
    }
    btui_disable(tty_out);
    return 0;
}
