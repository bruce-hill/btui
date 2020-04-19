/*
 * btui.h
 * Copyright 2020 Bruce Hill
 * Released under the MIT License
 *
 * This file defines some Text User Interface (TUI) functionality.
 */

#ifndef FILE__BTUI_H
#define FILE__BTUI_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define BTUI_VERSION 1

typedef struct {
    FILE *in, *out;
    int width, height;
    int size_changed;
} btui_t;

typedef unsigned long long attr_t;

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

static btui_t current_bt;

// Terminal escape sequences:
#define T_WRAP        "7"
#define T_SHOW_CURSOR "25"
#define T_MOUSE_XY    "1000"
#define T_MOUSE_CELL  "1002"
#define T_MOUSE_SGR   "1006"
#define T_ALT_SCREEN  "1049"
#define T_ON(opt)  "\033[?" opt "h"
#define T_OFF(opt) "\033[?" opt "l"

static const char *TUI_ENTER = T_OFF(T_SHOW_CURSOR ";" T_WRAP)  T_ON(T_ALT_SCREEN ";" T_MOUSE_XY ";" T_MOUSE_CELL ";" T_MOUSE_SGR);
static const char *TUI_LEAVE =  T_ON(T_SHOW_CURSOR ";" T_WRAP) T_OFF(T_ALT_SCREEN ";" T_MOUSE_XY ";" T_MOUSE_CELL ";" T_MOUSE_SGR) "\033[0m";

// Maximum time in milliseconds between double clicks
#ifndef DOUBLECLICK_THRESHOLD
#define DOUBLECLICK_THRESHOLD 200
#endif

// Keyboard modifiers:
#define MOD_BITSHIFT  9
#define MOD_META   (1 << (MOD_BITSHIFT + 0))
#define MOD_CTRL   (1 << (MOD_BITSHIFT + 1))
#define MOD_ALT    (1 << (MOD_BITSHIFT + 2))
#define MOD_SHIFT  (1 << (MOD_BITSHIFT + 3))

typedef enum {
    // ASCII chars:
    KEY_CTRL_AT = 0x00, KEY_CTRL_A, KEY_CTRL_B, KEY_CTRL_C, KEY_CTRL_D,
    KEY_CTRL_E, KEY_CTRL_F, KEY_CTRL_G, KEY_CTRL_H, KEY_CTRL_I, KEY_CTRL_J,
    KEY_CTRL_K, KEY_CTRL_L, KEY_CTRL_M, KEY_CTRL_N, KEY_CTRL_O, KEY_CTRL_P,
    KEY_CTRL_Q, KEY_CTRL_R, KEY_CTRL_S, KEY_CTRL_T, KEY_CTRL_U, KEY_CTRL_V,
    KEY_CTRL_W, KEY_CTRL_X, KEY_CTRL_Y, KEY_CTRL_Z,
    KEY_CTRL_LSQ_BRACKET, KEY_CTRL_BACKSLASH, KEY_CTRL_RSQ_BRACKET,
    KEY_CTRL_CARET, KEY_CTRL_UNDERSCORE, KEY_SPACE,
    // Printable chars would be here
    KEY_BACKSPACE2 = 0x7F,

    // Non-ascii multi-byte keys:
    KEY_F0, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8,
    KEY_F9, KEY_F10, KEY_F11, KEY_F12,
    KEY_INSERT, KEY_DELETE, KEY_HOME, KEY_END, KEY_PGUP, KEY_PGDN,
    KEY_ARROW_UP, KEY_ARROW_DOWN, KEY_ARROW_LEFT, KEY_ARROW_RIGHT,
    MOUSE_LEFT_PRESS, MOUSE_RIGHT_PRESS, MOUSE_MIDDLE_PRESS,
    MOUSE_LEFT_DRAG, MOUSE_RIGHT_DRAG, MOUSE_MIDDLE_DRAG,
    MOUSE_LEFT_RELEASE, MOUSE_RIGHT_RELEASE, MOUSE_MIDDLE_RELEASE,
    MOUSE_LEFT_DOUBLE, MOUSE_RIGHT_DOUBLE, MOUSE_MIDDLE_DOUBLE,
    MOUSE_WHEEL_RELEASE, MOUSE_WHEEL_PRESS,
} bkey_t;

// Overlapping key codes:
#define KEY_CTRL_BACKTICK  0x00 /* clash with ^@ */
#define KEY_CTRL_2         0x00 /* clash with ^@ */
#define KEY_BACKSPACE      0x08 /* clash with ^H */
#define KEY_TAB            0x09 /* clash with ^I */
#define KEY_ENTER          0x0D /* clash with ^M */
#define KEY_ESC            0x1B /* clash with ^[ */
#define KEY_CTRL_3         0x1B /* clash with ^[ */
#define KEY_CTRL_4         0x1C /* clash with ^\ */
#define KEY_CTRL_5         0x1D /* clash with ^] */
#define KEY_CTRL_TILDE     0x1E /* clash with ^^ */
#define KEY_CTRL_6         0x1E /* clash with ^^ */
#define KEY_CTRL_7         0x1F /* clash with ^_ */
#define KEY_CTRL_SLASH     0x1F /* clash with ^_ */
#define KEY_SPACE          0x20
#define KEY_BACKSPACE2     0x7F
#define KEY_CTRL_8         0x7F /* clash with 'BACKSPACE2' */

typedef struct {
    int key;
    const char *name;
} keyname_t;

static keyname_t key_names[] = {
    {KEY_SPACE, "Space"}, {KEY_BACKSPACE2, "Backspace"},
    {KEY_F1, "F1"}, {KEY_F2, "F2"}, {KEY_F3, "F3"}, {KEY_F4, "F4"}, {KEY_F5, "F5"},
    {KEY_F6, "F6"}, {KEY_F7, "F7"}, {KEY_F8, "F8"}, {KEY_F9, "F9"}, {KEY_F10, "F10"},
    {KEY_F11, "F11"}, {KEY_F12, "F12"},
    {KEY_INSERT, "Insert"}, {KEY_DELETE, "Delete"},
    {KEY_HOME, "Home"}, {KEY_END, "End"},
    {KEY_PGUP, "PgUp"}, {KEY_PGUP, "Page Up"},
    {KEY_PGDN, "PgDn"}, {KEY_PGDN, "Page Down"},
    {KEY_ARROW_UP, "Up"}, {KEY_ARROW_DOWN, "Down"}, {KEY_ARROW_LEFT, "Left"}, {KEY_ARROW_RIGHT, "Right"},
    {MOUSE_LEFT_PRESS, "Left press"}, {MOUSE_RIGHT_PRESS, "Right press"}, {MOUSE_MIDDLE_PRESS, "Middle press"},
    {MOUSE_LEFT_DRAG, "Left drag"}, {MOUSE_RIGHT_DRAG, "Right drag"}, {MOUSE_MIDDLE_DRAG, "Middle drag"},
    {MOUSE_LEFT_RELEASE, "Left release"}, {MOUSE_RIGHT_RELEASE, "Right release"}, {MOUSE_MIDDLE_RELEASE, "Middle release"},
    {MOUSE_LEFT_RELEASE, "Left up"}, {MOUSE_RIGHT_RELEASE, "Right up"}, {MOUSE_MIDDLE_RELEASE, "Middle up"},
    {MOUSE_LEFT_RELEASE, "Left click"}, {MOUSE_RIGHT_RELEASE, "Right click"}, {MOUSE_MIDDLE_RELEASE, "Middle click"},
    {MOUSE_LEFT_DOUBLE, "Double left click"}, {MOUSE_RIGHT_DOUBLE, "Double right click"}, {MOUSE_MIDDLE_DOUBLE, "Double middle click"},
    {MOUSE_WHEEL_RELEASE, "Mouse wheel up"}, {MOUSE_WHEEL_PRESS, "Mouse wheel down"},
    {KEY_TAB, "Tab"}, {KEY_ENTER, "Enter"}, {KEY_ENTER, "Return"},
    {KEY_CTRL_A, "Ctrl-a"}, {KEY_CTRL_B, "Ctrl-b"}, {KEY_CTRL_C, "Ctrl-c"},
    {KEY_CTRL_D, "Ctrl-d"}, {KEY_CTRL_E, "Ctrl-e"}, {KEY_CTRL_F, "Ctrl-f"},
    {KEY_CTRL_G, "Ctrl-g"}, {KEY_CTRL_H, "Ctrl-h"}, {KEY_CTRL_I, "Ctrl-i"},
    {KEY_CTRL_J, "Ctrl-j"}, {KEY_CTRL_K, "Ctrl-k"}, {KEY_CTRL_L, "Ctrl-l"},
    {KEY_CTRL_M, "Ctrl-m"}, {KEY_CTRL_N, "Ctrl-n"}, {KEY_CTRL_O, "Ctrl-o"},
    {KEY_CTRL_P, "Ctrl-p"}, {KEY_CTRL_Q, "Ctrl-q"}, {KEY_CTRL_R, "Ctrl-r"},
    {KEY_CTRL_S, "Ctrl-s"}, {KEY_CTRL_T, "Ctrl-t"}, {KEY_CTRL_U, "Ctrl-u"},
    {KEY_CTRL_V, "Ctrl-v"}, {KEY_CTRL_W, "Ctrl-w"}, {KEY_CTRL_X, "Ctrl-x"},
    {KEY_CTRL_Y, "Ctrl-y"}, {KEY_CTRL_Z, "Ctrl-z"},
    {KEY_ESC, "Esc"}, {KEY_ESC, "Escape"},
    {KEY_CTRL_TILDE, "Ctrl-~"}, {KEY_CTRL_BACKSLASH, "Ctrl-\\"},
    {KEY_CTRL_LSQ_BRACKET, "Ctrl-]"}, {KEY_CTRL_RSQ_BRACKET, "Ctrl-]"},
    {KEY_CTRL_UNDERSCORE, "Ctrl-_"}, {KEY_CTRL_SLASH, "Ctrl-/"},
    {KEY_CTRL_AT, "Ctrl-@"}, {KEY_CTRL_CARET, "Ctrl-^"},
    {KEY_CTRL_BACKTICK, "Ctrl-`"},
    {KEY_CTRL_2, "Ctrl-2"}, {KEY_CTRL_3, "Ctrl-3"}, {KEY_CTRL_4, "Ctrl-4"},
    {KEY_CTRL_5, "Ctrl-5"}, {KEY_CTRL_6, "Ctrl-6"}, {KEY_CTRL_7, "Ctrl-7"},
    {KEY_CTRL_5, "Ctrl-8"}, {KEY_CTRL_6, "Ctrl-9"},
    {':', "Colon"},
};

static const struct termios normal_termios = {
    .c_iflag = ICRNL,
    .c_oflag = OPOST | ONLCR | NL0 | CR0 | TAB0 | BS0 | VT0 | FF0,
    .c_lflag = ISIG | ICANON | IEXTEN | ECHO | ECHOE | ECHOK | ECHOCTL | ECHOKE,
    .c_cflag = CS8 | CREAD,
    .c_cc[VINTR] = '',
    .c_cc[VQUIT] = '',
    .c_cc[VERASE] = 127,
    .c_cc[VKILL] = '',
    .c_cc[VEOF] = '',
    .c_cc[VSTART] = '',
    .c_cc[VSTOP] = '',
    .c_cc[VSUSP] = '',
    .c_cc[VREPRINT] = '',
    .c_cc[VWERASE] = '',
    .c_cc[VLNEXT] = '',
    .c_cc[VDISCARD] = '',
    .c_cc[VMIN] = 1,
    .c_cc[VTIME] = 0,
};

static struct termios tui_termios = {
    .c_iflag = 0,
    .c_oflag = ONLCR | NL0 | CR0 | TAB0 | BS0 | VT0 | FF0,
    .c_lflag = ECHOE | ECHOK | ECHOCTL | ECHOKE,
    .c_cflag = CS8 | CREAD,
    .c_cc[VINTR] = '',
    .c_cc[VQUIT] = '',
    .c_cc[VERASE] = 127,
    .c_cc[VKILL] = '',
    .c_cc[VEOF] = '',
    .c_cc[VSTART] = '',
    .c_cc[VSTOP] = '',
    .c_cc[VSUSP] = '',
    .c_cc[VREPRINT] = '',
    .c_cc[VWERASE] = '',
    .c_cc[VLNEXT] = '',
    .c_cc[VDISCARD] = '',
    .c_cc[VMIN] = 1,
    .c_cc[VTIME] = 0,
};

static inline int nextchar(int fd)
{
    char c;
    return read(fd, &c, 1) == 1 ? c : -1;
}

static inline int nextnum(int fd, int c, int *n)
{
    for (*n = 0; '0' <= c && c <= '9'; c = nextchar(fd))
        *n = 10*(*n) + (c - '0');
    return c;
}

/*
 * Get one key of input from the given file. Returns -1 on failure.
 * If mouse_x or mouse_y are non-null and a mouse event occurs, they will be
 * set to the position of the mouse (0-indexed).
 */
int btui_getkey(btui_t *bt, int timeout, int *mouse_x, int *mouse_y)
{
    int new_vmin = timeout < 0 ? 1 : 0, new_vtime = timeout < 0 ? 0 : timeout;
    if (new_vmin != tui_termios.c_cc[VMIN] || new_vtime != tui_termios.c_cc[VTIME]) {
        tui_termios.c_cc[VMIN] = new_vmin;
        tui_termios.c_cc[VTIME] = new_vtime;
        if (tcsetattr(fileno(bt->out), TCSANOW, &tui_termios) == -1)
            return -1;
    }

    if (mouse_x) *mouse_x = -1;
    if (mouse_y) *mouse_y = -1;
    int fd = fileno(bt->in);
    int numcode = 0, modifiers = 0;
    int c = nextchar(fd);
    if (c == '\x1b')
        goto escape;

    return c;

  escape:
    c = nextchar(fd);
    // Actual escape key:
    if (c < 0)
        return KEY_ESC;

    switch (c) {
        case '\x1b': return KEY_ESC;
        case '[': c = nextchar(fd); goto CSI_start;
        case 'P': goto DCS;
        case 'O': goto SS3;
        default: return MOD_ALT | c;
    }

  CSI_start:
    if (c == -1)
        return MOD_ALT | '[';

    switch (c) {
        case 'A': return modifiers | KEY_ARROW_UP;
        case 'B': return modifiers | KEY_ARROW_DOWN;
        case 'C': return modifiers | KEY_ARROW_RIGHT;
        case 'D': return modifiers | KEY_ARROW_LEFT;
        case 'F': return modifiers | KEY_END;
        case 'H': return modifiers | KEY_HOME;
        case 'J': return numcode == 2 ? (MOD_SHIFT | KEY_HOME) : -1;
        case 'K': return MOD_SHIFT | KEY_END;
        case 'M': return MOD_CTRL | KEY_DELETE;
        case 'P': return modifiers | (numcode == 1 ? KEY_F1 : KEY_DELETE);
        case 'Q': return numcode == 1 ? (modifiers | KEY_F2) : -1;
        case 'R': return numcode == 1 ? (modifiers | KEY_F3) : -1;
        case 'S': return numcode == 1 ? (modifiers | KEY_F4) : -1;
        case '~':
            switch (numcode) {
                case 1: return modifiers | KEY_HOME;
                case 2: return modifiers | KEY_INSERT;
                case 3: return modifiers | KEY_DELETE;
                case 4: return modifiers | KEY_END;
                case 5: return modifiers | KEY_PGUP;
                case 6: return modifiers | KEY_PGDN;
                case 7: return modifiers | KEY_HOME;
                case 8: return modifiers | KEY_END;
                case 10: return modifiers | KEY_F0;
                case 11: return modifiers | KEY_F1;
                case 12: return modifiers | KEY_F2;
                case 13: return modifiers | KEY_F3;
                case 14: return modifiers | KEY_F4;
                case 15: return modifiers | KEY_F5;
                case 17: return modifiers | KEY_F6;
                case 18: return modifiers | KEY_F7;
                case 19: return modifiers | KEY_F8;
                case 20: return modifiers | KEY_F9;
                case 21: return modifiers | KEY_F10;
                case 23: return modifiers | KEY_F11;
                case 24: return modifiers | KEY_F12;
            }
            return -1;
        case '<': { // Mouse clicks
            int buttons = 0, x = 0, y = 0;
            c = nextnum(fd, nextchar(fd), &buttons);
            if (c != ';') return -1;
            c = nextnum(fd, nextchar(fd), &x);
            if (c != ';') return -1;
            c = nextnum(fd, nextchar(fd), &y);
            if (c != 'm' && c != 'M') return -1;

            if (mouse_x) *mouse_x = x - 1;
            if (mouse_y) *mouse_y = y - 1;

            if (buttons & 4) modifiers |= MOD_SHIFT;
            if (buttons & 8) modifiers |= MOD_META;
            if (buttons & 16) modifiers |= MOD_CTRL;
            int key = -1;
            switch (buttons & ~(4|8|16)) {
                case 0: key = c == 'm' ? MOUSE_LEFT_RELEASE : MOUSE_LEFT_PRESS; break;
                case 1: key = c == 'm' ? MOUSE_MIDDLE_RELEASE : MOUSE_MIDDLE_PRESS; break;
                case 2: key = c == 'm' ? MOUSE_RIGHT_RELEASE : MOUSE_RIGHT_PRESS; break;
                case 32: key = MOUSE_LEFT_DRAG; break;
                case 33: key = MOUSE_MIDDLE_DRAG; break;
                case 34: key = MOUSE_RIGHT_DRAG; break;
                case 64: key = MOUSE_WHEEL_RELEASE; break;
                case 65: key = MOUSE_WHEEL_PRESS; break;
                default: return -1;
            }
            if (key == MOUSE_LEFT_RELEASE || key == MOUSE_RIGHT_RELEASE || key == MOUSE_MIDDLE_RELEASE) {
                static int lastclick = -1;
                static struct timespec lastclicktime = {0, 0};
                struct timespec clicktime;
                clock_gettime(CLOCK_MONOTONIC, &clicktime);
                if (key == lastclick) {
                    double dt_ms = 1e3*(double)(clicktime.tv_sec - lastclicktime.tv_sec)
                        + 1e-6*(double)(clicktime.tv_nsec - lastclicktime.tv_nsec);
                    if (dt_ms < DOUBLECLICK_THRESHOLD) {
                        switch (key) {
                            case MOUSE_LEFT_RELEASE: key = MOUSE_LEFT_DOUBLE; break;
                            case MOUSE_RIGHT_RELEASE: key = MOUSE_RIGHT_DOUBLE; break;
                            case MOUSE_MIDDLE_RELEASE: key = MOUSE_MIDDLE_DOUBLE; break;
                        }
                    }
                }
                lastclicktime = clicktime;
                lastclick = key;
            }
            return modifiers | key;
        }
        default:
            if ('0' <= c && c <= '9') {
                // Ps prefix
                c = nextnum(fd, c, &numcode);
                if (c == ';') {
                    c = nextnum(fd, nextchar(fd), &modifiers);
                    modifiers = (modifiers >> 1) << MOD_BITSHIFT;
                }
                goto CSI_start;
            }
    }
    return -1;

  DCS:
    return -1;

  SS3:
    switch (nextchar(fd)) {
        case 'P': return KEY_F1;
        case 'Q': return KEY_F2;
        case 'R': return KEY_F3;
        case 'S': return KEY_F4;
        default: break;
    }
    return -1;
}

/*
 * Populate `buf` with the name of a key.
 */
char *btui_keyname(int key, char *buf)
{
    if (key & MOD_META) buf = stpcpy(buf, "Super-");
    if (key & MOD_CTRL) buf = stpcpy(buf, "Ctrl-");
    if (key & MOD_ALT) buf = stpcpy(buf, "Alt-");
    if (key & MOD_SHIFT) buf = stpcpy(buf, "Shift-");
    key &= ~(MOD_META | MOD_CTRL | MOD_ALT | MOD_SHIFT);
    for (size_t i = 0; i < sizeof(key_names)/sizeof(key_names[0]); i++) {
        if (key_names[i].key == key) {
            return stpcpy(buf, key_names[i].name);
        }
    }
    if (' ' < key && key <= '~')
        return buf + sprintf(buf, "%c", key);
    else
        return buf + sprintf(buf, "\\x%02X", key);
}

/*
 * Return the key with the given name, if one exists, otherwise -1.
 * (i.e. btui_keynamed("Space") == ' ', btui_keynamed("x") == 'x', btui_keynamed("F1") == KEY_F1, btui_keynamed("???") == -1)
 */
int btui_keynamed(const char *name)
{
    int modifiers = 0;
    static const struct { const char *prefix; int modifier; } modnames[] = {
        {"Super-", MOD_META}, {"Ctrl-", MOD_CTRL}, {"Alt-", MOD_ALT}, {"Shift-", MOD_SHIFT}
    };
  check_names:
    for (size_t i = 0; i < sizeof(key_names)/sizeof(key_names[0]); i++) {
        if (strcmp(key_names[i].name, name) == 0)
            return modifiers | key_names[i].key;
    }
    for (size_t i = 0; i < sizeof(modnames)/sizeof(modnames[0]); i++) {
        if (strncmp(name, modnames[i].prefix, strlen(modnames[i].prefix)) == 0) {
            modifiers |= modnames[i].modifier;
            name += strlen(modnames[i].prefix);
            goto check_names;
        }
    }
    return strlen(name) == 1 ? name[0] : -1;
}

static void cleanup(void)
{
    if (!current_bt.out) return;
    tcsetattr(fileno(current_bt.out), TCSANOW, &normal_termios);
    fputs(TUI_LEAVE, current_bt.out);
    fflush(current_bt.out);
}

static void cleanup_and_raise(int sig)
{
    cleanup();
    raise(sig);
    // This code will only ever be run if sig is SIGTSTP/SIGSTOP, otherwise, raise() won't return:
    btui_enable();
    struct sigaction sa = {.sa_handler = &cleanup_and_raise, .sa_flags = (int)(SA_NODEFER | SA_RESETHAND)};
    sigaction(sig, &sa, NULL);
}

static void update_term_size(int sig)
{
    (void)sig;
//struct winsize winsize = {0};
//int winsize_changed = 0;
    struct winsize winsize;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &winsize);
    if (winsize.ws_col != current_bt.width || winsize.ws_row != current_bt.height) {
        current_bt.width = winsize.ws_col;
        current_bt.height = winsize.ws_row;
        current_bt.size_changed = 1;
    }
}

btui_t *btui_enable(void)
{
    char *tty_name = ttyname(STDIN_FILENO);
    FILE *in = fopen(tty_name, "r");
    if (!in) return NULL;
    FILE *out = fopen(tty_name, "w");
    if (!out) return NULL;

    if (tcsetattr(fileno(out), TCSANOW, &tui_termios) == -1) {
        fclose(in);
        fclose(out);
        return NULL;
    }

    current_bt.in = in;
    current_bt.out = out;
    atexit(cleanup);

    struct sigaction sa_winch = {.sa_handler = &update_term_size};
    sigaction(SIGWINCH, &sa_winch, NULL);
    int signals[] = {SIGTERM, SIGINT, SIGXCPU, SIGXFSZ, SIGVTALRM, SIGPROF, SIGSEGV, SIGTSTP};
    struct sigaction sa = {.sa_handler = &cleanup_and_raise, .sa_flags = (int)(SA_NODEFER | SA_RESETHAND)};
    for (size_t i = 0; i < sizeof(signals)/sizeof(signals[0]); i++)
        sigaction(signals[i], &sa, NULL);

    update_term_size(SIGWINCH);
    current_bt.size_changed = 0;

    fputs(TUI_ENTER, out);
    fflush(out);
    return &current_bt;
}

void btui_disable(btui_t *bt)
{
    (void)bt;
    cleanup();
}

int btui_move_cursor(btui_t *bt, int x, int y)
{
    return fprintf(bt->out, "\033[%d;%dH", y+1, x+1);
}

int btui_set_fg_rgb(btui_t *bt, unsigned char r, unsigned char g, unsigned char b)
{
    return fprintf(bt->out, "\033[38;2;%d;%d;%dm", r, g, b);
}

int btui_set_bg_rgb(btui_t *bt, unsigned char r, unsigned char g, unsigned char b)
{
    return fprintf(bt->out, "\033[48;2;%d;%d;%dm", r, g, b);
}

int btui_set_fg_hex(btui_t *bt, int hex)
{
    return fprintf(bt->out, "\033[38;2;%d;%d;%dm",
                   (hex >> 16) & 0xFF, (hex >> 8) & 0xFF, hex & 0xFF);
}

int btui_set_bg_hex(btui_t *bt, int hex)
{
    return fprintf(bt->out, "\033[48;2;%d;%d;%dm",
                   (hex >> 16) & 0xFF, (hex >> 8) & 0xFF, hex & 0xFF);
}

const attr_t BTUI_NORMAL               = 1ul << 0;
const attr_t BTUI_BOLD                 = 1ul << 1;
const attr_t BTUI_FAINT                = 1ul << 2;
const attr_t BTUI_ITALIC               = 1ul << 3;
const attr_t BTUI_UNDERLINE            = 1ul << 4;
const attr_t BTUI_BLINK_SLOW           = 1ul << 5;
const attr_t BTUI_BLINK_FAST           = 1ul << 6;
const attr_t BTUI_REVERSE              = 1ul << 7;
const attr_t BTUI_CONCEAL              = 1ul << 8;
const attr_t BTUI_STRIKETHROUGH        = 1ul << 9;
const attr_t BTUI_FRAKTUR              = 1ul << 20;
const attr_t BTUI_DOUBLE_UNDERLINE     = 1ul << 21;
const attr_t BTUI_NO_BOLD_OR_FAINT     = 1ul << 22;
const attr_t BTUI_NO_ITALIC_OR_FRAKTUR = 1ul << 23;
const attr_t BTUI_NO_UNDERLINE         = 1ul << 24;
const attr_t BTUI_NO_BLINK             = 1ul << 25;
const attr_t BTUI_NO_REVERSE           = 1ul << 27;
const attr_t BTUI_NO_CONCEAL           = 1ul << 28;
const attr_t BTUI_NO_STRIKETHROUGH     = 1ul << 29;
const attr_t BTUI_FG_BLACK             = 1ul << 30;
const attr_t BTUI_FG_RED               = 1ul << 31;
const attr_t BTUI_FG_GREEN             = 1ul << 32;
const attr_t BTUI_FG_YELLOW            = 1ul << 33;
const attr_t BTUI_FG_BLUE              = 1ul << 34;
const attr_t BTUI_FG_MAGENTA           = 1ul << 35;
const attr_t BTUI_FG_CYAN              = 1ul << 36;
const attr_t BTUI_FG_WHITE             = 1ul << 37;
// 38: 256/24bit color
const attr_t BTUI_FG_NORMAL            = 1ul << 39;
const attr_t BTUI_BG_BLACK             = 1ul << 40;
const attr_t BTUI_BG_RED               = 1ul << 41;
const attr_t BTUI_BG_GREEN             = 1ul << 42;
const attr_t BTUI_BG_YELLOW            = 1ul << 43;
const attr_t BTUI_BG_BLUE              = 1ul << 44;
const attr_t BTUI_BG_MAGENTA           = 1ul << 45;
const attr_t BTUI_BG_CYAN              = 1ul << 46;
const attr_t BTUI_BG_WHITE             = 1ul << 47;
// 48: 256/24bit color
const attr_t BTUI_BG_NORMAL            = 1ul << 49;
const attr_t BTUI_FRAMED               = 1ul << 51;
const attr_t BTUI_ENCIRCLED            = 1ul << 52;
const attr_t BTUI_OVERLINED            = 1ul << 53;

int btui_set_attributes(btui_t *bt, attr_t attrs)
{
    int printed = fputs("\033[", bt->out);
    for (int i = 0; i < 64; i++) {
        if (attrs & (1ul << i)) {
            attrs ^= (1ul << i);
            if (attrs)
                printed += fprintf(bt->out, "%d;", i);
            else
                printed += fprintf(bt->out, "%d", i);
        }
    }
    printed += fputs("m", bt->out);
    return printed;
}

int btui_scroll(btui_t *bt, int firstline, int lastline, int scroll_amount)
{
    if (scroll_amount > 0) {
        return fprintf(bt->out, "\033[%d;%dr\033[%dS\033[r",
                       firstline, lastline, scroll_amount);
    } else if (scroll_amount < 0) {
        return fprintf(bt->out, "\033[%d;%dr\033[%dT\033[r",
                       firstline, lastline, -scroll_amount);
    }
    return 0;
}

#endif
// vim: ts=4 sw=0 et cino=L2,l1,(0,W4,m1
