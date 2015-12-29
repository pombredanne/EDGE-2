local tiny = require("tiny")

local talkingSystem = tiny.processingSystem()
talkingSystem.filter = tiny.requireAll("name", "mass", "phrase")
function talkingSystem:process(e, dt)
    e.mass = e.mass + dt * 3
    print(e.name .. ", who weighs " .. e.mass .. " pounds, says, \"" .. e.phrase .. "\"")
end

local joe = {
    name = "Joe",
    phrase = "I'm a plumber.",
    mass = 150,
    hairColor = "brown"
}

local world = tiny.world(talkingSystem, joe)

for i = 1, 20 do
    world:update(1)
end
