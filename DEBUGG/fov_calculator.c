#include <stdio.h>
#include <math.h>

// Función para calcular la magnitud del plano de cámara
double calcular_plano_camara(int fov) {
    // Convertir FOV de grados a radianes
    double fov_radianes = fov * M_PI / 180.0;
    
    // Calcular magnitud del plano: |plane| = tan(FOV/2)
    double magnitud_plano = tan(fov_radianes / 2.0);
    
    return magnitud_plano;
}

int main() {
    int fov;
    double magnitud_plano;
    
    printf("=== CALCULADORA DE PLANO DE CAMARA ===\n");
    printf("Ingresa el campo de vision deseado (FOV) en grados: ");
    scanf("%d", &fov);
    
    // Validar entrada
    if (fov <= 0 || fov >= 180) {
        printf("Error: El FOV debe estar entre 1 y 179 grados.\n");
        return 1;
    }
    
    // Calcular magnitud del plano
    magnitud_plano = calcular_plano_camara(fov);
    
    // Mostrar resultados
    printf("\n=== RESULTADOS ===\n");
    printf("FOV solicitado: %d grados\n", fov);
    printf("Magnitud del plano de camara: %.6f\n", magnitud_plano);
    
    // Ejemplo de código para raycasting
    printf("\n=== CODIGO PARA TU RAYCASTER ===\n");
    printf("double dirX = -1, dirY = 0;           // Direccion inicial\n");
    printf("double planeX = 0, planeY = %.6f;    // Plano para FOV %d°\n", 
           magnitud_plano, fov);
    
    // Verificación (calcular FOV desde la magnitud)
    double fov_verificacion = 2.0 * atan(magnitud_plano) * 180.0 / M_PI;
    printf("\nVerificacion: %.2f grados\n", fov_verificacion);
    
    // Tabla de valores comunes
    printf("\n=== TABLA DE VALORES COMUNES ===\n");
    printf("FOV\tMagnitud del Plano\n");
    printf("---\t------------------\n");
    
    int fovs_comunes[] = {30, 45, 60, 70, 80, 90, 120};
    int num_fovs = sizeof(fovs_comunes) / sizeof(fovs_comunes[0]);
    
    for (int i = 0; i < num_fovs; i++) {
        double mag = calcular_plano_camara(fovs_comunes[i]);
        printf("%d°\t%.6f\n", fovs_comunes[i], mag);
    }
    
    return 0;
}

/* 
COMPILACION:
gcc -o fov_calculator fov_calculator.c -lm

EJEMPLO DE USO:
./fov_calculator

FORMULA UTILIZADA:
|plane| = tan(FOV/2)

DONDE:
- FOV es el campo de vision en radianes
- |plane| es la magnitud del vector plano de camara
- Se asume que |dir| = 1 (vector direccion normalizado)
*/
