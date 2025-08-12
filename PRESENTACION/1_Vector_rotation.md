Excelente pregunta. Vamos a construir el concepto desde cero para que quede cristalino.

# Multiplicación de Matrices

### 1. **Regla Fundamental**
Para multiplicar dos matrices A × B:
- El número de **columnas de A** debe ser igual al número de **filas de B**
- El resultado tendrá las **filas de A** y las **columnas de B**

```
[2×3] × [3×2] = [2×2]
  A       B    Resultado
```

### 2. **El Proceso: Fila por Columna**

Para cada elemento del resultado:
- Tomas una **fila** de la primera matriz
- Tomas una **columna** de la segunda matriz
- Multiplicas elemento por elemento y **sumas todo**

Ejemplo simple:
```
[1  2]   [5]     [1×5 + 2×6]     [5+12]     [17]
[3  4] × [6]  =  [3×5 + 4×6]  =  [15+24] =  [39]
```

### 3. **Aplicación a la Rotación de Vectores**

En tu caso, tienes:
- **Matriz de rotación** (2×2)
- **Vector** (2×1) - el vector se trata como una matriz columna

```
[cos(θ)  -sin(θ)]   [x]     [cos(θ)×x + (-sin(θ))×y]     [x·cos(θ) - y·sin(θ)]
[sin(θ)   cos(θ)] × [y]  =  [sin(θ)×x + cos(θ)×y]     =  [x·sin(θ) + y·cos(θ)]
```

### 4. **Desglose Detallado del Cálculo**

Vamos paso a paso para obtener el nuevo x' (primera fila):

```
Primera fila de matriz × Vector columna:
[cos(θ)  -sin(θ)] × [x]
                    [y]

= cos(θ) × x  +  (-sin(θ)) × y
= x·cos(θ) - y·sin(θ)  ← Este es x'
```

Para obtener el nuevo y' (segunda fila):

```
Segunda fila de matriz × Vector columna:
[sin(θ)   cos(θ)] × [x]
                    [y]

= sin(θ) × x  +  cos(θ) × y
= x·sin(θ) + y·cos(θ)  ← Este es y'
```

### 5. **Ejemplo Numérico Concreto**

Rotemos el vector (1, 0) por 90°:
- cos(90°) = 0
- sin(90°) = 1

```
[0  -1]   [1]     [0×1 + (-1)×0]     [0]
[1   0] × [0]  =  [1×1 + 0×0]     =  [1]
```

¡El vector (1,0) rotado 90° se convierte en (0,1)! 
Pasó de apuntar hacia la derecha a apuntar hacia arriba.

### 6. **Por qué lo hacemos así**

Tu función `rotate_vector` implementa exactamente esta multiplicación matricial:

```c
nx = v->x * cosv - v->y * sinv;  // Primera fila × vector
ny = v->x * sinv + v->y * cosv;  // Segunda fila × vector
```

Es literalmente la multiplicación matriz×vector "desenrollada" en dos líneas de código.

### 7. **Truco Mnemotécnico**

Para recordar la matriz de rotación:
```
[cos  -sin]
[sin   cos]
```
- La diagonal principal tiene **cos**
- La diagonal secundaria tiene **sin**
- El **-sin** está arriba a la derecha (como rotación antihoraria)


# Rotación Horaria y Rotación Antihoraria

### **Antihoraria (sentido contrario a las agujas del reloj):**
```
[cos(θ)  -sin(θ)]
[sin(θ)   cos(θ)]
```

### **Horaria (sentido de las agujas del reloj):**
```
[cos(θ)   sin(θ)]
[-sin(θ)  cos(θ)]
```

### ¿Qué cambia?
Solo se **intercambian los signos de los senos**:
- El `-sin` pasa de estar arriba a estar abajo
- El `sin` pasa de estar abajo a estar arriba

### Verificación con un ejemplo (rotar 90°):

**Antihoraria** (vector (1,0) → (0,1)):
```
[0  -1]   [1]     [0]
[1   0] × [0]  =  [1]  ← Apunta hacia arriba
```

**Horaria** (vector (1,0) → (0,-1)):
```
[0   1]   [1]     [0]
[-1  0] × [0]  =  [-1]  ← Apunta hacia abajo
```

### Truco alternativo: Rotación horaria = Rotación antihoraria con ángulo negativo

Matemáticamente:
- Rotar θ grados en sentido horario
- Es equivalente a rotar -θ grados en sentido antihorario

Porque:
- cos(-θ) = cos(θ)
- sin(-θ) = -sin(θ)

Por eso en el código:
```c
if (world->key_down['a'])  // Rotar a la izquierda (horario desde arriba)
{
    rotate_vector(&world->char_direction, -ROT_SIN, ROT_COS);
    //                                      ↑ negativo!
}
```

Usa la misma función pero con `-ROT_SIN`, que efectivamente invierte la dirección de rotación. ¡Muy elegante!

¡Exactísimo! Tu intuición es perfecta. Es **exactamente** la matriz de rotación evaluada en 90°. Vamos a verlo:

# Rotacion a 90º
## Derivación de la Fórmula (x,y) → (-y,x)

### 1. **Partimos de la matriz general de rotación:**
```
[cos(θ)  -sin(θ)]
[sin(θ)   cos(θ)]
```

### 2. **Evaluamos en θ = 90°:**
- cos(90°) = 0
- sin(90°) = 1

```
[0  -1]
[1   0]
```

### 3. **Aplicamos esta matriz al vector (x,y):**
```
[0  -1]   [x]     [0·x + (-1)·y]     [-y]
[1   0] × [y]  =  [1·x + 0·y]     =  [x]
```

¡Ahí está! **(x,y) → (-y,x)**

## ¿Por qué es tan elegante?

### **Interpretación Geométrica:**

Piensa en el plano cartesiano:
1. El vector (x,y) apunta hacia algún lugar
2. Para rotarlo 90° antihorario:
   - Lo que apuntaba hacia la **derecha** (+x) ahora apunta **arriba** (+y)
   - Lo que apuntaba hacia **arriba** (+y) ahora apunta a la **izquierda** (-x)

### **Ejemplos visuales:**
```
(1,0) → (0,1)   // Este → Norte
(0,1) → (-1,0)  // Norte → Oeste
(-1,0) → (0,-1) // Oeste → Sur
(0,-1) → (1,0)  // Sur → Este

(2,3) → (-3,2)  // Cualquier vector rota 90°
```

## En tu código de cub3D:

```c
// En calculate_camera_plane
plane_direction->x = -char_dir_y * PLANE_MAGNITUDE;
plane_direction->y = char_dir_x * PLANE_MAGNITUDE;
```

Esto hace **dos cosas a la vez**:
1. **Rota 90°**: (-y, x)
2. **Escala**: multiplica por PLANE_MAGNITUDE

Es como decir: "Dame un vector perpendicular a la dirección de vista, con longitud PLANE_MAGNITUDE"

## Otras rotaciones "especiales":

### **180°**: (x,y) → (-x,-y)
```
[-1  0]
[0  -1]
```

### **270° o -90°**: (x,y) → (y,-x)
```
[0   1]
[-1  0]
```

### **¿Por qué 90° es tan "limpio"?**

Porque cos(90°) = 0 y sin(90°) = 1 son valores **exactos**. No hay decimales, no hay aproximaciones. La matriz se vuelve super simple:
- Los cosenos (0) eliminan términos
- Los senos (±1) solo cambian signos

## Verificación rápida:

Si rotas 4 veces por 90°, deberías volver al vector original:
```
(x,y) → (-y,x) → (-x,-y) → (y,-x) → (x,y) ✓
```

¡Funciona! Es un ciclo perfecto.

Por eso tu función `calculate_camera_plane` es tan elegante: aprovecha que necesitas exactamente 90° y puede usar la forma simplificada en lugar de calcular senos y cosenos. **Eficiencia y claridad** en una sola línea.