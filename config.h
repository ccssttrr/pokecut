//en el config vamos a poner como "las reglas del negocio", como horario de tienda o duracion de servicios, 
//pero no para clientes ni peluqueras ni reservas, eso va en basededatos
#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <string.h>

// Estructura
typedef struct{
    char horarioApertura[10];
    char horarioCierre[10];
    int duracionPorDefecto;
    int antelacionMaxima;
    char rutaBD[100];
    char rutaLogs[100];
    char adminUser[50];
    char adminPass[50];
} Config;

// Variable global (declaración, NO definición)
extern Config config;

// Prototipos
void inicializarConfig();
void cargarConfig();
void guardarConfig();
void mostrarConfig();

#endif