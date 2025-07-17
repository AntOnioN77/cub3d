
##NOTAS

- Una funcion debe sustituir N S E o W por un cero, despues de anotar posicion y direccion del jugador en el struct world.

-Al parsear char_position, añadir Y+0.5 , X+05. Se supone que el personaje esta en el centro de la casilla, pero el plano cartesiano esta numerado por lineas, no por casillas.
    0   1   2   3   4   (columnas/X)
  ┌───┬───┬───┬───┬───┐
0 │[1]│[1]│[1]│[1]│[1]│
  ├───┼───┼───┼───┼───┤
1 │[1]│[N]│[0]│[0]│[1]│
  ├───┼───┼───┼───┼───┤
2 │[1]│[0]│[0]│[0]│[1]│
  ├───┼───┼───┼───┼───┤
3 │[1]│[1]│[1]│[1]│[1]│
  └───┴───┴───┴───┴───┘
(filas/Y)

Y
4 ┼───┼───┼───┼───┼───┼
  │   │   │   │   │   │
3 ┼───┼───┼───┼───┼───┼
  │   │   │   │   │   │
2 ┼───┼───┼───┼───┼───┼
  │   │ N │   │   │   │  N= X1.5 Y1.5
1 ┼───┼───┼───┼───┼───┼
  │   │   │   │   │   │
0 ┼───┼───┼───┼───┼───┼
  0   1   2   3   4   5  X