/*
 * This file contains the source of a program used to demonstrate BTUI's 24-bit
 * color support by printing a rapidfire rainbow.
 */
#include <stdio.h>
#include <math.h>
#include "btui.h"

int main(void)
{
    btui_t *bt = btui_enable();
    if (!bt) return 1;
    int done = 0;
    double t = 0;
    double a = 1.13, b = 1.23, c = 1.37;
    double dt = 0.1;
    char buf[1<<20];
    setvbuf(bt->out, buf, _IOFBF, sizeof(buf));
    while (!done) {
        btui_scroll(bt, 1, bt->height, +1);
        const char *msg = " 24 BIT COLOR SUPPORT! ";
        btui_move_cursor(bt, (bt->width - (int)strlen(msg)) / 2, 0);
        btui_set_attributes(bt, BTUI_NORMAL | BTUI_BOLD);
        btui_puts(bt, msg);
        int y = bt->height-1;
        btui_move_cursor(bt, 0, y);
        for (int x = 0; x < bt->width; x++) {
            int r = (int)(255.0 * (0.5 + 0.5*sin(t*a + (double)(x) / 50.0)));
            int g = (int)(255.0 * (0.5 + 0.5*sin(0.8 + t*b + (double)(x) / 50.0)));
            int b = (int)(255.0 * (0.5 + 0.5*sin(1.3 + t*c + (double)(x) / 50.0)));
            btui_set_bg(bt,
                        (r < 0 ? 0 : (r > 255 ? 255 : r)),
                        (g < 0 ? 0 : (g > 255 ? 255 : g)),
                        (b < 0 ? 0 : (b > 255 ? 255 : b)));
            btui_puts(bt, " ");
        }
        btui_flush(bt);
        usleep(10000);
        t += dt;

        int mouse_x = -1, mouse_y = -1;
        int key = btui_getkey(bt, 0, &mouse_x, &mouse_y);
        switch (key) {
            case 'q': case KEY_CTRL_C: done = 1; break;
        }
    }
    btui_disable(bt);
    return 0;
}
