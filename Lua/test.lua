local btui = require("btui")

btui(function(bt)
    local key = nil
    local x, y = 1, 1
    while key ~= "q" and key ~= "Ctrl-c" do
        if key == "?" then
            bt:withdisabled(function()
                io.write("Press enter to continue.")
                io.flush()
                io.read()
            end)
        elseif key == "Ctrl-z" then
            bt:suspend()
        end

        bt:clear()

        bt:withfg(.8,.95,.2, function()
            bt:linebox(x, y, 30, 1);
            bt:move(x, y)
            bt:write("Pressed: ", key)
        end)

        bt:withattributes("bg_blue", "fg_black", function()
            local title = "Lua BTUI Demo"
            local w = #title
            local center = math.floor((bt:width() - w) / 2)
            bt:fillbox(center-2, 0, w+4, 3)
            bt:shadow(center-2, 0, w+4, 3)
            bt:move(center, 1)
            bt:write(title)
        end)

        bt:withattributes("bold", function()
            bt:move(0, bt:height()-1)
            bt:write("Press 'q' to quit")
        end)

        bt:withattributes("faint", function()
            local s = ("Size: (%dx%d)"):format(bt:width(), bt:height())
            bt:move(bt:width()-#s, bt:height()-1)
            bt:write(s)
        end)

        local mouse_x, mouse_y
        key, mouse_x, mouse_y = bt:getkey()
        if mouse_x then x, y = mouse_x, mouse_y end
    end
    if key == "Ctrl-c" then
        error("Interrupt received!")
    end
end)
