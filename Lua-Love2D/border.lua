Border = {
    _theme = 0
}

function Border:new(image, width, height)
    local newBorder = {}
    LoadSpriteSheet(newBorder, image, width, height)
    setmetatable(newBorder, { __index = self })
    return newBorder
end

function Border:set_theme(theme)
    self._theme = theme
end

function Border:left_width()
    return self._left_width
end

function Border:right()
    return self._right
end

function Border:height()
    return self._height
end

function Border:bottom()
    return self._bottom
end

function Border:reset(scale, columns, rows)
    self._columns = columns
    self._rows = rows
    self._scale = scale
    self._width = self._originalWidth * scale
    self._height = self._originalHeight * scale
    self._right_width = 8 * scale
    self._left_width = 12 * scale
    self._bottom_height = 8 * scale
    self._top = 0
    self._left = self._left_width - self._width
    self._right = self._left + self._width * ( self._columns + 1 )
    self._bottom = self._top + self._height + self._width * self._rows
end

function Border:draw()
    love.graphics.draw(self._image, self._quads[1 + self._theme], self._left, self._top, 0, self._scale)
    love.graphics.draw(self._image, self._quads[3 + self._theme], self._right, self._top, 0, self._scale)
    love.graphics.draw(self._image, self._quads[6 + self._theme], self._left, self._bottom, 0, self._scale)
    love.graphics.draw(self._image, self._quads[8 + self._theme], self._right, self._bottom, 0, self._scale)

    for row = 0, (self._rows - 1) do
        local y = row * self._width + self._height
        love.graphics.draw(self._image, self._quads[4 + self._theme], self._left, y, 0, self._scale)
        love.graphics.draw(self._image, self._quads[5 + self._theme], self._right, y, 0, self._scale)
    end
    for column = 0, (self._columns - 1) do
        local x = column * self._width + self._left_width
        love.graphics.draw(self._image, self._quads[2 + self._theme], x, self._top, 0, self._scale)
        love.graphics.draw(self._image, self._quads[7 + self._theme], x, self._bottom, 0, self._scale)
    end
end
