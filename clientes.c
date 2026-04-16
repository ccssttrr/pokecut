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
        printf("1. Alta\n2. Buscar\n3. Listar\n0. Volver\n");
        scanf("%d", &op);

        if (op == 1) altaCliente();
        else if (op == 2) buscarCliente();
        else if (op == 3) listarClientes();

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

    printf("ID: ");
    scanf("%d", &c.id);

    printf("Nombre: ");
    scanf("%s", c.nombre);

    printf("Telefono: ");
    scanf("%s", c.telefono);

    printf("email: ");
    scanf("%s", c.email);

    clientes[numClientes++] = c;

    guardarClientes();
}



void buscarCliente(){
    int id;
    scanf(" %d", &id);
    for (int i = 0; i < numClientes; i++){
        if (clientes[i].id == id) {
            printf("%s %s\n", clientes[i].nombre, clientes[i].telefono);
            return;
        }
    }
    printf("No encontrado\n");
}


void listarClientes(){
    for (int i = 0; i < numClientes; i++){
        printf("%d %s %s\n",
            clientes[i].id,
            clientes[i].nombre,
            clientes[i].telefono);
    }
}

void modificarCliente(){
    int id;
    scanf("%d", &id);

    for (int i = 0; i < numClientes; i++){
        if (clientes[i].id == id) {

            scanf("%s", clientes[i].nombre);
            scanf("%s", clientes[i].telefono);

            guardarClientes();
            return;
        }
    }
}

//---tema ficheros
void guardarClientes() {
    FILE *f = fopen("clientes.txt", "w");
    if (!f) return;
    for (int i = 0; i < numClientes; i++){
        fprintf(f, "%d,%s,%s\n",
            clientes[i].id,
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