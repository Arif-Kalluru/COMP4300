### Config file
- Window W H FL FS
  W: int; Width
  H: int; Height
  FL: int; Frame limit
  FS: int; Full screen if 1

- Font F S R G B
  F: string; font file
  S: int; size
  R G B: int, int, int; RGB Color

- Player SR CR S FR FG FB OR OG OB OT V T
  SR: int; shape radius
  CR: int; collision radius
  S: int; speed
  FR, FG, FB: int, int, int; fill color
  OR, OG, OB: int, int, int; outline color
  OT: int; outline thickness
  V: shape vertices
  T: Time before special weapon can be used (in frames)

- Enemy SR CR SMIN SMAX OR OG OB OT VMIN VMAX L SI
  SMIN, SMAX: float, float; min/max speed
  VMIN, VMAX: int, int; min/max vertices
  L: int; lifespan of smaller enemies that it generates when it dies
  SI: int, spawn interval (in frames)

- Bullet SR CR S FR FG FB OR OG OB OT V L

### Instructions to run
```
g++ src/*.cpp -Isrc -std=c++17 -o bin/sfmlgame -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
./bin/sfmlgame
```