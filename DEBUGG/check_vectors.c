//COMPILACION: gcc -o check_vectors DEBUGG/check_vectors.c -lm
//EJECUCION: ./cub3D | ./check_vectors
/*UTILIDAD: comprueba vectores con el formato:
	printf("char_direction: Y%f X%f\n", punto_Y, punto_x);
	Indicando si el vector esta NORMALIZADO.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TOLERANCE 0.0001  // Tolerancia para considerar un vector normalizado
#define MAX_LINE_LENGTH 256

int main() {
    char line[MAX_LINE_LENGTH];
    char prefix[64];
    float x, y;
    int line_number = 0;
    
    // Leer línea por línea desde stdin
    while (fgets(line, sizeof(line), stdin) != NULL) {
        line_number++;
        
        // Parsear la línea con formato: "char_direction: Y<valor> X<valor>"
        int parsed = 0;
        
        // Parsear con formato Y<valor> X<valor>
        if (sscanf(line, "%63s Y%f X%f", prefix, &y, &x) == 3) {
            parsed = 1;
        }
        
        if (parsed) {
            // Calcular la magnitud del vector
            float magnitude = sqrt(x * x + y * y);
            
            // Verificar si está normalizado (magnitud ≈ 1)
            if (fabs(magnitude - 1.0) < TOLERANCE) {
                printf("Línea %d: Vector (%.6f, %.6f) está NORMALIZADO (magnitud: %.6f)\n", 
                       line_number, x, y, magnitude);
            } else {
                printf("Línea %d: Vector (%.6f, %.6f) NO está normalizado (magnitud: %.6f)\n", 
                       line_number, x, y, magnitude);
            }
        } else {
            printf("Línea %d: Error al parsear la línea: %s", line_number, line);
        }
    }
}