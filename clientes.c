#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clientes.h"
#include "reservas.h"

Cliente *clientes = NULL;
int numClientes = 0;
int capacidadClientes = 0;

void menuClientes(){
    int op;
    do{
        printf("\n1. Alta\n");
        printf("2. Buscar\n");

        printf("3. Editar\n");
        printf("4. Listar\n");
        printf("0. Volver\n");
        printf("Opcion: ");
        scanf("%d", &op);

        if (op == 1) altaCliente();
        else if (op == 2) buscarCliente();
        else if (op == 3) editarCliente();
        else if (op == 4) listarClientes();

    } while (op != 0);
}


void inicializarClientes(){
    capacidadClientes = 10;
    numClientes = 0;

    clientes = malloc(capacidadClientes * sizeof(Cliente));

    if (!clientes) {
        printf("Error memoria\n");
        exit(1);
    }
}

int generarNuevoIdCliente() {
    if (numClientes == 0) {
        return 1;   
    }

    
    int maxId = 0;
    for (int i = 0; i < numClientes; i++) {
        if (clientes[i].id > maxId) {
            maxId = clientes[i].id;
        }
    }
    return maxId + 1;
}

int existeIdCliente(int id) {
    for (int i = 0; i < numClientes; i++) {
        if (clientes[i].id == id) {
            return 1;
        }
    }

    return 0;
}

void altaCliente(){
    if (numClientes >= capacidadClientes){
        capacidadClientes *= 2;
        Cliente *temp = realloc(clientes, capacidadClientes * sizeof(Cliente));
        
        if (!temp) {
            printf("Error memoria\n");
            exit(1);
        }
        clientes = temp;
    }

    Cliente c;

    //al ser nuevo cliente, para que no se repita ID se asigna automaticamente
    c.id = generarNuevoIdCliente();

    printf("Nombre: ");
    scanf("%s", c.nombre);

    printf("Telefono: ");
    scanf("%s", c.telefono);

    clientes[numClientes++] = c;

    guardarClientes();
}

void buscarCliente(){
    int id;
    printf("ID del cliente: \n");
    scanf(" %d", &id);
    
    for (int i = 0; i < numClientes; i++){
        if (clientes[i].id == id) {
            printf("Nombre:%s, Tf:%s\n", clientes[i].nombre, clientes[i].telefono);
            return;
        }
    }
    printf("No encontrado\n");
}

void editarCliente(){
    int id;
    printf("ID del cliente a editar: ");
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < numClientes; i++){
        if (clientes[i].id == id) {
            printf("ID: %d\n", clientes[i].id);
            printf("Nombre actual: %s\n", clientes[i].nombre);
            printf("Nuevo nombre (dejar vacio para mantener): ");
            char nuevoNombre[50];
            fgets(nuevoNombre, sizeof(nuevoNombre), stdin);
            nuevoNombre[strcspn(nuevoNombre, "\n")] = '\0';
            
            if (strlen(nuevoNombre) > 0) {
                strcpy(clientes[i].nombre, nuevoNombre);
            }

            printf("Telefono actual: %s\n", clientes[i].telefono);
            printf("Nuevo telefono (dejar vacio para mantener): ");
            char nuevoTelefono[20];
            fgets(nuevoTelefono, sizeof(nuevoTelefono), stdin);
            nuevoTelefono[strcspn(nuevoTelefono, "\n")] = '\0';
            
            if (strlen(nuevoTelefono) > 0) {
                strcpy(clientes[i].telefono, nuevoTelefono);
            }

            guardarClientes();
            printf("\nCliente editado exitosamente!\n");
            return;
        }
    }
    
    printf("Cliente con ID %d no encontrado\n", id);
}


void listarClientes(){
    
    for (int i = 0; i < numClientes; i++){
        printf("\n---Cliente numero %d----\n",i);
        printf("ID:%d, Nombre:%s, Tf:%s\n", clientes[i].id, clientes[i].nombre, clientes[i].telefono);
    }
}


//---tema ficheros
void guardarClientes() {
    FILE *f = fopen("clientes.txt", "w");
    if (!f) return;
    
    for (int i = 0; i < numClientes; i++){
        fprintf(f, "%d,%s,%s\n",clientes[i].id,
            clientes[i].nombre,
            clientes[i].telefono);
    }

    fclose(f);
}


void cargarClientes(){
    FILE *f = fopen("clientes.txt", "r");
    if (!f) return;
    numClientes = 0;
    capacidadClientes = 10;
    
    clientes = malloc(capacidadClientes * sizeof(Cliente));
    Cliente c;

    while (fscanf(f, "%d,%49[^,],%19[^\n]",//el %49 es basicamente leer el fichero max 49 caracteres hasta q haya una coma
        &c.id,
        c.nombre,
        c.telefono) == 3){
        if (numClientes >= capacidadClientes) {
            capacidadClientes *= 2;
            clientes = realloc(clientes, capacidadClientes * sizeof(Cliente));
        }
        clientes[numClientes++] = c;
    }
    fclose(f);
}

void liberarClientes() {
    
    free(clientes);
}