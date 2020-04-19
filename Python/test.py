from btui import btui

with btui:
    key = None
    x, y = 1, 1
    while key != 'q' and key != 'Ctrl-c':
        if key == '?':
            with btui.disabled():
                input("Press enter to continue.")
        elif key == 'Ctrl-z':
            btui.suspend()

        btui.clear()

        with btui.fg(.8, .95, .2):
            btui.outline_box(x, y, 30, 1)
            btui.move(x, y)
            btui.write(f"Pressed: {key}")

        with btui.attributes("bg_blue", "fg_black"):
            title = "Python BTUI Demo"
            w = len(title)
            center = (btui.width - w) // 2
            btui.fill_box(center - 2, 0, w + 4, 3)
            btui.draw_shadow(center - 2, 0, w + 4, 3)
            btui.move(center, 1)
            btui.write(title)

        with btui.attributes("faint"):
            s = f"Size: ({btui.width}x{btui.height})"
            btui.move(btui.width-len(s), btui.height-1)
            btui.write(s)

        key, mx, my = btui.getkey()
        if mx:
            x, y = mx, my

    if key == 'Ctrl-c':
        raise KeyboardInterrupt
