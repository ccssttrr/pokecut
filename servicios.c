//gestiona los servicios ofrecidos (precio, duración....) y define el catalogo que hay para las reservas.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "servicios.h"
#include "reservas.h"

Servicio *servicios = NULL;
int numServicios = 0;
int capacidadServicios = 0;

void menuServicios(){
    int op;
    do{
        printf("1. Alta\n2. Buscar\n3. Listar\n0. Volver\n");
        scanf("%d", &op);

        if (op == 1) altaServicio();
        else if (op == 2) buscarServicio();
        else if (op == 3) listarServicios();

    } while (op != 0);
}

void inicializarServicios(){
    capacidadServicios = 10;
    servicios = malloc(capacidadServicios * sizeof(Servicio));

    if (servicios == NULL){
        printf("Error de memoria \n");
        exit(1);
    }
}

void altaServicio(){
    if (numServicios >= capacidadServicios){
        capacidadServicios *= 2;

        //malloc es para asignar y realloc para hacerlo mas grande o pequeño
        Servicio *temp = realloc(servicios, capacidadServicios * sizeof(Servicio));
        if (temp == NULL){ //para comprobar y asegurar de que realloc nod e null y tenga donde asignar
            printf("Error de memoria \n");
            exit(1);
        }
        

        servicios = temp;

        if (servicios == NULL){
            printf("Error de memoria \n");
            exit(1);
        }
    }
    Servicio s;

    printf("ID: ");
    scanf("%d", &s.id);

    printf("Nombre: ");
    scanf("%s", s.nombre);

    printf("Descripcion: ");
    scanf("%s", s.descripcion);

    printf("Duracion (min): ");
    scanf("%d", &s.duracion);

    printf("Precio: ");
    scanf("%f", &s.precio);

    servicios[numServicios] = s;
    numServicios++;

    guardarServicios();
    printf("Servicio registrado \n");
}

void buscarServicio(){
    int id;
    printf("ID servicio: ");
    scanf("%d", &id);

    for (int i = 0; i < numServicios; i++){
        if (servicios[i].id == id) {
            printf("Nombre: %s \n", servicios[i].nombre);
            printf("Descripcion: %s \n", servicios[i].descripcion);
            printf("Duracion: %d min \n", servicios[i].duracion);
            printf("Precio: %.2f \n", servicios[i].precio);
            return;
        }
    }
    printf("Servicio no encontrado \n");
}

void modificarServicio(){
    int id;
    printf("ID servicio: ");
    scanf("%d", &id);

    for (int i = 0; i < numServicios; i++){
        if (servicios[i].id == id){

            printf("Nuevo nombre: ");
            scanf("%s", servicios[i].nombre);

            printf("Nueva descripcion: ");
            scanf("%s", servicios[i].descripcion);

            printf("Nueva duracion: ");
            scanf("%d", &servicios[i].duracion);

            printf("Nuevo precio: ");
            scanf("%f", &servicios[i].precio);

            guardarServicios();
            printf("Servicio modificado \n");
            return;
        }
    }
    printf("Servicio no encontrado \n");
}

void listarServicios() {
    for (int i = 0; i < numServicios; i++){
        printf("ID: %d \n", servicios[i].id);
        printf("Nombre: %s \n", servicios[i].nombre);
        printf("Duracion: %d \n", servicios[i].duracion);
        printf("Precio: %.2f \n", servicios[i].precio);
    }
}

void eliminarServicio(){
    int id;
    printf("ID servicio: ");
    scanf("%d", &id);
    for (int i = 0; i < numServicios; i++){
        if (servicios[i].id == id){
            for (int j = i; j < numServicios - 1; j++){
                servicios[j] = servicios[j + 1];
            }
            numServicios--;
            guardarServicios();
            printf("Eliminado\n");
            return;
        }
    }
    printf("No encontrado\n");
}




// ----tema ficheros
void guardarServicios(){

    FILE *f = fopen("servicios.txt", "w");
    if (!f) return;

    for (int i = 0; i < numServicios; i++){
        fprintf(f, "%d,%s,%s,%d,%.2f\n",
            servicios[i].id,
            servicios[i].nombre,
            servicios[i].descripcion,
            servicios[i].duracion,
            servicios[i].precio);
    }
    fclose(f);
}

void cargarServicios(){

    FILE *f = fopen("servicios.txt", "r");
    if (!f) return;

    capacidadServicios = 10;
    numServicios = 0;
    
    if (reservas != NULL){ //esto es por si ya existe la reservas en memoria, pues para no perderla
    free(reservas);
}
    servicios = malloc(capacidadServicios * sizeof(Servicio));
    Servicio s;

    while (fscanf(f, "%d,%49[^,],%f,%d", &s.id, s.nombre, &s.precio, &s.duracion) == 4){
        if (numServicios >= capacidadServicios){
            capacidadServicios *= 2;
            Servicio *temp = realloc(servicios, capacidadServicios * sizeof(Servicio));
            if (!temp){
                fclose(f);
                return;
            }
            servicios = temp;
        }
        servicios[numServicios++] = s;
    }
    fclose(f);
}

void liberarServicios(){
    free(servicios);
    servicios = NULL;
}