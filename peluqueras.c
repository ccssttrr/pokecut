#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "peluqueras.h"

Peluquera *peluqueras = NULL;
int numPeluqueras = 0;
int capacidadPeluqueras = 0;


void menuPeluqueras(){
    int op;
    int id;

    do{
        printf("\n1. Alta\n");
        printf("2. Buscar\n");
        printf("3. Editar\n");
        printf("4. Listar\n");
        printf("5. Fichar entrada\n");
        printf("6. Fichar salida\n");
        printf("0. Volver\n");
        printf("Opcion: ");
        scanf("%d", &op);
        getchar();

        if (op == 1) altaPeluquera();
        else if (op == 2) buscarPeluquera();
        else if (op == 3) editarPeluquera();
        else if (op == 4) listarPeluqueras();
        else if (op == 5) {
            printf("ID de la peluquera: ");
            scanf("%d", &id);
            getchar();
            ficharEntrada(id);
        }else if (op == 6) {
            printf("ID de la peluquera: ");
            scanf("%d", &id);
            getchar();
            ficharSalida(id);
        }

    } while (op != 0);
}
void inicializarPeluqueras(){
    capacidadPeluqueras = 10;
    numPeluqueras = 0;

    peluqueras = malloc(capacidadPeluqueras * sizeof(Peluquera));

    if (!peluqueras){
        printf("Error de memoria\n");
        exit(1);
    }
}

int generarNuevoIdPeluquera() {

    if (numPeluqueras == 0) {
        return 1;
    }
    
    int maxId = 0;
    for (int i = 0; i < numPeluqueras; i++) {
        if (peluqueras[i].id > maxId) {
            maxId = peluqueras[i].id;
        }
    }
    return maxId + 1;
}

int existeIdPeluquera(int id) {

    for (int i = 0; i < numPeluqueras; i++) {
        if (peluqueras[i].id == id) {
            return 1;
        }
    }
    return 0;
}

void altaPeluquera(){

    if (numPeluqueras >= capacidadPeluqueras){
        capacidadPeluqueras *= 2;
        Peluquera *temp = realloc(peluqueras, capacidadPeluqueras * sizeof(Peluquera));
        
        if (temp == NULL){
            printf("Error de memoria \n");
            exit(1);
        }
        peluqueras = temp;
    }

    Peluquera p;
    
    //asignar ID a nuevas peluqeras automaticamente
    p.id = generarNuevoIdPeluquera();
    
    printf("\n--- NUEVA PELUQUERA ---\n");
    printf("ID asignado: %d\n", p.id);

    printf("Nombre: ");
    fgets(p.nombre, sizeof(p.nombre), stdin);
    p.nombre[strcspn(p.nombre, "\n")] = '\0';

    printf("Especialidad: ");
    fgets(p.especialidad, sizeof(p.especialidad), stdin);
    p.especialidad[strcspn(p.especialidad, "\n")] = '\0';

    printf("Telefono: ");
    fgets(p.telefono, sizeof(p.telefono), stdin);
    p.telefono[strcspn(p.telefono, "\n")] = '\0';

    p.horasTrabajadas = 0;

    peluqueras[numPeluqueras] = p;
    numPeluqueras++;

    guardarPeluqueras();
    printf("\nPeluquera registrada con exito! ID: %d\n", p.id);
}

void listarPeluqueras(){

    if (numPeluqueras == 0) {
        printf("No hay peluqueras registradas\n");
        return;
    }
    
    for (int i = 0; i < numPeluqueras; i++) {
        printf("\n--- Peluquera numero %d ---\n", i+1);
        printf("ID: %d\n", peluqueras[i].id);
        printf("Nombre: %s\n", peluqueras[i].nombre);
        printf("Especialidad: %s\n", peluqueras[i].especialidad);
        printf("Telefono: %s\n", peluqueras[i].telefono);
        printf("Horas trabajadas: %.1f\n \n", peluqueras[i].horasTrabajadas);
    }
}

void buscarPeluquera(){

    int id;
    printf("ID de la peluquera: "); //mucho mas seguro buscar todo por id y no por nombre porque igual nombre se repite, ademas si le das el id he puesto para que se enseñe el nombre luego
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < numPeluqueras; i++) {
        if (peluqueras[i].id == id) {
            printf("ID: %d\n", peluqueras[i].id);
            printf("Nombre: %s\n", peluqueras[i].nombre);
            printf("Especialidad: %s\n", peluqueras[i].especialidad);
            printf("Telefono: %s\n", peluqueras[i].telefono);
            printf("Horas trabajadas: %.1f\n", peluqueras[i].horasTrabajadas);
            return;
        }
    }
    printf("Peluquera no encontrada\n");
}

void editarPeluquera(){
    int id;
    printf("ID de la peluquera a editar: ");
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < numPeluqueras; i++){
        if (peluqueras[i].id == id) {
            printf("\nID: %d\n", peluqueras[i].id);
            printf("Nombre actual: %s\n", peluqueras[i].nombre);
            printf("Nuevo nombre (dejar vacio para mantener): ");
            char nuevoNombre[50];
            fgets(nuevoNombre, sizeof(nuevoNombre), stdin);
            nuevoNombre[strcspn(nuevoNombre, "\n")] = '\0';
            
            if (strlen(nuevoNombre) > 0){
                strcpy(peluqueras[i].nombre, nuevoNombre);
            }

            printf("Especialidad actual: %s\n", peluqueras[i].especialidad);
            printf("Nueva especialidad (dejar vacio para mantener): ");
            char nuevaEspecialidad[50];
            fgets(nuevaEspecialidad, sizeof(nuevaEspecialidad), stdin);
            nuevaEspecialidad[strcspn(nuevaEspecialidad, "\n")] = '\0';
            
            if (strlen(nuevaEspecialidad) > 0){
                strcpy(peluqueras[i].especialidad, nuevaEspecialidad);
            }

            printf("Telefono actual: %s\n", peluqueras[i].telefono);
            printf("Nuevo telefono (dejar vacio para mantener): ");
            char nuevoTelefono[20];
            fgets(nuevoTelefono, sizeof(nuevoTelefono), stdin);
            nuevoTelefono[strcspn(nuevoTelefono, "\n")] = '\0';
            
            if (strlen(nuevoTelefono) > 0){
                strcpy(peluqueras[i].telefono, nuevoTelefono);
            }

            guardarPeluqueras();
            printf("\nPeluquera editada\n");
            return;
        }
    }
    printf("Peluquera con ID %d no encontrada\n", id);
}

void ficharEntrada(int id){

    for (int i = 0; i < numPeluqueras; i++){

        if (peluqueras[i].id == id) {
            printf("Entrada registrada para %s\n", peluqueras[i].nombre);
            return;
        }
    }
    printf("Peluquera no encontrada\n");
}

void ficharSalida(int id){
    float horas;
    printf("Horas trabajadas hoy: ");
    scanf("%f", &horas);
    getchar();
    
    for (int i = 0; i < numPeluqueras; i++){

        if (peluqueras[i].id == id) {
            peluqueras[i].horasTrabajadas += horas; //la suma de las horas
            guardarPeluqueras();
            printf("Salida registrada para %s. Total horas acumuladas: %.1f\n", peluqueras[i].nombre, peluqueras[i].horasTrabajadas);
            return;
        }
    }
    printf("Peluquera no encontrada\n");
}

void ficharSalidaSimple(int id){

    for (int i = 0; i < numPeluqueras; i++){
        if (peluqueras[i].id == id) {
            peluqueras[i].horasTrabajadas += 8;
            guardarPeluqueras();
            printf("Salida registrada para %s\n", peluqueras[i].nombre);
            return;
        }
    }
    printf("Peluquera no encontrada\n");
}

// --- tema ficheros
void guardarPeluqueras(){

    FILE *f = fopen("peluqueras.txt", "w");
    if (!f) return;
    
    for (int i = 0; i < numPeluqueras; i++){
        fprintf(f, "%d,%s,%s,%s,%.1f\n",
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
    if (!f) {
        printf("No se encontró peluqueras.txt, se creará uno nuevo al guardar\n");
        return;
    }

    capacidadPeluqueras = 10;
    numPeluqueras = 0;

    peluqueras = malloc(capacidadPeluqueras * sizeof(Peluquera));
    if (!peluqueras) {
        fclose(f);
        return;
    }

    Peluquera p;
    char linea[300];

    while (fgets(linea, sizeof(linea), f)) {
        linea[strcspn(linea, "\n")] = '\0';
        
        if (sscanf(linea, "%d,%49[^,],%49[^,],%19[^,],%f", &p.id, p.nombre, p.especialidad, p.telefono, &p.horasTrabajadas) == 5){

            if (numPeluqueras >= capacidadPeluqueras){
                capacidadPeluqueras *= 2;
                Peluquera *temp = realloc(peluqueras, capacidadPeluqueras * sizeof(Peluquera));

                if (!temp) {
                    printf("Error de memoria\n");
                    fclose(f);
                    return;
                }
                peluqueras = temp;
            }
            peluqueras[numPeluqueras] = p;
            numPeluqueras++;
        }
    }
    fclose(f);
}

void liberarPeluqueras(){
    
    if (peluqueras != NULL) {
        free(peluqueras);
        peluqueras = NULL;
    }
    numPeluqueras = 0;
    capacidadPeluqueras = 0;
}