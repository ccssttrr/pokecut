#include <stdio.h>
#include <stdlib.h>
#include "reservas.h"
#include "clientes.h"
#include "peluqueras.h"
#include "servicios.h"

//el problema es que una reserva da para un servicio, pero queria un N : M por si queria hacer mas de un servicio a la vez
//hay que preguntar como hacerlo o si no dejarlo como una reserva un servicio. Por ahora se queda en un servicio una reserva pero en un futuro si sabemos lo cambiamos
//si no otra solucion es meter servicios con mas de una cosa, ej: cortar y peinar , lavar y peinar... o q en el alisado ya te venga default el lavar por ejemplo
// ---preguntar al profe esto----

Reserva *reservas = NULL;
int numReservas = 0;
int capacidadReservas = 0;

void menuReservas(){
    int op;

    do{
        printf("\n1. Crear\n2. Listar\n3. Buscar\n4. Cancelar\n0. Volver\n");
        printf("opcion: ")
        scanf("%d", &op);

        if (op == 1) crearReserva();
        else if (op == 2) listarReservas();
        else if (op == 3) buscarReserva();
        else if (op == 4) cancelarReserva();

    } while (op != 0);
}

void inicializarReservas(){
    capacidadReservas = 10;
    reservas = malloc(capacidadReservas * sizeof(Reserva));
    if (reservas == NULL){
        printf("Error de memoria \n");
        exit(1);
    }
}


int existeServicio(int id){
    for (int i = 0; i < numServicios; i++)
        if (servicios[i].id == id) return 1;
    return 0;
}

int existePeluquera(int id){

    for (int i = 0; i < numPeluqueras; i++)
        if (peluqueras[i].id == id) return 1;
    return 0;
}

int existeCliente(int id){

    for (int i = 0; i < numClientes; i++)
        if (clientes[i].id == id) return 1;
    return 0;
}

char* obtenerNombreCliente(int idCliente){

    for (int i = 0; i < numClientes; i++) {
        if (clientes[i].id == idCliente) {
            return clientes[i].nombre;  //asumiendo que clientes[i].nombre es un array o puntero
        }
    }
    return "Desconocido";
}

char* obtenerNombrePeluquera(int idPeluquera){

    for (int i = 0; i < numPeluqueras; i++){
        if (peluqueras[i].id == idPeluquera){
            return peluqueras[i].nombre;
        }
    }
    return "Desconocida";
}

char* obtenerNombreServicio(int idServicio){

    for (int i = 0; i < numServicios; i++){
        if (servicios[i].id == idServicio){
            return servicios[i].nombre;
        }
    }
    return "Desconocido";
}

void crearReserva(){

    if (numReservas >= capacidadReservas){
        capacidadReservas *= 2;
        Reserva *temp = realloc(reservas, capacidadReservas * sizeof(Reserva));
        if (temp == NULL) {
            printf("Error de memoria \n");
            return;
        }
        reservas = temp;
    }
    Reserva r;
    printf("ID reserva: ");
    scanf("%d", &r.idReserva);

    printf("ID cliente: ");
    scanf("%d", &r.idCliente);
    if (!existeCliente(r.idCliente)){ //para asegurar de que el cliente existe, si no existe se cancela return nada y ya
        printf("Cliente no existe \n");
        return;
    }

    printf("ID peluquera: ");
    scanf("%d", &r.idPeluquera);
    if (!existePeluquera(r.idPeluquera)){
        printf("Peluquera no existe \n");
        return;
    }

    printf("ID servicio: ");
    scanf("%d", &r.idServicio);
    if (!existeServicio(r.idServicio)){
        printf("Servicio no existe \n");
        return;
    }

    printf("Fecha (dd/mm/aaaa): ");
    scanf("%s", r.fecha);

    printf("Hora (hh:mm): ");
    scanf("%s", r.hora);

    reservas[numReservas] = r;
    numReservas++;
    guardarReservas();

    printf("Reserva creada \n");
}

void buscarReserva(){
    int id;
    printf("ID reserva: "); //para buscar usamos solo ID nombre no porque se puede repetir, luego podemos usamos nombre para sacar por pantalla
    scanf("%d", &id);

    for (int i = 0; i < numReservas; i++){
        if (reservas[i].idReserva == id) {
            printf("\n=== RESERVA ENCONTRADA ===\n");
            printf("ID Reserva: %d\n", reservas[i].idReserva);
            printf("Cliente: %s (ID: %d)\n", obtenerNombreCliente(reservas[i].idCliente), reservas[i].idCliente);
            printf("Peluquera: %s (ID: %d)\n", obtenerNombrePeluquera(reservas[i].idPeluquera), reservas[i].idPeluquera);
            printf("Servicio: %s (ID: %d)\n", obtenerNombreServicio(reservas[i].idServicio), reservas[i].idServicio);
            printf("Fecha: %s\n", reservas[i].fecha);
            printf("Hora: %s\n", reservas[i].hora);
            return;
        }
    }
    printf("Reserva no encontrada\n");
}

void listarReservas(){

    if (numReservas == 0) {
        printf("No hay reservas registradas\n");
        return;
    }

    for (int i = 0; i < numReservas; i++){
        printf("\n--- Reserva %d ---\n", i+1);
        printf("ID Reserva: %d\n", reservas[i].idReserva);
        printf("Cliente: %s (ID: %d)\n", obtenerNombreCliente(reservas[i].idCliente), reservas[i].idCliente);
        printf("Peluquera: %s (ID: %d)\n", obtenerNombrePeluquera(reservas[i].idPeluquera), reservas[i].idPeluquera);
        printf("Servicio: %s (ID: %d)\n", obtenerNombreServicio(reservas[i].idServicio), reservas[i].idServicio);
        printf("Fecha: %s\n", reservas[i].fecha);
        printf("Hora: %s\n", reservas[i].hora);
    }
}

void cancelarReserva(){
    int id;
    printf("ID reserva: ");
    scanf("%d", &id);

    for (int i = 0; i < numReservas; i++) {
        if (reservas[i].idReserva == id) {
            for (int j = i; j < numReservas - 1; j++) {
                reservas[j] = reservas[j + 1];
            }
            numReservas--;
            guardarReservas();
            printf("Reserva cancelada \n");
            return;
        }
    }
    printf("Reserva no encontrada \n");
}

//tema ficheros
void guardarReservas(){

    FILE *f = fopen("reservas.txt", "w");
    if (!f) return;

    for (int i = 0; i < numReservas; i++){
        fprintf(f, "%d,%d,%d,%d,%s,%s \n", reservas[i].idReserva, reservas[i].idCliente, reservas[i].idPeluquera, reservas[i].idServicio, reservas[i].fecha, reservas[i].hora);
    }
    fclose(f);
}


void cargarReservas(){
    FILE *f = fopen("reservas.txt", "r");
    if (!f) return;
    capacidadReservas = 10;
    numReservas = 0;

    if (reservas != NULL){
    free(reservas);
    reservas = NULL;
}
    reservas = malloc(capacidadReservas * sizeof(Reserva));
    Reserva r;

    while (fscanf(f, "%d,%d,%d,%d,%50[^,],%20[^\n]",
        &r.idReserva,
        &r.idCliente,
        &r.idPeluquera,
        &r.idServicio,      //todos estos valores dentro del while porque mientras q lea todo esto sigue ejecutando
        r.fecha,
        r.hora) == 6) { // =6 porque en total 6 campos hay.
            if (numReservas >= capacidadReservas) {
                capacidadReservas *= 2;
                Reserva *temp = realloc(reservas, capacidadReservas * sizeof(Reserva));
                
                if (temp == NULL) {
                    printf("Error de memoria \n");
                    return;
                }
                reservas = temp;
            }
            reservas[numReservas] = r;
            numReservas++;
    }
    fclose(f);
}

void liberarReservas(){
    free(reservas);
    reservas = NULL;
}