Remain = {}

function Remain:new(image, width, height)
    local newRemain = {}
    LoadSpriteSheet(newRemain, image, width, height)
    setmetatable(newRemain, { __index = self })
    return newRemain
end

function Remain:set_theme(theme)
    self._theme = theme
end

function Remain:reset(scale, left, top, remaining)
    self._scale = scale
    self._width = self._originalWidth * scale
    self._height = self._originalHeight * scale
    self._first = 12
    self._second = 12
    self._third = 1
    self._top = top + ( 2 * scale )
    self._firstx = left + ( 1 * scale )
    self._secondx = self._firstx + self._width
    self._thirdx =  self._secondx + self._width
    self:update(remaining)
end

function Remain:update(remaining)
    if remaining < 0 then
        remaining = math.abs(remaining)
        if remaining > 99 then
            remaining = 99
        end
        self._first = 11
        if remaining > 9 then
            self._second = ( math.floor(remaining / 10 % 10) ) + 1
        else
            self._second = 12
        end
        self._third = ( math.floor(remaining % 10) ) + 1
    else
        if remaining > 999 then
            remaining = 999
        end
        if remaining > 99 then
            self._first = ( math.floor(remaining / 100 % 10) ) + 1
        else
            self._first = 12
        end
        if remaining > 9 then
            self._second = ( math.floor(remaining / 10 % 10) ) + 1
        else
            self._second = 12
        end
        self._third = ( math.floor(remaining % 10) ) + 1
    end
end

function Remain:draw()
    love.graphics.draw(self._image, self._quads[self._first + self._theme], self._firstx, self._top, 0, self._scale)
    love.graphics.draw(self._image, self._quads[self._second + self._theme], self._secondx, self._top, 0, self._scale)
    love.graphics.draw(self._image, self._quads[self._third + self._theme], self._thirdx, self._top, 0, self._scale)
end

