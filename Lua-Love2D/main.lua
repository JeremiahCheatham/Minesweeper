require("game")
require("border")
require("face")
require("digitback")
require("remain")
require("clock")
require("board")

function love.load()
    math.randomseed(os.time())
    love.graphics.setDefaultFilter("nearest", "nearest")
    Game:load()
end

function love.update(dt)
    Game:update(dt)
end

function love.draw()
    Game:draw()
end

function love.mousereleased(x, y, button)
    Game:mousereleased(x, y, button)
end

function love.mousepressed(x, y, button)
    Game:mousepressed(x, y, button)
end

function love.keypressed(k)
    if k == "escape" then
        love.event.quit()
    else
        Game:keypressed(k)
    end
end