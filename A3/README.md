# config.txt
### Window Specification
`Window W H F M`
- Window Width W int
- Window Height H int
- Frame rate F int
- Toggle Full Screen Mode M bool (0/1)

# assets.txt
### Texture Asset Specification:
`Texture N P X Y S`
- Texture Name N string
- Texture FilePath P string
- Texture X (left) Position X int
- Texture Y (top) Position Y int
- Enable Smooth S bool (0/1)

### Animation Asset Specification:
`Animation N T F D S`
- Animation Name N string
- Texture Name T string
- Frame Count F int
- Animation Duration D int

### Font Asset Specification:
`Font N P`
- Font Name N string
- Font FilePath P string

# Game Specifications:
- The player's sprite and bounding box  are centered on the player's position

# Level Specification:
Game levels will be specified by a level file, which will contain a list of entity specifications, one per line. All (GX, GY) positions given in the level specification file are given in 'grid' coordinates. The grid cells are of size 16 x 16 pixels, and the entity should be positioned such that the bottom left corner of it's texture is aligned with the bottom left corner of the given grid coordinate. The grid starts at (0, 0) in the bottom-left of the screen, and can be seen by pressing the `G` key while the game is running.

`Tile N GX GY`
- Animation Name N string
- Grid X Position GX float
- Grid Y Position GY float

`Decoration N X Y`
- Animation Name N string
- Grid X Position GX float
- Grid Y Position GY float

`Player GX GY CW CH SX SY SM G B`
- Player starting position GX, GY float, float
- Bounding box dimensions CW, CH float, float
- Left/right speed SX float
- Jump speed SY float
- Max speed SM float
- Gravity G float
- Bullet Animation B string

### Keys: (All are remappable)
- W: Jump
- A: Move left
- D: Move right
- P: Pause game
- T: Toggles drawing textures
- C: Toggles drawing bounding boxes of entities
- G: Toggles drawing of the grid
- ESC: Toggle menu