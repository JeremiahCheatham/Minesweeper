Clock = {}

function Clock:new(image, width, height)
    local newClock = {}
    LoadSpriteSheet(newClock, image, width, height)
    setmetatable(newClock, { __index = self })
    return newClock
end

function Clock:set_theme(theme)
    self._theme = theme
end

function Clock:reset(scale, left, top)
    self._scale = scale
    self._width = self._originalWidth * self._scale
    self._height = self._originalHeight * self._scale
    self._lastTime = 0
    self._currentTime = 0
    self._first = 12
    self._second = 12
    self._third = 1
    self._top = top + ( 2 * self._scale )
    self._firstx = left + ( 1 * self._scale )
    self._secondx = self._firstx + self._width
    self._thirdx =  self._secondx + self._width
end

function Clock:update(dt)
    if self._lastTime < 999 then
        self._currentTime = self._currentTime + dt
        if self._currentTime > self._lastTime + 1 then
            self._lastTime = self._lastTime + 1
            if self._lastTime > 99 then
                self._first = ( math.floor(self._lastTime / 100 % 10) ) + 1
            else
                self._first = 12
            end
            if self._lastTime > 9 then
                self._second = ( math.floor(self._lastTime / 10 % 10) ) + 1
            else
                self._second = 12
            end
            self._third = ( math.floor(self._lastTime % 10) ) + 1
        end
    end
end

function Clock:draw()
    love.graphics.draw(self._image, self._quads[self._first + self._theme], self._firstx, self._top, 0, self._scale)
    love.graphics.draw(self._image, self._quads[self._second + self._theme], self._secondx, self._top, 0, self._scale)
    love.graphics.draw(self._image, self._quads[self._third + self._theme], self._thirdx, self._top, 0, self._scale)
end

