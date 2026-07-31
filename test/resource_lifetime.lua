local picoturtle = require "picoturtle"

for index = 1, 20 do
    local turtle = picoturtle.new()
    turtle:penup()
    turtle:setpos(index, -index)
    turtle:font("Noto Sans", 12 + index)

    local state = turtle:state()
    assert(state:x() == index)
    assert(state:y() == -index)

    local image = turtle:loadpic("res/images/mandrill.png")
    assert(string.find(tostring(image), "mandrill.png", 1, true))

    turtle = nil
    state = nil
    image = nil
    collectgarbage("collect")
end

collectgarbage("collect")
