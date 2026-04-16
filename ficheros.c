//para trabajar con ficheros (logs, comprobaciones, utilidades)
#include <stdio.h>
#include "ficheros.h"

int existeFichero(const char *ruta) {
    
    FILE *f = fopen(ruta, "r");
    if (!f) return 0;
    fclose(f);
    return 1;
}

void escribirLog(const char *mensaje) {
    FILE *f = fopen("logs.txt", "a");
    if (!f) return;

    fprintf(f, "%s\n", mensaje);
    fclose(f);
}