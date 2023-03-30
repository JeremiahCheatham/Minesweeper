Face = {}

function Face:new(image, width, height)
    local newFace = {}
    LoadSpriteSheet(newFace, image, width, height)
    setmetatable(newFace, { __index = self })
    return newFace
end

function Face:set_theme(theme)
    self._theme = theme
end

function Face:index()
    return self._index
end

function Face:set_index(index)
    self._index = index
end

function Face:left()
    return self._left
end

function Face:right()
    return self._right
end

function Face:top()
    return self._top
end

function Face:bottom()
    return self._bottom
end

function Face:reset(scale, left, right, height)
    self._index = 1
    self._scale = scale
    self._width = self._originalWidth * scale
    self._height = self._originalHeight * scale
    self._left = (left + right - self._width ) / 2
    self._right = self._left + self._width
    self._top = math.floor(( height - self._height ) / 2)
    self._bottom = self._top + self._height
end

function Face:draw()
    love.graphics.draw(self._image, self._quads[self._index + self._theme], self._left, self._top, 0, self._scale)
end
