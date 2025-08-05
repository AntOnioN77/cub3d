# CUB3D

## Lista de Tareas

### 1. Validar el Mapa

#### Verificaciones:
- **Formato del archivo**: extensión `.cub`.
- **Elementos obligatorios**:
  - Texturas: `NO`, `SO`, `WE`, `EA`.
  - Colores: `F`, `C`.
  - El mapa.
- **Formato de texturas**: rutas válidas a archivos `.xpm`.
- **Formato de colores**: RGB en formato `R,G,B` (0-255).

#### Requisitos del Mapa:
- Solo caracteres permitidos: `0`, `1`, `N`, `S`, `E`, `W`, espacios.
- Un único punto de spawn del jugador.
- Cerrado por paredes (los bordes deben ser `1` o espacios que no conecten con `0`).
- **Duda:** ¿Se consideraría un mapa abierto cuando es posible un paso en diagonal?

```text
111  <---
1001
1111
```

### 2. Cargar el Mapa en Memoria (Array 2D)

#### Pasos:
1. **Parsear línea por línea**:
   - Separar elementos de configuración del mapa.
2. **Crear array 2D**:
   - Recorrer el mapa una vez, para contar el número de líneas y la longitud de la línea más larga.
   - Crear un array dinámico con `malloc()` mapa[numero_de_lineas][ancho_de_linea].
   - Rellenar con espacios si es necesario para rectangularizar.
3. **Verificar cierre del mapa**:
   - Usar flood fill o método similar para asegurar que el mapa está cerrado.

