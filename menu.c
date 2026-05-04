#include <stdio.h>
#include "menu.h"
#include "clientes.h"
#include "peluqueras.h"
#include "servicios.h"
#include "reservas.h"
#include "config.h"

void menuPrincipal(){
    int op;
    do{
        printf("\n      MENU PRINCIPAL\n");
        printf("1. Clientes\n");
        printf("2. Peluqueras\n");
        printf("3. Servicios\n");
        printf("4. Reservas\n");
        printf("5. Configuracion\n");
        printf("0. Salir\n");
        scanf("%d", &op);

        switch (op){
            case 1:
                menuClientes();
                break;
            case 2:
                menuPeluqueras();
                break;
            case 3:
                menuServicios();
                break;
            case 4:
                menuReservas();
                break;
            case 5:
                mostrarConfig();
                break;
            case 0:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion invalida \n");
        }
    } while (op != 0);
}