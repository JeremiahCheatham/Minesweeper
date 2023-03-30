Game = {
    _difficulty = 0.095,
    _difficulty_name = "Easy"
}

function Game:load()
    self.border = Border:new("assets/borders.png", 16, 55)
    self.board = Board:new("assets/board.png", 16, 16)
    self.face = Face:new("assets/faces.png", 26, 26)
    self.digitback = DigitBack:new("assets/digitback.png", 41, 25)
    self.remain = Remain:new("assets/digits.png", 13, 23)
    self.clock = Clock:new("assets/digits.png", 13, 23)
    self:set_theme(0, 0, 0, 0, 0)
    self:set_size(2, 9, 9, "Tiny")
end

function Game:set_theme(border, board, face, digitback, clock)
    self.border:set_theme(border)
    self.board:set_theme(board)
    self.face:set_theme(face)
    self.digitback:set_theme(digitback)
    self.clock:set_theme(clock)
    self.remain:set_theme(clock)
end

function Game:set_size(scale, columns, rows, size_name)
    self._scale = scale
    self._columns = columns
    self._rows = rows
    self._size_name = size_name
    self:reset()
end

function Game:set_difficulty(difficulty, difficulty_name)
    self._difficulty = difficulty
    self._difficulty_name = difficulty_name
    self:reset()
end

function Game:reset()
    self._mines = math.floor(self._columns * self._rows * self._difficulty + 0.5)
    self._remaining_mines = self._mines
    self._running = true
    self._flags = 0
    self.border:reset(self._scale, self._columns, self._rows)
    self.board:reset(self._scale, self.border:left_width(), self.border:height(), self._columns, self._rows, self._mines)
    self.face:reset(self._scale, self.border:left_width(), self.border:right(), self.border:height())
    self.digitback:reset(self._scale, self.border:left_width(), self.border:right(), self.face:top())
    self.remain:reset(self._scale, self.digitback:left(), self.face:top(), self._remaining_mines)
    self.clock:reset(self._scale, self.digitback:right(), self.face:top())
    self._width = self.border._right + self.border._right_width
    self._height = self.border._bottom + self.border._bottom_height
    love.window.setMode( self._width, self._height)
    love.window.setTitle( "Minesweeper   "..self._difficulty_name.."   "..self._size_name )
end

function Game:update(dt)
    if self._running then
        self.clock:update(dt)
    end
end

function Game:draw()
    self.border:draw()
    self.board:draw()
    self.face:draw()
    self.digitback:draw()
    self.remain:draw()
    self.clock:draw()
end

function LoadSpriteSheet(t, image, width, height)
    t = t or {}
    t._image = love.graphics.newImage(image)
    t._quads = {}    
    for y = 0, t._image:getHeight() - height, height do
        for x = 0, t._image:getWidth() - width, width do
            table.insert(t._quads, love.graphics.newQuad(x, y, width, height, t._image:getDimensions()))
        end
    end
    t._originalWidth = width
    t._originalHeight = height
end

function Game:keypressed(k)
    if k == '1' then
        self:set_theme(0, 0, 0, 0, 0)
    elseif k == '2' then
        self:set_theme(0, 16, 0, 0, 0)
    elseif k == '3' then
        self:set_theme(0, 32, 0, 0, 0)
    elseif k == '4' then
        self:set_theme(0, 48, 5, 0, 0)
    elseif k == '5' then
        self:set_theme(0, 64, 5, 0, 0)
    elseif k == '6' then
        self:set_theme(0, 80, 5, 0, 0)
    elseif k == '7' then
        self:set_theme(8, 96, 10, 1, 12)
    elseif k == '8' then
        self:set_theme(8, 112, 10, 1, 12)
    elseif k == 't' then
        self:set_size(1, 80, 40, "Huge")
    elseif k == 'r' then
        self:set_size(2, 40, 20, "Large")
    elseif k == 'e' then
        self:set_size(2, 30, 16, "Medium")
    elseif k == 'w' then
        self:set_size(2, 16, 16, "Small")
    elseif k == 'q' then
        self:set_size(2, 9, 9, "Tiny")
    elseif k == 'a' then
        self:set_difficulty(0.095, "Easy")
    elseif k == 's' then
        self:set_difficulty(0.13, "Medium")
    elseif k == 'd' then
        self:set_difficulty(0.165, "Hard")
    elseif k == 'f' then
        self:set_difficulty(0.2, "Very Hard")
    end
end

function Game:mousepressed(x, y, button)
    if button == 1 then
        if y >= self.face:top() and y <= self.face:bottom() then
            if x >= self.face:left() and x <= self.face:right() then
                self.face:set_index(2)
            end
        end
        if self._running then
            if y >= self.border:height() and y <= self.border:bottom() then
                if x >= self.border:left_width() and x <= self.border:right() then
                    if self.board:tilePressed(x, y) then
                        self.face:set_index(3)
                    end
                end
            end
        end
    end
end

function Game:mousereleased(x, y, button)
    if button == 1 then
        if self.face:index() == 2 then
            self.face:set_index(1)
            if y >= self.face:top() and y <= self.face:bottom() then
                if x >= self.face:left() and x <= self.face:right() then
                    self:reset()
                end
            end
        end
    end
    if self._running then
        if button == 1 then
            if self.face:index() == 3 then
                self.face:set_index(1)
                if y >= self.border:height() and y <= self.border:bottom() then
                    if x >= self.border:left_width() and x <= self.border:right() then
                        self.board:tileReleased(x, y)
                        if self.board:game_won() then
                            self._running = false
                            self.face:set_index(4)
                            self._remaining_mines = 0
                            self.remain:update(self._remaining_mines)
                        elseif self.board:game_lost() then
                            self._running = false
                            self.face:set_index(5)
                        end
                    end
                end
            end
        elseif button == 2 then
            if y >= self.border:height() and y <= self.border:bottom() then
                if x >= self.border:left_width() and x <= self.border:right() then
                    local change = self.board:flagTile(x, y)
                    self._remaining_mines = self._remaining_mines + change
                    if change ~= 0 then
                        self.remain:update(self._remaining_mines)
                    end
                end
            end
        end
    end
end