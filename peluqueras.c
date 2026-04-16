#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "peluqueras.h"

Peluquera *peluqueras = NULL;
int numPeluqueras = 0;
int capacidadPeluqueras = 0;


void menuPeluqueras(){
    int op;
    do{
        printf("1. Alta\n2. Buscar\n3. Listar\n0. Volver\n");
        scanf("%d", &op);

        if (op == 1) altaPeluquera();
        else if (op == 2) buscarPeluquera();
        else if (op == 3) listarPeluqueras();

    } while (op != 0);
}

void inicializarPeluqueras(){
    capacidadPeluqueras = 10;

    peluqueras = malloc(capacidadPeluqueras * sizeof(Peluquera));
    if (!peluqueras){
        printf("Error de memoria\n");
        exit(1);
    }
}

void altaPeluquera(){

        if (numPeluqueras >= capacidadPeluqueras){
        capacidadPeluqueras *= 2;

        Peluquera *temp = realloc(peluqueras, capacidadPeluqueras * sizeof(Peluquera));
        if (temp == NULL){//asegurarnos de que realloc no de null
            printf("Error de memoria \n");
            exit(1);
        }
        peluqueras = temp;
    }

    Peluquera p;

    printf("ID: ");
    scanf("%d", &p.id); //como es un tipo simple siq ue suamos la direccion de memoria

    printf("Nombre: ");
    scanf("%s", p.nombre); //en estos casos no usamos el & proque es un array y como tal ya es una direccion asi que no hay que señalar nada

    printf("Especialidad: ");
    scanf("%s", p.especialidad);

    printf("Telefono: ");
    scanf("%s", p.telefono);

    p.horasTrabajadas = 0;

    peluqueras[numPeluqueras] = p;
    numPeluqueras++;

    guardarPeluqueras();

    printf("Peluquera registrada \n");
}

void listarPeluqueras(){
    for (int i = 0; i < numPeluqueras; i++) {
        printf("ID: %d \n", peluqueras[i].id);
        printf("Nombre: %s \n", peluqueras[i].nombre);
        printf("Especialidad: %s \n", peluqueras[i].especialidad);
        printf("Telefono: %s \n", peluqueras[i].telefono);
        printf("Horas trabajadas: %d \n", peluqueras[i].horasTrabajadas);
    }
}

void buscarPeluquera(){
    int id;
    printf("ID peluquera: ");
    scanf("%d", &id);

    for (int i = 0; i < numPeluqueras; i++) {
        if (peluqueras[i].id == id) {
            printf("Nombre: %s \n", peluqueras[i].nombre);
            printf("Especialidad: %s \n", peluqueras[i].especialidad);
            printf("Telefono: %s \n", peluqueras[i].telefono);
            return;
        }
    }
    printf("No encontrada \n");
}

void modificarPeluquera(){
    int id;
    printf("ID peluquera: ");
    scanf("%d", &id);
    for (int i = 0; i < numPeluqueras; i++){
        if (peluqueras[i].id == id) {

            printf("Nuevo nombre: ");
            scanf("%s", peluqueras[i].nombre);

            printf("Nueva especialidad: ");
            scanf("%s", peluqueras[i].especialidad);

            printf("Nuevo telefono: ");
            scanf("%s", peluqueras[i].telefono);

            guardarPeluqueras();

            printf("Modificada \n");
            return;
        }
    }
    printf("No encontrada \n");
}

void ficharEntrada(int id){
    for (int i = 0; i < numPeluqueras; i++){
        if (peluqueras[i].id == id) {
            printf("Entrada registrada \n");
        }
    }
}

void ficharSalida(int id){
    for (int i = 0; i < numPeluqueras; i++){
        if (peluqueras[i].id == id) {
            peluqueras[i].horasTrabajadas += 8;//suponiendo que todas trabajan 8 horas
            guardarPeluqueras();
            printf("Salida registrada \n");
        }
    }
}

void guardarPeluqueras(){
    FILE *f = fopen("peluqueras.txt", "w");
    if (!f) return;
    for (int i = 0; i < numPeluqueras; i++){
        fprintf(f, "%d,%s,%s,%s,%d\n",
            peluqueras[i].id,
            peluqueras[i].nombre,
            peluqueras[i].especialidad,
            peluqueras[i].telefono,
            peluqueras[i].horasTrabajadas);
    }
    fclose(f);
}

void cargarPeluqueras(){
    FILE *f = fopen("peluqueras.txt", "r");
    if (!f) return;

    capacidadPeluqueras = 10;
    numPeluqueras = 0;

    peluqueras = malloc(capacidadPeluqueras * sizeof(Peluquera));

    Peluquera p;

    while (fscanf(f, "%d,%49[^,],%49[^,],%19[^,],%d",
        &p.id,
        p.nombre,
        p.especialidad,
        p.telefono,
        &p.horasTrabajadas) == 5){

        if (numPeluqueras >= capacidadPeluqueras){
            capacidadPeluqueras *= 2;
            Peluquera *temp = realloc(peluqueras, capacidadPeluqueras * sizeof(Peluquera));
            if (!temp) {
                printf("Error de memoria \n");
                return;
            }
            peluqueras = temp;
        }
        peluqueras[numPeluqueras] = p;
        numPeluqueras++;
    }
    fclose(f);
}

void liberarPeluqueras(){
    free(peluqueras);
    peluqueras = NULL;
}