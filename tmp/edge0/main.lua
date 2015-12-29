local ecs = require 'libs.knife.system'

--local lfs = require"lfs"
--print (lfs._VERSION)

require('libs.clidebugger.clidebugger')
--pause("hello")

--require("libs.lurker.lurker").update()

-- test console
log("hello from lua")

-- log = require 'libs.log.log'
-- log = log.info

-- test fs
fs.mount( "assets/kenney/Art/Platformer assets*")
fs.mount( "assets/kenney/Audio/*.ogg")
fs.scan()
local file = fs.find
local image = fs.find
local sound = fs.find

ghost = {
    position = {
        100, 100
    },
    rotation = {
        1, 1, 1
    },
    scale = {
        1, 1, 1
    },
    aabb = {
        30, 30, 20
    },
    audio = {
        boo = sound('music loops (30 files)/sad town') 
    },
    GraphicsComponent = {
        alive = image("enemy sprites/ghost"),
        dead = image("enemy sprites/ghost dead")
    },
    NpcComponent = {
        phrase = "I'M A SCARY GHOST!!!"
    },
    --[[
    CollisionComponent = {
        boundingBox = {0, 0, 32, 32},
        collide = function(this, second)
            this:setAnimation("blush")
            this:say("Sorry...")
            second:setDamage(1)
        end
    }]]
}

json = require 'libs.json.json'
log(json.encode(ghost))

window.openScreen(800, 600)

function newEntity()
    return {
        _name = "",
        _x = 0,
        _y = 0,
        _w = 0,
        _h = 0,
        _colour = 0xff000000,

        name = function(self, newValue)
            if (newValue ~= nil) then
                self._name = newValue
            end
            return self._name
        end,

        x = function(self, newValue)
            if (newValue ~= nil) then
                self._x = newValue
            end
            return self._x
        end,

        y = function(self, newValue)
            if (newValue ~= nil) then
                self._y = newValue
            end
            return self._y
        end,

        w = function(self, newValue)
            if (newValue ~= nil) then
                self._w = newValue
            end
            return self._w
        end,

        h = function(self, newValue)
            if (newValue ~= nil) then
                self._h = newValue
            end
            return self._h
        end,

        colour = function(self, newValue)
            if (newValue ~= nil) then
                self._colour = newValue
            end
            return self._colour
        end,

        draw = function(self)
            window.drawRect(self._x, self._y, self._w, self._h, self._colour)
        end,

        sprite = function(self)
            window.drawSprite(self._x, self._y, self._w, self._h, self._name)
        end
    }
end

local blockEntity = newEntity()
blockEntity:w(25)
blockEntity:h(25)
blockEntity:colour(0xffff8800)

local sky = newEntity()
sky:w(window.width())
sky:h(window.height())
sky:colour(0x0088ff00)

local spr = newEntity()
spr:name( ghost.GraphicsComponent.alive )

function mkerror()
    local s = "this is a string"
    local n = 42
    local t = { foo = "bar" }
    local co = coroutine
    local cr = coroutine.create 
    pause("hello")
    error("an error")
end

function update()
    spr:x(input.mouseX()-12)
    spr:y(input.mouseY()-12)

    blockEntity:x(input.mouseX()-12)
    blockEntity:y(input.mouseY()-12)

    if(input.mouseButton() == 2) then
        mkerror()
    elseif(input.mouseButton() == 1) then
        spr:name( ghost.GraphicsComponent.dead )
        blockEntity:colour(0xff000000) -- red
    else
        spr:name( ghost.GraphicsComponent.alive )
        blockEntity:colour(0x00ff0000) -- green
    end
end

function render()
    sky:draw()

    spr:sprite()
    blockEntity:draw()
end

function debug()
    pause("break")
end
