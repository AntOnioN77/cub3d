#include <stdio.h>

int main(void)
{
#if defined(__clang__)
    printf("Compilando con clang\n");
#elif defined(__GNUC__)
    printf("Compilando con gcc\n");
#else
    printf("Compilador desconocido\n");
#endif
    return 0;
}