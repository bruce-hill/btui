local btui = require("btui")

btui(function(bt)
    local key = nil
    local x, y = 0, 0
    while key ~= "q" and key ~= "Ctrl-c" do
        bt:clear()
        bt:move(x, y)
        bt:print("Pressed: ", key)
        if key == "e" then error("ERR MESSAGE") end
        local s = ("Size: (%dx%d)"):format(bt:width(), bt:height())
        bt:move(bt:width()-#s, bt:height()-1)
        bt:print(s)

        local mouse_x, mouse_y
        key, mouse_x, mouse_y = bt:getkey()
        if mouse_x then x, y = mouse_x, mouse_y end
    end
    if key == "Ctrl-c" then
        error("Interrupt received!")
    end
end)
