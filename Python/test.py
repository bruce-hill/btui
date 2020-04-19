from btui import open_btui

with open_btui() as bt:
    key = None
    x, y = 1, 1
    while key != 'q' and key != 'Ctrl-c':
        if key == '?':
            with bt.disabled():
                input("Press enter to continue.")
        elif key == 'Ctrl-z':
            bt.suspend()

        bt.clear()

        with bt.fg(.8, .95, .2):
            bt.outline_box(x, y, 30, 1)
            bt.move(x, y)
            bt.write(f"Pressed: {key}")

        with bt.attributes("bg_blue", "fg_black"):
            title = "Python bt Demo"
            w = len(title)
            center = (bt.width - w) // 2
            bt.fill_box(center - 2, 0, w + 4, 3)
            bt.draw_shadow(center - 2, 0, w + 4, 3)
            bt.move(center, 1)
            bt.write(title)

        with bt.attributes("bold"):
            bt.move(0, bt.height-1)
            bt.write("Press 'q' to quit")

        with bt.attributes("faint"):
            s = f"Size: ({bt.width}x{bt.height})"
            bt.move(bt.width-len(s), bt.height-1)
            bt.write(s)

        key, mx, my = bt.getkey()
        if mx:
            x, y = mx, my

    if key == 'Ctrl-c':
        raise KeyboardInterrupt
