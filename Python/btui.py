import ctypes
from contextlib import contextmanager

__all__ = ['open_btui']

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

attr = lambda name: ctypes.c_longlong.in_dll(libbtui, name)
attr_t = ctypes.c_longlong
BTUI_ATTRS = {
    "normal":               attr('BTUI_NORMAL'),
    "bold":                 attr('BTUI_BOLD'),
    "faint":                attr('BTUI_FAINT'),
    "dim":                  attr('BTUI_FAINT'),
    "italic":               attr('BTUI_ITALIC'),
    "underline":            attr('BTUI_UNDERLINE'),
    "blink_slow":           attr('BTUI_BLINK_SLOW'),
    "blink_fast":           attr('BTUI_BLINK_FAST'),
    "reverse":              attr('BTUI_REVERSE'),
    "conceal":              attr('BTUI_CONCEAL'),
    "strikethrough":        attr('BTUI_STRIKETHROUGH'),
    "fraktur":              attr('BTUI_FRAKTUR'),
    "double_underline":     attr('BTUI_DOUBLE_UNDERLINE'),
    "no_bold_or_faint":     attr('BTUI_NO_BOLD_OR_FAINT'),
    "no_italic_or_fraktur": attr('BTUI_NO_ITALIC_OR_FRAKTUR'),
    "no_underline":         attr('BTUI_NO_UNDERLINE'),
    "no_blink":             attr('BTUI_NO_BLINK'),
    "no_reverse":           attr('BTUI_NO_REVERSE'),
    "no_conceal":           attr('BTUI_NO_CONCEAL'),
    "no_strikethrough":     attr('BTUI_NO_STRIKETHROUGH'),
    "fg_black":             attr('BTUI_FG_BLACK'),
    "fg_red":               attr('BTUI_FG_RED'),
    "fg_green":             attr('BTUI_FG_GREEN'),
    "fg_yellow":            attr('BTUI_FG_YELLOW'),
    "fg_blue":              attr('BTUI_FG_BLUE'),
    "fg_magenta":           attr('BTUI_FG_MAGENTA'),
    "fg_cyan":              attr('BTUI_FG_CYAN'),
    "fg_white":             attr('BTUI_FG_WHITE'),
    "fg_normal":            attr('BTUI_FG_NORMAL'),
    "bg_black":             attr('BTUI_BG_BLACK'),
    "bg_red":               attr('BTUI_BG_RED'),
    "bg_green":             attr('BTUI_BG_GREEN'),
    "bg_yellow":            attr('BTUI_BG_YELLOW'),
    "bg_blue":              attr('BTUI_BG_BLUE'),
    "bg_magenta":           attr('BTUI_BG_MAGENTA'),
    "bg_cyan":              attr('BTUI_BG_CYAN'),
    "bg_white":             attr('BTUI_BG_WHITE'),
    "bg_normal":            attr('BTUI_BG_NORMAL'),
    "framed":               attr('BTUI_FRAMED'),
    "encircled":            attr('BTUI_ENCIRCLED'),
    "overlined":            attr('BTUI_OVERLINED'),
}

BTUI_INVERSE_ATTRS = {
    "normal":               attr('BTUI_NORMAL'),
    "bold":                 attr('BTUI_NO_BOLD_OR_FAINT'),
    "faint":                attr('BTUI_NO_BOLD_OR_FAINT'),
    "dim":                  attr('BTUI_NO_BOLD_OR_FAINT'),
    "italic":               attr('BTUI_NO_ITALIC_OR_FRAKTUR'),
    "underline":            attr('BTUI_NO_UNDERLINE'),
    "blink_slow":           attr('BTUI_NO_BLINK'),
    "blink_fast":           attr('BTUI_NO_BLINK'),
    "reverse":              attr('BTUI_NO_REVERSE'),
    "conceal":              attr('BTUI_NO_CONCEAL'),
    "strikethrough":        attr('BTUI_NO_STRIKETHROUGH'),
    "fraktur":              attr('BTUI_NO_ITALIC_OR_FRAKTUR'),
    "double_underline":     attr('BTUI_NO_UNDERLINE'),
    "fg_black":             attr('BTUI_FG_NORMAL'),
    "fg_red":               attr('BTUI_FG_NORMAL'),
    "fg_green":             attr('BTUI_FG_NORMAL'),
    "fg_yellow":            attr('BTUI_FG_NORMAL'),
    "fg_blue":              attr('BTUI_FG_NORMAL'),
    "fg_magenta":           attr('BTUI_FG_NORMAL'),
    "fg_cyan":              attr('BTUI_FG_NORMAL'),
    "fg_white":             attr('BTUI_FG_NORMAL'),
    "fg_normal":            attr('BTUI_FG_NORMAL'),
    "bg_black":             attr('BTUI_BG_NORMAL'),
    "bg_red":               attr('BTUI_BG_NORMAL'),
    "bg_green":             attr('BTUI_BG_NORMAL'),
    "bg_yellow":            attr('BTUI_BG_NORMAL'),
    "bg_blue":              attr('BTUI_BG_NORMAL'),
    "bg_magenta":           attr('BTUI_BG_NORMAL'),
    "bg_cyan":              attr('BTUI_BG_NORMAL'),
    "bg_white":             attr('BTUI_BG_NORMAL'),
    "bg_normal":            attr('BTUI_BG_NORMAL'),
    "framed":               attr('BTUI_NO_FRAMED_OR_ENCIRCLED'),
    "encircled":            attr('BTUI_NO_FRAMED_OR_ENCIRCLED'),
    "overlined":            attr('BTUI_NO_OVERLINED'),
}


class BTUI:
    @contextmanager
    def attributes(self, *attrs):
        self.set_attributes(*attrs)
        try: yield
        finally: self.unset_attributes(*attrs)

    @contextmanager
    def bg(self, r, g, b):
        self.set_bg(r, g, b)
        try: yield
        finally: self.set_attributes("bg_normal")

    def clear(self, mode='screen'):
        assert self._btui
        if mode not in ('screen', 'above', 'below', 'line', 'left', 'right'):
            raise ArgumentError("Not a supported clear type: "+repr(mode))
        clr = ctypes.c_uint.in_dll(libbtui, 'BTUI_CLEAR_' + mode.upper())
        libbtui.btui_clear(self._btui, clr)

    def disable(self):
        libbtui.btui_disable(self._btui)

    @contextmanager
    def disabled(self):
        self.disable()
        try: yield self
        finally: self.enable()

    def draw_shadow(self, x, y, w, h):
        assert self._btui
        libbtui.btui_draw_shadow(self._btui, int(x), int(y), int(w), int(h))

    def enable(self):
        self._btui = libbtui.btui_enable()

    @contextmanager
    def fg(self, r, g, b):
        self.set_fg(r, g, b)
        try: yield
        finally: self.set_attributes("fg_normal")

    def fill_box(self, x, y, w, h):
        assert self._btui
        libbtui.btui_fill_box(self._btui, int(x), int(y), int(w), int(h))

    def flush(self):
        assert self._btui
        libbtui.btui_flush(self._btui)

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
    def height(self):
        assert self._btui
        return self._btui.contents.height

    def move(self, x, y):
        assert self._btui
        libbtui.btui_move_cursor(self._btui, int(x), int(y))

    def hide_cursor(self):
        assert self._btui
        libbtui.btui_hide_cursor(self._btui)

    def show_cursor(self):
        assert self._btui
        libbtui.btui_show_cursor(self._btui)

    def outline_box(self, x, y, w, h):
        assert self._btui
        libbtui.btui_draw_linebox(self._btui, int(x), int(y), int(w), int(h))

    def scroll(self, firstline, lastline=None, amount=None):
        assert self._btui
        if amount is None:
            amount = firstline
            firstline, lastline = 1, self.height
        libbtui.btui_scroll(self._btui, firstline, lastline, amount)

    def set_attributes(self, *attrs):
        assert self._btui
        attr_long = ctypes.c_longlong(0)
        for a in attrs:
            attr_long.value |= BTUI_ATTRS[a].value
        libbtui.btui_set_attributes(self._btui, attr_long)

    def set_bg(self, r, g, b):
        assert self._btui
        libbtui.btui_set_bg(self._btui, int(r*255), int(g*255), int(b*255))

    def set_fg(self, r, g, b):
        assert self._btui
        libbtui.btui_set_fg(self._btui, int(r*255), int(g*255), int(b*255))

    def suspend(self):
        assert self._btui
        libbtui.btui_suspend(self._btui)

    def unset_attributes(self, *attrs):
        assert self._btui
        attr_long = ctypes.c_longlong(0)
        for a in attrs:
            attr_long.value |= BTUI_INVERSE_ATTRS[a].value
        libbtui.btui_set_attributes(self._btui, attr_long)

    @property
    def width(self):
        assert self._btui
        return self._btui.contents.width

    def write(self, *args, sep=''):
        assert self._btui
        s = sep.join(args)
        self.write_bytes(bytes(s, 'utf8'))

    def write_bytes(self, b):
        assert self._btui
        libbtui.btui_puts(self._btui, b)


_btui = BTUI()
@contextmanager
def open_btui():
    _btui.enable()
    try: yield _btui
    finally: _btui.disable()

