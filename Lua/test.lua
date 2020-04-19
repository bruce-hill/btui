local btui = require("btui")

btui(function(bt)
    local key = nil
    local x, y = 0, 0
    while key ~= "q" and key ~= "Ctrl-c" do
        bt:clear()
        bt:move(x, y)
        bt:withbg(150,225,80, function()
            bt:withfg(0,0,0, function()
                bt:print(" Pressed: ", key, " ")
            end)
        end)
        if key == "e" then error("ERR MESSAGE") end

        local title = "Lua BTUI Demo"
        bt:withattributes("bold", "underline", function()
            bt:withfg(100,200,255, function()
                bt:move(math.floor((bt:width()-#title)/2), 0)
                bt:print(title)
            end)
        end)

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
