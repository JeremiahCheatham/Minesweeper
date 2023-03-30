Board = {}

function Board:new(image, width, height)
    local newBoard = {}
    LoadSpriteSheet(newBoard, image, width, height)
    setmetatable(newBoard, { __index = self })
    return newBoard
end

function Board:set_theme(theme)
    self._theme = theme
end

function Board:game_won()
    return self._game_won
end

function Board:game_lost()
    return self._game_lost
end

function Board:reset(scale, left, top, columns, rows, mines)
    self._scale = scale
    self._left = left
    self._top = top
    self._columns = columns
    self._rows = rows
    self._mines = mines
    self._width = self._originalWidth * scale
    self._height = self._originalHeight * scale
    self._remaining_moves = ( rows  * columns ) - mines
    self._game_lost = false
    self._game_won = false
    self._first_move = true

    self:generate()
end

function Board:generate()
    self._back = {}
    self._front = {}
    for row = 1, self._rows do
        self._back[row] = {}
        self._front[row] = {}
        for column = 1, self._columns do
            self._back[row][column] = 10
            self._front[row][column] = 9
        end
    end
    local i = 0
    while i < self._mines do
        local row = math.random(self._rows)
        local column = math.random(self._columns)
        if self._back[row][column] ~= 14 then
            self._back[row][column] = 14
            for y = row - 1, row + 1 do
                for x = column - 1, column + 1 do
                    if y >0 and y <= self._rows and x > 0 and x<= self._columns then
                        if self._back[y][x] == 10 then
                            self._back[y][x] = 1
                        elseif self._back[y][x] < 9 then
                            self._back[y][x] = self._back[y][x] + 1
                        end
                    end
                end
            end
            i = i + 1
        end
    end
end

function Board:tilePressed(x, y)
    local column = math.floor((x - self._left) / self._width) + 1
    local row = math.floor((y - self._top) / self._height) + 1
    if self._front[row][column] == 9 then
        return true
    end
    return false
end

function Board:tileReleased(x, y)
    local column = math.floor((x - self._left) / self._width) + 1
    local row = math.floor((y - self._top) / self._height) + 1
    if self._front[row][column] == 9 then
        local tiles = {{row, column}}
        while tiles[1] ~= nil do
            local tile = table.remove(tiles)
            local r = tile[1]
            local c = tile[2]                
            if self._front[r][c] == 9 and self._back[r][c] < 11 then
                self._front[r][c] = self._back[r][c]
                self._remaining_moves = self._remaining_moves - 1
                if self._back[r][c] == 10 then
                    for y = r - 1, r + 1 do
                        for x = c - 1, c + 1 do
                            if y >0 and y <= self._rows and x > 0 and x<= self._columns then
                                if y ~= r or x ~= c then
                                    if self._front[y][x] == 9 then
                                        table.insert(tiles, {y, x})
                                    end
                                end
                            end
                        end
                    end
                end
            elseif self._back[r][c] == 14 then
                if self._first_move == true then
                    while self._back[r][c] == 14 do
                        self:generate()
                    end
                    self:tileReleased(x, y)
                else
                    self._front[r][c] = 15
                    self:lost()
                end
            end
        end
        if self._remaining_moves <= 0 then
            if self._first_move == true then
                self:generate()
                self:tileReleased(x, y)
            else
                self:won()
            end
        end
    end
    self._first_move = false
end

function Board:won()
    self._game_won = true
    for row = 1, self._rows do
        for column = 1, self._columns do
            if self._back[row][column] == 14 then
                self._front[row][column] = 14
            end
        end
    end
end

function Board:lost()
    self._game_lost = true
    for row = 1, self._rows do
        for column = 1, self._columns do
            if self._front[row][column] == 9 or self._front[row][column] == 12 then
                self._front[row][column] = self._back[row][column]
            elseif self._front[row][column] == 11 then
                if self._back[row][column] == 14 then
                    self._front[row][column] = 14
                else
                    self._front[row][column] = 16
                end
            end
        end
    end
end

function Board:flagTile(x, y)
    local column = math.floor((x - self._left) / self._width) + 1
    local row = math.floor((y - self._top) / self._height) + 1
    local remaining = 0
    if self._front[row][column] == 9 then
        self._front[row][column] = 11
        remaining = -1
    elseif self._front[row][column] == 11 then
        self._front[row][column] = 12
        remaining = 1
    elseif self._front[row][column] == 12 then
        self._front[row][column] = 9
    end
    return remaining
end

function Board:draw()
    for row = 1, self._rows do
        for column = 1, self._columns do
            local x = (column - 1) * self._width + self._left
            local y = (row - 1) * self._height + self._top
            love.graphics.draw(self._image, self._quads[self._front[row][column] + self._theme], x, y, 0, self._scale)
        end
    end
end

