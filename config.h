#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    char horarioApertura[10];
    char horarioCierre[10];
    int duracionPorDefecto;
    int antelacionMaxima;
    char rutaBD[100];
    char rutaLogs[100];
    char adminUser[50];
    char adminPass[50];
} Config;

extern Config config;

void inicializarConfig();
void cargarConfig();
void guardarConfig();
void mostrarConfig();

#endif