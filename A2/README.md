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

- Player SR CR S FR FG FB OR OG OB OT V
  SR: int; shape radius
  CR: int; collision radius
  S: int; speed
  FR, FG, FB: int, int, int; fill color
  OR, OG, OB: int, int, int; outline color
  OT: int; outline thickness
  V: shape vertices

- Enemy SR CR SMIN SMAX OR OG OB OT VMIN VMAX L SI
  SMIN, SMAX: float, float; min/max speed
  VMIN, VMAX: int, int; min/max vertices
  L: int; lifespan of smaller enemies that it generates when it dies
  SP: int, spawn interval

- Bullet SR CR S FR FG FB OR OG OB OT V L


### Game Specifications
- Enemy fill color is random