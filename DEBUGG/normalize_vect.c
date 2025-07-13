#include <stdio.h>
#include <math.h>

// Estructura para representar un vector 2D
typedef struct {
    double x;
    double y;
} Vector2D;

// Función para normalizar un vector 2D
Vector2D normalizar_vector2D(Vector2D vector) {
    Vector2D resultado;
    
    // Calcular la magnitud del vector
    double magnitud = sqrt(vector.x * vector.x + vector.y * vector.y);
    
    // Verificar que la magnitud no sea cero
    if (magnitud == 0.0) {
        printf("Error: No se puede normalizar un vector de magnitud cero.\n");
        resultado.x = 0.0;
        resultado.y = 0.0;
        return resultado;
    }
    
    // Normalizar
    resultado.x = vector.x / magnitud;
    resultado.y = vector.y / magnitud;
    
    return resultado;
}

// Función auxiliar para imprimir un vector 2D
void imprimir_vector2D(Vector2D v) {
    printf("(%.3f, %.3f)\n", v.x, v.y);
}

// Ejemplo de uso
int main() {
    Vector2D vector_original = {1, 1.3};
    Vector2D vector_normalizado;
    
    printf("Vector original: ");
    imprimir_vector2D(vector_original);
    
    vector_normalizado = normalizar_vector2D(vector_original);
    
    printf("Vector normalizado: ");
    imprimir_vector2D(vector_normalizado);
    
    // Verificar que la magnitud es 1
    double magnitud = sqrt(vector_normalizado.x * vector_normalizado.x + 
                          vector_normalizado.y * vector_normalizado.y);
    printf("Magnitud del vector normalizado: %.6f\n", magnitud);
    
    return 0;
}