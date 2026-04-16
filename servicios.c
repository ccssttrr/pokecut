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

        printf("\n1. Alta\n2. Buscar\n3. Listar\n0. Volver\n");
        printf("Opcion: ")
        scanf(" %d", &op);

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
    getchar();  //limpiar para que no se pille


    printf("Nombre: ");
    fgets(s.nombre, sizeof(s.nombre), stdin);
    s.nombre[strcspn(s.nombre, "\n")] = '\0';//para quitar el salto de linea


    printf("Descripcion: ");
    fgets(s.descripcion, sizeof(s.descripcion), stdin);
    s.descripcion[strcspn(s.descripcion, "\n")] = '\0';

    printf("Duracion (min): ");
    scanf("%d", &s.duracion);
    getchar();

    printf("Precio: ");
    scanf("%f", &s.precio);
    getchar();

    servicios[numServicios] = s;
    numServicios++;

    guardarServicios();
    printf("Servicio registrado \n");
}

void buscarServicio(){

    int id;
    printf("ID servicio: ");
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < numServicios; i++){
        if (servicios[i].id == id) {

            printf("\n=== SERVICIO ENCONTRADO ===\n");
            printf("ID: %d\n", servicios[i].id);
            printf("Nombre: %s\n", servicios[i].nombre);
            printf("Descripcion: %s\n", servicios[i].descripcion);
            printf("Duracion: %d min\n", servicios[i].duracion);
            printf("Precio: %.2f €\n", servicios[i].precio);
            return;
        }
    }
    printf("Servicio no encontrado\n");
}

void eliminarServicio(){
    int id;
    printf("ID servicio a eliminar: ");
    scanf("%d", &id);
    getchar();
    
    for (int i = 0; i < numServicios; i++){
        if (servicios[i].id == id){
            printf("¿Está seguro de eliminar '%s'? (s/n): ", servicios[i].nombre);
            char confirmar;
            scanf("%c", &confirmar);
            getchar();
            
            if (confirmar == 's' || confirmar == 'S'){
                for (int j = i; j < numServicios - 1; j++){
                    servicios[j] = servicios[j + 1];
                }
                numServicios--;
                guardarServicios();
                printf("Servicio eliminado\n");
            } else {
                printf("Eliminación cancelada\n");
            }
            return;
        }
    }
    printf("Servicio no encontrado\n");
}

void modificarServicio(){
    int id;
    printf("ID servicio a modificar: ");
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < numServicios; i++){

        if (servicios[i].id == id){
            printf("\n--- Modificando servicio: %s ---\n", servicios[i].nombre);
            
            printf("Nuevo nombre (actual: %s): ", servicios[i].nombre);
            fgets(servicios[i].nombre, sizeof(servicios[i].nombre), stdin);
            servicios[i].nombre[strcspn(servicios[i].nombre, "\n")] = '\0';

            printf("Nueva descripcion (actual: %s): ", servicios[i].descripcion);
            fgets(servicios[i].descripcion, sizeof(servicios[i].descripcion), stdin);
            servicios[i].descripcion[strcspn(servicios[i].descripcion, "\n")] = '\0';

            printf("Nueva duracion (actual: %d min): ", servicios[i].duracion);
            scanf("%d", &servicios[i].duracion);
            getchar();

            printf("Nuevo precio (actual: %.2f €): ", servicios[i].precio);
            scanf("%f", &servicios[i].precio);
            getchar();

            guardarServicios();
            printf("Servicio modificado correctamente\n");
            return;
        }
    }
    printf("Servicio no encontrado\n");
}

void listarServicios() {
    
    if (numServicios == 0) {
        printf("No hay servicios registrados\n");
        return;
    }
    
    printf("\n=== LISTADO DE SERVICIOS ===\n");
    for (int i = 0; i < numServicios; i++){
        printf("\n--- Servicio %d ---\n", i+1);
        printf("ID: %d\n", servicios[i].id);
        printf("Nombre: %s\n", servicios[i].nombre);
        printf("Descripcion: %s\n", servicios[i].descripcion);
        printf("Duracion: %d min\n", servicios[i].duracion);
        printf("Precio: %.2f €\n", servicios[i].precio);
    }
}



// ---- tema ficheros
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
    if (!f) {
        //si no hay archivo, inicializamos vacío
        printf("No se encontró servicios.txt, se creará uno nuevo al guardar\n");
        return;
    }

    capacidadServicios = 10;
    numServicios = 0;
    
    if (servicios != NULL){
        free(servicios);
        servicios = NULL;
    }
    
    servicios = malloc(capacidadServicios * sizeof(Servicio));
    if (servicios == NULL){
        fclose(f);
        return;
    }
    
    Servicio s;
    
    char linea[300];
    while (fgets(linea, sizeof(linea), f)) {
        linea[strcspn(linea, "\n")] = '\0';
        
        int leidos = sscanf(linea, "%d,%49[^,],%99[^,],%d,%f", &s.id, s.nombre, s.descripcion, &s.duracion, &s.precio);
        if (leidos == 5) {
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
    }
    fclose(f);
}

void liberarServicios(){
    if (servicios != NULL){
        free(servicios);
        servicios = NULL;
    }
    numServicios = 0;
    capacidadServicios = 0;
}