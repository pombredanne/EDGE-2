## Day XX

### XXX

QUEST
FIRST?: TXT1
COMPLETED? (TXT2 -> NEXT QUEST) : TXT3

DIALOGUE: fox
- a: hi
- b: whats your name?
  - a: $name(mona)
  - a: $name(lisa)
- b: nice to meet you $name

INVENTORY:
tree of {
ITEM, GFX, DESCRIPTION,
NUM USAGES, EXPIRATION DATE,
POWERUP MUL ADD ATTR
}

- stamina, stamina.icon, "use this to recover health",
- usages: 1, during: 10s, requires: health, powerup: { health, 2, 0 }, action: [] { health = health * 2 + 0; }


