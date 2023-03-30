DigitBack = {}

function DigitBack:new(image, width, height)
    local newDigitBack = {}
    LoadSpriteSheet(newDigitBack, image, width, height)
    setmetatable(newDigitBack, { __index = self })
    return newDigitBack
end

function DigitBack:set_theme(theme)
    self._theme = theme
end

function DigitBack:left()
    return self._left
end

function DigitBack:right()
    return self._right
end

function DigitBack:reset(scale, left, right, top)
    self._scale = scale
    self._width = self._originalWidth * scale
    self._height = self._originalHeight * scale
    self._top = top + ( 1 * scale )
    self._left = left + ( 3 * scale)
    self._right = right - self._width - ( 3 * scale)
end

function DigitBack:draw()
    love.graphics.draw(self._image, self._quads[1 + self._theme], self._left, self._top, 0, self._scale)
    love.graphics.draw(self._image, self._quads[1 + self._theme], self._right, self._top, 0, self._scale)
end

