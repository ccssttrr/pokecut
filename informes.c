//para crear informes (reservas, ingresos....)
#include <stdio.h>
#include "informes.h"

//depende de otros modulos este
#include "reservas.h"
#include "clientes.h"
#include "servicios.h"
#include "peluqueras.h"

void informeReservasTotales(){

    printf("Total reservas: %d\n", numReservas);
}

void informeReservasPorCliente(){

    for (int i = 0; i < numClientes; i++){
        int contador = 0;
        for (int j = 0; j < numReservas; j++) {
            if (reservas[j].idCliente == clientes[i].id)
                contador++;
        }
        printf("Cliente %s: %d reservas\n", clientes[i].nombre, contador);
    }
}

void informeServiciosMasDemandados(){

    for (int i = 0; i < numServicios; i++){
        int contador = 0;
        for (int j = 0; j < numReservas; j++) {
            if (reservas[j].idServicio == servicios[i].id)
                contador++;
        }
        printf("Servicio %s: %d usos\n", servicios[i].nombre, contador);
    }
}

void informeFacturacionEstimada(){

    float total = 0;
    for (int i = 0; i < numReservas; i++){
        for (int j = 0; j < numServicios; j++){
            if (servicios[j].id == reservas[i].idServicio){
                total += servicios[j].precio;
            }
        }
    }
    printf("Facturacion total: %.2f\n", total);
}

void informeFacturacionPorPeluquera(){
    
    for (int i = 0; i < numPeluqueras; i++){
        float total = 0;
        for (int j = 0; j < numReservas; j++){
            if (reservas[j].idPeluquera == peluqueras[i].id){

                for (int k = 0; k < numServicios; k++){
                    if (servicios[k].id == reservas[j].idServicio){
                        total += servicios[k].precio;
                    }
                }
            }
        }

        printf("Peluquera %s: %.2f\n", peluqueras[i].nombre, total);
    }
}