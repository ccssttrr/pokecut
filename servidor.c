#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "servidor.h"
#include "servidor_red.h"
#include "servidor_protocolo.h"
#include "servidor_log.h"
#include "basededatos.h"
#include "config.h"

#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
#else
    #include <unistd.h>
#endif

int ejecutando = 1;
int socket_servidor = -1;

void manejador_salida(int sig) {
    (void)sig;
    printf("\nApagando servidor...\n");
    escribirLog("Servidor apagado por señal");
    ejecutando = 0;
    if (socket_servidor != -1) {
        #ifdef _WIN32
            closesocket(socket_servidor);
        #else
            close(socket_servidor);
        #endif
    }
}

int main() {
    signal(SIGINT, manejador_salida);
    
    printf("[1] Cargando configuración...\n");
    cargarConfig();
    printf("Configuración cargada\n");
    printf("Puerto configurado: %d\n", config.puerto);
    
    printf("[2] Inicializando log...\n");
    inicializarLog();
    escribirLog("=== SERVIDOR INICIADO ===");
    printf("Log inicializado\n");
    
    printf("[3] Inicializando base de datos...\n");
    inicializarBD();
    printf("BD inicializada\n");
    
    printf("    Cargando datos...\n");
    cargarBD();
    printf("Datos cargados\n");
    
    printf("[4] Creando socket en puerto %d...\n", config.puerto);
    socket_servidor = crearSocketServidor(config.puerto);
    if (socket_servidor < 0) {
        printf("Error al crear socket en puerto %d\n", config.puerto);
        escribirLog("Error al crear socket en puerto %d", config.puerto);
        printf("Presiona Enter para salir...\n");
        getchar();
        return 1;
    }
    
    printf("Servidor escuchando en puerto %d\n", config.puerto);
    escribirLog("Servidor escuchando en puerto %d", config.puerto);
    
    printf("[5] Servidor en ejecución. Presiona Ctrl+C para detener.\n");
    
    while (ejecutando) {
        printf("[6] Esperando cliente...\n");
        int socket_cliente = aceptarCliente(socket_servidor);
        if (socket_cliente < 0) {
            printf("    Error al aceptar cliente\n");
            continue;
        }
        
        printf("Cliente conectado\n");
        escribirLog("Cliente conectado");
        
        atenderCliente(socket_cliente);
        
        #ifdef _WIN32
            closesocket(socket_cliente);
        #else
            close(socket_cliente);
        #endif
        
        printf("Cliente desconectado\n");
        escribirLog("Cliente desconectado");
    }
    
    printf("\nLimpiando recursos...\n");
    guardarBD();
    cerrarBD();
    cerrarLog();
    
    printf("Servidor cerrado correctamente\n");
    return 0;
}