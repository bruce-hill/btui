import ctypes
from contextlib import contextmanager

__all__ = ['btui']

# Load the shared library into c types.
libbtui = ctypes.CDLL("./libbtui.so")

class FILE(ctypes.Structure):
    pass

class BTUI_struct(ctypes.Structure):
    _fields_ = [
        ('in', ctypes.POINTER(FILE)),
        ('out', ctypes.POINTER(FILE)),
        ('width', ctypes.c_int),
        ('height', ctypes.c_int),
        ('size_changed', ctypes.c_int),
    ]

libbtui.btui_enable.restype = ctypes.POINTER(BTUI_struct)

attr_t = ctypes.c_longlong
BTUI_ATTRS = {
    "normal":               attr_t.in_dll(libbtui, 'BTUI_NORMAL'),
    "bold":                 attr_t.in_dll(libbtui, 'BTUI_BOLD'),
    "faint":                attr_t.in_dll(libbtui, 'BTUI_FAINT'),
    "dim":                  attr_t.in_dll(libbtui, 'BTUI_FAINT'),
    "italic":               attr_t.in_dll(libbtui, 'BTUI_ITALIC'),
    "underline":            attr_t.in_dll(libbtui, 'BTUI_UNDERLINE'),
    "blink_slow":           attr_t.in_dll(libbtui, 'BTUI_BLINK_SLOW'),
    "blink_fast":           attr_t.in_dll(libbtui, 'BTUI_BLINK_FAST'),
    "reverse":              attr_t.in_dll(libbtui, 'BTUI_REVERSE'),
    "conceal":              attr_t.in_dll(libbtui, 'BTUI_CONCEAL'),
    "strikethrough":        attr_t.in_dll(libbtui, 'BTUI_STRIKETHROUGH'),
    "fraktur":              attr_t.in_dll(libbtui, 'BTUI_FRAKTUR'),
    "double_underline":     attr_t.in_dll(libbtui, 'BTUI_DOUBLE_UNDERLINE'),
    "no_bold_or_faint":     attr_t.in_dll(libbtui, 'BTUI_NO_BOLD_OR_FAINT'),
    "no_italic_or_fraktur": attr_t.in_dll(libbtui, 'BTUI_NO_ITALIC_OR_FRAKTUR'),
    "no_underline":         attr_t.in_dll(libbtui, 'BTUI_NO_UNDERLINE'),
    "no_blink":             attr_t.in_dll(libbtui, 'BTUI_NO_BLINK'),
    "no_reverse":           attr_t.in_dll(libbtui, 'BTUI_NO_REVERSE'),
    "no_conceal":           attr_t.in_dll(libbtui, 'BTUI_NO_CONCEAL'),
    "no_strikethrough":     attr_t.in_dll(libbtui, 'BTUI_NO_STRIKETHROUGH'),
    "fg_black":             attr_t.in_dll(libbtui, 'BTUI_FG_BLACK'),
    "fg_red":               attr_t.in_dll(libbtui, 'BTUI_FG_RED'),
    "fg_green":             attr_t.in_dll(libbtui, 'BTUI_FG_GREEN'),
    "fg_yellow":            attr_t.in_dll(libbtui, 'BTUI_FG_YELLOW'),
    "fg_blue":              attr_t.in_dll(libbtui, 'BTUI_FG_BLUE'),
    "fg_magenta":           attr_t.in_dll(libbtui, 'BTUI_FG_MAGENTA'),
    "fg_cyan":              attr_t.in_dll(libbtui, 'BTUI_FG_CYAN'),
    "fg_white":             attr_t.in_dll(libbtui, 'BTUI_FG_WHITE'),
    "fg_normal":            attr_t.in_dll(libbtui, 'BTUI_FG_NORMAL'),
    "bg_black":             attr_t.in_dll(libbtui, 'BTUI_BG_BLACK'),
    "bg_red":               attr_t.in_dll(libbtui, 'BTUI_BG_RED'),
    "bg_green":             attr_t.in_dll(libbtui, 'BTUI_BG_GREEN'),
    "bg_yellow":            attr_t.in_dll(libbtui, 'BTUI_BG_YELLOW'),
    "bg_blue":              attr_t.in_dll(libbtui, 'BTUI_BG_BLUE'),
    "bg_magenta":           attr_t.in_dll(libbtui, 'BTUI_BG_MAGENTA'),
    "bg_cyan":              attr_t.in_dll(libbtui, 'BTUI_BG_CYAN'),
    "bg_white":             attr_t.in_dll(libbtui, 'BTUI_BG_WHITE'),
    "bg_normal":            attr_t.in_dll(libbtui, 'BTUI_BG_NORMAL'),
    "framed":               attr_t.in_dll(libbtui, 'BTUI_FRAMED'),
    "encircled":            attr_t.in_dll(libbtui, 'BTUI_ENCIRCLED'),
    "overlined":            attr_t.in_dll(libbtui, 'BTUI_OVERLINED'),
}

BTUI_INVERSE_ATTRS = {
    "normal":               attr_t.in_dll(libbtui, 'BTUI_NORMAL'),
    "bold":                 attr_t.in_dll(libbtui, 'BTUI_NO_BOLD_OR_FAINT'),
    "faint":                attr_t.in_dll(libbtui, 'BTUI_NO_BOLD_OR_FAINT'),
    "dim":                  attr_t.in_dll(libbtui, 'BTUI_NO_BOLD_OR_FAINT'),
    "italic":               attr_t.in_dll(libbtui, 'BTUI_NO_ITALIC_OR_FRAKTUR'),
    "underline":            attr_t.in_dll(libbtui, 'BTUI_NO_UNDERLINE'),
    "blink_slow":           attr_t.in_dll(libbtui, 'BTUI_NO_BLINK'),
    "blink_fast":           attr_t.in_dll(libbtui, 'BTUI_NO_BLINK'),
    "reverse":              attr_t.in_dll(libbtui, 'BTUI_NO_REVERSE'),
    "conceal":              attr_t.in_dll(libbtui, 'BTUI_NO_CONCEAL'),
    "strikethrough":        attr_t.in_dll(libbtui, 'BTUI_NO_STRIKETHROUGH'),
    "fraktur":              attr_t.in_dll(libbtui, 'BTUI_NO_ITALIC_OR_FRAKTUR'),
    "double_underline":     attr_t.in_dll(libbtui, 'BTUI_NO_UNDERLINE'),
    "fg_black":             attr_t.in_dll(libbtui, 'BTUI_FG_NORMAL'),
    "fg_red":               attr_t.in_dll(libbtui, 'BTUI_FG_NORMAL'),
    "fg_green":             attr_t.in_dll(libbtui, 'BTUI_FG_NORMAL'),
    "fg_yellow":            attr_t.in_dll(libbtui, 'BTUI_FG_NORMAL'),
    "fg_blue":              attr_t.in_dll(libbtui, 'BTUI_FG_NORMAL'),
    "fg_magenta":           attr_t.in_dll(libbtui, 'BTUI_FG_NORMAL'),
    "fg_cyan":              attr_t.in_dll(libbtui, 'BTUI_FG_NORMAL'),
    "fg_white":             attr_t.in_dll(libbtui, 'BTUI_FG_NORMAL'),
    "fg_normal":            attr_t.in_dll(libbtui, 'BTUI_FG_NORMAL'),
    "bg_black":             attr_t.in_dll(libbtui, 'BTUI_BG_NORMAL'),
    "bg_red":               attr_t.in_dll(libbtui, 'BTUI_BG_NORMAL'),
    "bg_green":             attr_t.in_dll(libbtui, 'BTUI_BG_NORMAL'),
    "bg_yellow":            attr_t.in_dll(libbtui, 'BTUI_BG_NORMAL'),
    "bg_blue":              attr_t.in_dll(libbtui, 'BTUI_BG_NORMAL'),
    "bg_magenta":           attr_t.in_dll(libbtui, 'BTUI_BG_NORMAL'),
    "bg_cyan":              attr_t.in_dll(libbtui, 'BTUI_BG_NORMAL'),
    "bg_white":             attr_t.in_dll(libbtui, 'BTUI_BG_NORMAL'),
    "bg_normal":            attr_t.in_dll(libbtui, 'BTUI_BG_NORMAL'),
};


class BTUI:
    def __enter__(self):
        self.enable()

    def __exit__(self, *exc):
        self.disable()

    def enable(self):
        self._btui = libbtui.btui_enable()

    def disable(self):
        libbtui.btui_disable(self._btui)

    @contextmanager
    def disabled(self):
        self.disable()
        try: yield self
        finally: self.enable()

    def getkey(self, timeout=None):
        assert self._btui
        timeout = -1 if timeout is None else int(timeout)
        mouse_x, mouse_y = ctypes.c_int(-1), ctypes.c_int(-1)
        key = libbtui.btui_getkey(self._btui, timeout,
                ctypes.byref(mouse_x), ctypes.byref(mouse_y))
        buf = ctypes.create_string_buffer(64)
        libbtui.btui_keyname(key, buf)
        if mouse_x.value == -1:
            return buf.value.decode('utf8'), None, None
        else:
            return buf.value.decode('utf8'), mouse_x.value, mouse_y.value

    @property
    def width(self):
        assert self._btui
        return self._btui.contents.width

    @property
    def height(self):
        assert self._btui
        return self._btui.contents.height

    def write(self, *args, sep=''):
        assert self._btui
        s = sep.join(args)
        self.write_bytes(bytes(s, 'utf8'))

    def write_bytes(self, b):
        assert self._btui
        libbtui.btui_puts(self._btui, b)

    def move(self, x, y):
        assert self._btui
        libbtui.btui_move_cursor(self._btui, int(x), int(y))

    def flush(self):
        assert self._btui
        libbtui.btui_flush(self._btui)

    def suspend(self):
        assert self._btui
        libbtui.btui_suspend(self._btui)

    def clear(self, mode='screen'):
        if mode == 'below':
            self.write_bytes(b'\033[J')
        elif mode == 'above':
            self.write_bytes(b'\033[1J')
        elif mode == 'screen':
            self.write_bytes(b'\033[2J')
        elif mode == 'right':
            self.write_bytes(b'\033[K')
        elif mode == 'left':
            self.write_bytes(b'\033[1K')
        elif mode == 'line':
            self.write_bytes(b'\033[2K')
        libbtui.btui_flush(self._btui)

    def set_attributes(self, *attrs):
        assert self._btui
        attr_long = ctypes.c_longlong(0)
        for a in attrs:
            attr_long.value |= BTUI_ATTRS[a].value
        libbtui.btui_set_attributes(self._btui, attr_long)

    def unset_attributes(self, *attrs):
        assert self._btui
        attr_long = ctypes.c_longlong(0)
        for a in attrs:
            attr_long.value |= BTUI_INVERSE_ATTRS[a].value
        libbtui.btui_set_attributes(self._btui, attr_long)

    @contextmanager
    def attributes(self, *attrs):
        self.set_attributes(*attrs)
        try: yield
        finally: self.unset_attributes(*attrs)

    def set_fg(self, r, g, b):
        assert self._btui
        libbtui.btui_set_fg(self._btui, int(r*255), int(g*255), int(b*255))

    def set_bg(self, r, g, b):
        assert self._btui
        libbtui.btui_set_bg(self._btui, int(r*255), int(g*255), int(b*255))

    @contextmanager
    def fg(self, r, g, b):
        self.set_fg(r, g, b)
        try: yield
        finally: self.set_attributes("fg_normal")

    @contextmanager
    def bg(self, r, g, b):
        self.set_bg(r, g, b)
        try: yield
        finally: self.set_attributes("bg_normal")

    def outline_box(self, x, y, w, h):
        assert self._btui
        libbtui.btui_draw_linebox(self._btui, int(x), int(y), int(w), int(h))

    def fill_box(self, x, y, w, h):
        assert self._btui
        libbtui.btui_fill_box(self._btui, int(x), int(y), int(w), int(h))

    def draw_shadow(self, x, y, w, h):
        assert self._btui
        libbtui.btui_draw_shadow(self._btui, int(x), int(y), int(w), int(h))


btui = BTUI()
