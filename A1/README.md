_Contents of bin/config.txt_

- Window W H
W (int)
H (int)

- Font F S R B G
F - file (string)
S - size (int)
R B G (int, int, int)

- Rectangle N X Y SX SY R G B W H
N - name (string)
X, Y - initial position (float, float)
SX, SY - initial speed (float, float)
R G B (int, int, int)
W H - size (float, float)

- Circle N X Y SX SY R G B R
N - name (string)
X, Y - initial position (float, float)
SX, SY - initial speed (float, float)
R G B (int, int, int)
R - Radius (float)
---
- Instructions to run:
`g++ src/*.cpp -Isrc -std=c++17 -o bin/sfmlgame -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio`
`./bin/sfmlgame`

- Possible improvements: avoiding raw pointers, using smart pointers instead