//en el config vamos a poner como "las reglas del negocio", como horario de tienda o duracion de servicios, 
//pero no para clientes ni peluqueras ni reservas, eso va en basededatos
#include <stdio.h>
#include <string.h>
#include "config.h"

Config config;

void inicializarConfig() {
    strcpy(config.horarioApertura, "09:00");
    strcpy(config.horarioCierre, "20:00");
    config.duracionPorDefecto = 60;
    config.antelacionMaxima = 7;
    strcpy(config.rutaBD, "datos/");
    strcpy(config.rutaLogs, "logs/");
    strcpy(config.adminUser, "admin");
    strcpy(config.adminPass, "1234");
}

void cargarConfig(){
    FILE *f = fopen("config.txt", "r");

    if (!f){
        printf("No existe config.txt, usando valores por defecto\n");
        inicializarConfig();
        return;
    }
    
    inicializarConfig();
    fscanf(f, "HorarioApertura=%s\n", config.horarioApertura);
    fscanf(f, "HorarioCierre=%s\n", config.horarioCierre);
    fscanf(f, "DuracionPorDefecto=%d\n", &config.duracionPorDefecto);
    fscanf(f, "AntelacionMaxima=%d\n", &config.antelacionMaxima);
    fscanf(f, "RutaBD=%s\n", config.rutaBD);
    fscanf(f, "RutaLogs=%s\n", config.rutaLogs);
    fscanf(f, "AdminUser=%s\n", config.adminUser);
    fscanf(f, "AdminPass=%s\n", config.adminPass);

    fclose(f);
}

void guardarConfig(){
    FILE *f = fopen("config.txt", "w");
    if (!f) {
        printf("Error al guardar config\n");
        return;
    }
    fprintf(f, "HorarioApertura=%s\n", config.horarioApertura);
    fprintf(f, "HorarioCierre=%s\n", config.horarioCierre);
    fprintf(f, "DuracionPorDefecto=%d\n", config.duracionPorDefecto);
    fprintf(f, "AntelacionMaxima=%d\n", config.antelacionMaxima);
    fprintf(f, "RutaBD=%s\n", config.rutaBD);
    fprintf(f, "RutaLogs=%s\n", config.rutaLogs);
    fprintf(f, "AdminUser=%s\n", config.adminUser);
    fprintf(f, "AdminPass=%s\n", config.adminPass);

    fclose(f);
}

void mostrarConfig(){
    printf("\n      CONFIGURACION\n");
    printf("Horario apertura: %s\n", config.horarioApertura);
    printf("Horario cierre: %s\n", config.horarioCierre);
    printf("Duracion por defecto: %d minutos\n", config.duracionPorDefecto);
    printf("Antelacion maxima: %d dias\n", config.antelacionMaxima);
    printf("Ruta BD: %s\n", config.rutaBD);
    printf("Ruta logs: %s\n", config.rutaLogs);
    printf("Usuario admin: %s\n", config.adminUser);
}