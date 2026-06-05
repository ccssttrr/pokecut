#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clientes.h"
#include "basededatos.h"

Cliente *clientes = NULL;
int numClientes = 0;
int capacidadClientes = 0;

void menuClientes() {
    int op;
    do {
        printf("\n--- CLIENTES ---\n");
        printf("1. Alta\n2. Buscar\n3. Listar\n 4. Modificar\n5. Eliminar\n0. Volver\n");
        scanf("%d", &op);

        if      (op == 1) altaCliente();
        else if (op == 2) buscarCliente();
        else if (op == 3) listarClientes();
        else if (op == 4) modificarCliente();
        else if (op == 5) eliminarCliente();

    } while (op != 0);
}

void inicializarClientes() {
    capacidadClientes = 10;
    numClientes = 0;
    clientes = malloc(capacidadClientes * sizeof(Cliente));
    if (!clientes) {
        printf("Error de memoria en inicializarClientes\n");
        exit(1);
    }
    printf("Clientes inicializados con capacidad %d\n", capacidadClientes);
}

void altaCliente() {
    if (numClientes >= capacidadClientes) {
        capacidadClientes *= 2;
        Cliente *temp = realloc(clientes, capacidadClientes * sizeof(Cliente));
        if (!temp) { printf("Error memoria\n"); exit(1); }
        clientes = temp;
    }

    Cliente c;
    printf("ID: ");
    scanf("%d",  &c.id);
    printf("Nombre: ");
    scanf("%49s", c.nombre);
    printf("Telefono: ");
    scanf("%19s", c.telefono);


    clientes[numClientes++] = c;
    guardarCliente(c);  // guarda solo este en SQLite
    printf("Cliente registrado.\n");
}

void buscarCliente() {
    int id;
    printf("ID cliente: ");
    scanf("%d", &id);
    for (int i = 0; i < numClientes; i++) {
        if (clientes[i].id == id) {
            printf("Nombre: %s\nTelefono: %s\nEmail: %s\n",
                clientes[i].nombre, clientes[i].telefono, clientes[i].email);
            return;
        }
    }
    printf("No encontrado.\n");
}

void listarClientes() {
    if (numClientes == 0) { printf("No hay clientes.\n"); return; }
    for (int i = 0; i < numClientes; i++) {
        printf("[%d] %s - %s\n",
            clientes[i].id, clientes[i].nombre, clientes[i].telefono);
    }
}

void modificarCliente() {
    int id;
    printf("ID cliente: ");
    scanf("%d", &id);
    for (int i = 0; i < numClientes; i++) {
        if (clientes[i].id == id) {
            printf("Nuevo nombre: ");    scanf("%49s", clientes[i].nombre);
            printf("Nuevo telefono: ");  scanf("%19s", clientes[i].telefono);
            printf("Nuevo email: ");     scanf("%99s", clientes[i].email);
            guardarCliente(clientes[i]);
            printf("Cliente modificado.\n");
            return;
        }
    }
    printf("No encontrado.\n");
}
void eliminarCliente() {
    int id;
    printf("ID cliente: ");
    scanf("%d", &id);
    for (int i = 0; i < numClientes; i++) {
        if (clientes[i].id == id) {
            clientes[i] = clientes[numClientes - 1];  
            numClientes--;
            printf("Cliente eliminado.\n");
            return;
        }
    }
    printf("No encontrado.\n");
}
// ---- SQLite ----

// Guarda (inserta o actualiza) un cliente en la BD
void guardarCliente(Cliente c) {
    if (!db) return;
    sqlite3_stmt *stmt;
    const char *sql =
        "INSERT OR REPLACE INTO clientes (id, nombre, telefono, email) "
        "VALUES (?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return;

    sqlite3_bind_int (stmt, 1, c.id);
    sqlite3_bind_text(stmt, 2, c.nombre,   -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, c.telefono, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, c.email,    -1, SQLITE_STATIC);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

// Guarda todos los clientes en memoria (para compatibilidad)
void guardarClientes() {
    for (int i = 0; i < numClientes; i++)
        guardarCliente(clientes[i]);
}

// Callback que recibe cada fila de SQLite y la mete en el array
static int callbackCliente(void *unused, int cols, char **valores, char **nombres) {
    (void)unused;
    (void)cols;
    (void)nombres;
    
    if (numClientes >= capacidadClientes) {
        capacidadClientes *= 2;
        Cliente *temp = realloc(clientes, capacidadClientes * sizeof(Cliente));
        if (!temp) {
            printf("Error de memoria en callbackCliente\n");
            return 1;
        }
        clientes = temp;
    }
    
    Cliente c;
    c.id = atoi(valores[0]);
    strncpy(c.nombre, valores[1] ? valores[1] : "", 49);
    strncpy(c.telefono, valores[2] ? valores[2] : "", 19);
    c.nombre[49] = '\0';
    c.telefono[19] = '\0';
    
    clientes[numClientes++] = c;
    
    return 0;
}

void cargarClientes() {
    printf("Ejecutando consulta SELECT...\n");
    
    if (!db) {
        printf("Error: db es NULL\n");
        return;
    }
    
    // Asegurar que clientes está inicializado
    if (clientes == NULL) {
        printf("Inicializando clientes antes de cargar...\n");
        inicializarClientes();
    }
    
    printf("Preparando consulta...\n");
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, nombre, telefono FROM clientes;";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Error preparando consulta: %s\n", sqlite3_errmsg(db));
        return;
    }
    printf("Consulta preparada correctamente\n");
    
    printf("Ejecutando consulta...\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (numClientes >= capacidadClientes) {
            int nuevaCapacidad = capacidadClientes * 2;
            printf("Ampliando memoria: %d -> %d\n", capacidadClientes, nuevaCapacidad);
            Cliente *temp = realloc(clientes, nuevaCapacidad * sizeof(Cliente));
            if (!temp) {
                printf("Error de memoria en realloc\n");
                sqlite3_finalize(stmt);
                return;
            }
            clientes = temp;
            capacidadClientes = nuevaCapacidad;
        }
        
        Cliente c;
        c.id = sqlite3_column_int(stmt, 0);
        strncpy(c.nombre, (const char*)sqlite3_column_text(stmt, 1), 49);
        strncpy(c.telefono, (const char*)sqlite3_column_text(stmt, 2), 19);
        c.nombre[49] = '\0';
        c.telefono[19] = '\0';
        
        clientes[numClientes++] = c;
        printf("Cliente cargado: ID=%d, Nombre=%s\n", c.id, c.nombre);
    }
    
    sqlite3_finalize(stmt);
    printf("Clientes cargados correctamente. Total: %d\n", numClientes);
}


void liberarClientes() {
    free(clientes);
    clientes = NULL;
    numClientes = 0;
    capacidadClientes = 0;
}

