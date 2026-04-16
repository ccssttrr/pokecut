//Se encarga de inicializar, cargar, guardar y cerrar todo el sistema.
#include <stdio.h>
#include <string.h>

#include "sistema.h"
#include "config.h"
#include "clientes.h"
#include "peluqueras.h"
#include "servicios.h"
#include "reservas.h"

void inicializarSistema(){
    cargarConfig();
    inicializarClientes();
    inicializarPeluqueras();
    inicializarServicios();
    inicializarReservas();
}

void cargarSistema(){
    cargarClientes();
    cargarPeluqueras();
    cargarServicios();
    cargarReservas();
}

void guardarSistema(){
    guardarClientes();
    guardarPeluqueras();
    guardarServicios();
    guardarReservas();

    guardarConfig();
}

void cerrarSistema(){
    liberarClientes();
    liberarPeluqueras();
    liberarServicios();
    liberarReservas();
}

int login() { //ponemos un log in al principio porque la app solo la van a usar los administradores, asi que solo ellos tienen contraseña
    char user[50];
    char pass[50];

    printf("     LOGIN ADMIN \n");

    printf("Usuario: "); //el usuario y contraseña son: admin, se modifica en el config.txt. donde pone admin admin
    scanf("%s", user);
    printf("Password: ");
    scanf("%s", pass);

    if (strcmp(user, config.adminUser) == 0 && strcmp(pass, config.adminPass) == 0){
        return 1;
    }

    printf("Acceso denegado\n");
    return 0;
}