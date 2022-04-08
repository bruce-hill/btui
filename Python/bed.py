#!/usr/bin/env python3
#
# bed - the BTUI editor
#
# This is a simple example program demonstrating a minimal text editor
# implemented with the Python BTUI bindings.
#
# Usage: ./bed.py [-d|--debug] <file>
# You can move around with arrow keys, PgUp/PgDn and make basic file edits.
# Save with Ctrl-S, Quit with Ctrl-Q or Ctrl-C
#
import sys
import btui
from collections import namedtuple

Vec2 = namedtuple('Vec2', 'x y')

def clamp(x, low, high):
    if x < low: return low
    elif x > high: return high
    return x

class BedFile:
    def __init__(self, bt, lines, pos=Vec2(0,0), size=None):
        self.bt = bt
        self.lines = lines
        self.drawn = set()
        self.numlen = len(str(len(self.lines)))
        self.pos = pos
        self.size = size if size else Vec2(self.bt.width-pos.x, self.bt.height-pos.y)
        self.scroll = 0
        self._cursor = Vec2(0, 0)
        self.unsaved = False

    @property
    def cursor(self):
        return self._cursor

    @cursor.setter
    def cursor(self, p):
        x, y = p.x, p.y
        y = clamp(y, 0, len(self.lines)-1)
        x = clamp(x, 0, len(self.lines[y]))
        self._cursor = Vec2(x, y)

    def update_term_cursor(self):
        self.bt.move(self.pos.x + self.numlen + 1 + self.cursor.x,
                     self.pos.y + self.cursor.y - self.scroll)

    def set_scroll(self, newscroll):
        newscroll = clamp(newscroll, 0, len(self.lines)-1 - (self.size.y-1))
        if newscroll == self.scroll: return
        delta = newscroll - self.scroll
        self.bt.scroll(self.pos.y, self.pos.y + self.size.y - 1, delta)
        self.scroll = newscroll
        self.drawn &= set(range(self.scroll, self.scroll + (self.size.y-1)+1))
        if self.cursor.y < newscroll:
            self.cursor = Vec2(self.cursor.x, newscroll)
        elif self.cursor.y > newscroll + (self.size.y-1):
            self.cursor = Vec2(self.cursor.x, newscroll + (self.size.y-1))

    def handle_input(self, key, mx=None, my=None):
        if key == 'Space': key = ' '
        if key == 'Tab': key = '    '

        if key == 'Left':
            self.cursor = Vec2(self.cursor.x - 1, self.cursor.y)
        elif key == 'Right':
            self.cursor = Vec2(self.cursor.x + 1, self.cursor.y)
        elif key == 'Up':
            self.cursor = Vec2(self.cursor.x, self.cursor.y - 1)
            if self.cursor.y < self.scroll:
                self.set_scroll(self.cursor.y)
        elif key == 'Down':
            self.cursor = Vec2(self.cursor.x, self.cursor.y + 1)
            newscroll = clamp(self.scroll, self.cursor.y - (self.size.y-1), self.cursor.y)
            if self.cursor.y > self.scroll + (self.size.y-1):
                self.set_scroll(self.cursor.y - (self.size.y-1))
        elif key == 'Home' or key == 'Ctrl-a':
            self.cursor = Vec2(0, self.cursor.y)
        elif key == 'End' or key == 'Ctrl-e':
            self.cursor = Vec2(len(self.lines[self.cursor.y]), self.cursor.y)
        elif key == 'Page Down' or key == 'Ctrl-d':
            self.set_scroll(self.scroll + self.bt.height // 2)
        elif key == 'Page Up' or key == 'Ctrl-u':
            self.set_scroll(self.scroll - self.bt.height // 2)
        elif key == 'Delete':
            line = self.lines[self.cursor.y]
            i = self.cursor.x
            if i == len(line) and self.cursor.y < len(self.lines)-1:
                line.extend(self.lines.pop(self.cursor.y+1))
                self.drawn &= {d for d in self.drawn if d < self.cursor.y or d >= len(self.lines)+1}
                self.unsaved = True
            elif i < len(line):
                del line[i:i+1]
                self.drawn.discard(self.cursor.y)
                self.unsaved = True
        elif key == 'Backspace':
            line = self.lines[self.cursor.y]
            i = self.cursor.x
            if i == 0 and self.cursor.y > 0:
                self.lines.pop(self.cursor.y)
                prevline = self.lines[self.cursor.y - 1]
                self.cursor = Vec2(len(prevline), self.cursor.y-1)
                prevline.extend(line)
                self.drawn &= {d for d in self.drawn if d < self.cursor.y or d >= len(self.lines)+1}
                self.unsaved = True
            elif i > 0:
                del line[i-1:i]
                self.drawn.discard(self.cursor.y)
                self.cursor = Vec2(self.cursor.x - 1, self.cursor.y)
                self.unsaved = True
        elif key == 'Enter':
            line = self.lines[self.cursor.y]
            before, after = line[:self.cursor.x], line[self.cursor.x:]
            self.lines[self.cursor.y] = before
            self.lines.insert(self.cursor.y+1, after)
            self.drawn &= {d for d in self.drawn if d < self.cursor.y or d >= len(self.lines)}
            self.cursor = Vec2(0, self.cursor.y + 1)
            self.unsaved = True
        elif key == 'Left release':
            self.cursor = Vec2(mx-(self.numlen + 1), my - 1 + self.scroll)
        elif key == 'Mouse wheel down':
            self.set_scroll(self.scroll + 3)
        elif key == 'Mouse wheel up':
            self.set_scroll(self.scroll - 3)
        elif key and (len(key) == 1 or key == '    '):
            line = self.lines[self.cursor.y]
            i = self.cursor.x
            line[i:i] = bytearray(key, 'utf8')
            self.cursor = Vec2(self.cursor.x + len(key), self.cursor.y)
            self.drawn.discard(self.cursor.y)
            self.unsaved = True

    def render(self):
        self.bt.hide_cursor()
        for i in range(self.size.y):
            lineno = self.scroll + i
            if lineno in self.drawn: continue
            y = self.pos.y + i
            x = self.pos.x
            self.bt.move(x, y)
            if lineno >= len(self.lines):
                self.bt.clear(btui.ClearType.LINE)
                self.drawn.add(lineno)
                continue
            with self.bt.attributes("faint"):
                self.bt.write(("{:>"+str(self.numlen)+"}").format(lineno + 1))
                x += self.numlen + 1
            self.bt.move(x, y)
            self.bt.write_bytes(bytes(self.lines[lineno]))
            self.bt.clear(btui.ClearType.RIGHT)
            self.drawn.add(lineno)

class BED:
    def __init__(self, bt, filename):
        self.bt = bt
        self.filename = filename
        try:
            lines = [bytearray(line.rstrip(b'\n')) for line in open(filename, 'rb').readlines()]
        except FileNotFoundError:
            lines = [bytearray(b'')]
        self.file = BedFile(bt, lines, Vec2(0, 1), Vec2(bt.width, bt.height-2))
        self.drawn = set()
        self.renders = 0

    def edit(self):
        self.bt.set_cursor(btui.CursorType.BLINKING_BAR)
        self.render()
        key, mx, my = self.bt.getkey()
        while True:
            if key == 'Ctrl-c':
                sys.exit(1)
            elif key == 'Ctrl-q':
                if self.file.unsaved:
                    self.confirm_save()
                break
            elif key == 'Ctrl-s':
                self.save()
            else:
                self.file.handle_input(key, mx, my)
            self.render()
            key, mx, my = self.bt.getkey()

    def confirm_save(self):
        self.bt.move(0, self.bt.height)
        with self.bt.attributes('bold'):
            self.bt.write(f"Do you want to save {self.filename}? [y/n]")
        key, mx, my = self.bt.getkey()
        if key in ('y', 'Y'):
            self.save()
            return True
        elif key in ('n', 'N', 'Ctrl-c', 'Escape', 'Ctrl-q'):
            return False

    def save(self):
        with open(self.filename, 'wb') as f:
            for line in self.file.lines:
                f.write(line)
                f.write(b'\n')
        self.file.unsaved = False

    def render(self):
        self.renders += 1

        k = 'title-modified' if self.file.unsaved else 'title'
        if k not in self.drawn:
            self.drawn -= {'title', 'title-modified'}
            self.bt.move(0,0)
            with self.bt.attributes("bold"):
                self.bt.write(self.filename)
            if self.file.unsaved:
                with self.bt.attributes("dim"):
                    self.bt.write(" (modified)")
            self.bt.clear(btui.ClearType.RIGHT)
            self.drawn.add(k)

        self.file.render()

        # Just redraw this every time:
        with self.bt.attributes("bold"):
            self.bt.move(0, self.bt.height-1)
            self.bt.write("Ctrl-Q to quit, Ctrl-S to save")
            self.bt.clear(btui.ClearType.RIGHT)
        with self.bt.attributes("faint"):
            s = f"Line {self.file.cursor.y}, Col {self.file.cursor.x}, {self.renders} redraws"
            self.bt.move(self.bt.width-len(s), self.bt.height-1)
            self.bt.write(s)

        self.file.update_term_cursor()
        self.bt.show_cursor()


if __name__ == '__main__':
    args = sys.argv[1:]
    debug = False
    if args and args[0] in ('-d', '--debug'):
        args.pop(0)
        debug = True

    if not args:
        print("Usage: bed.py [-d|--debug] file")
        sys.exit(1)

    filename = args[0]
    with btui.open(debug=debug) as bt:
        bed = BED(bt, filename)
        bed.edit()
