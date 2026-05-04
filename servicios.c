#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "servicios.h"
#include "basededatos.h"

Servicio *servicios = NULL;
int numServicios = 0;
int capacidadServicios = 0;

void menuServicios() {
    int op;
    do {
        printf("\n--- SERVICIOS ---\n");
        printf("1. Alta\n2. Buscar\n3. Listar\n4. Eliminar\n0. Volver\n");
        scanf("%d", &op);

        if      (op == 1) altaServicio();
        else if (op == 2) buscarServicio();
        else if (op == 3) listarServicios();
        else if (op == 4) eliminarServicio();

    } while (op != 0);
}

void inicializarServicios() {
    capacidadServicios = 10;
    numServicios = 0;
    servicios = malloc(capacidadServicios * sizeof(Servicio));
    if (!servicios) { printf("Error de memoria\n"); exit(1); }
}

void altaServicio() {
    if (numServicios >= capacidadServicios) {
        capacidadServicios *= 2;
        Servicio *temp = realloc(servicios, capacidadServicios * sizeof(Servicio));
        if (!temp) { printf("Error de memoria\n"); exit(1); }
        servicios = temp;
    }

    Servicio s;
    printf("ID: ");           scanf("%d",   &s.id);
    printf("Nombre: ");       scanf("%49s",  s.nombre);
    printf("Descripcion: ");  scanf("%99s",  s.descripcion);
    printf("Duracion (min): ");scanf("%d",  &s.duracion);
    printf("Precio: ");       scanf("%f",   &s.precio);

    servicios[numServicios++] = s;
    guardarServicio(s);
    printf("Servicio registrado.\n");
}

void buscarServicio() {
    int id;
    printf("ID servicio: ");
    scanf("%d", &id);
    for (int i = 0; i < numServicios; i++) {
        if (servicios[i].id == id) {
            printf("Nombre: %s\nDescripcion: %s\nDuracion: %d min\nPrecio: %.2f\n",
                servicios[i].nombre, servicios[i].descripcion,
                servicios[i].duracion, servicios[i].precio);
            return;
        }
    }
    printf("Servicio no encontrado.\n");
}

void listarServicios() {
    if (numServicios == 0) { printf("No hay servicios.\n"); return; }
    for (int i = 0; i < numServicios; i++) {
        printf("[%d] %s - %d min - %.2f EUR\n",
            servicios[i].id, servicios[i].nombre,
            servicios[i].duracion, servicios[i].precio);
    }
}

void modificarServicio() {
    int id;
    printf("ID servicio: ");
    scanf("%d", &id);
    for (int i = 0; i < numServicios; i++) {
        if (servicios[i].id == id) {
            printf("Nuevo nombre: ");      scanf("%49s", servicios[i].nombre);
            printf("Nueva descripcion: "); scanf("%99s", servicios[i].descripcion);
            printf("Nueva duracion: ");    scanf("%d",  &servicios[i].duracion);
            printf("Nuevo precio: ");      scanf("%f",  &servicios[i].precio);
            guardarServicio(servicios[i]);
            printf("Servicio modificado.\n");
            return;
        }
    }
    printf("Servicio no encontrado.\n");
}

void eliminarServicio() {
    int id;
    printf("ID servicio: ");
    scanf("%d", &id);
    for (int i = 0; i < numServicios; i++) {
        if (servicios[i].id == id) {
            // Borrar de la BD
            if (db) {
                sqlite3_stmt *stmt;
                sqlite3_prepare_v2(db, "DELETE FROM servicios WHERE id = ?;", -1, &stmt, NULL);
                sqlite3_bind_int(stmt, 1, id);
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
            // Borrar del array en memoria
            for (int j = i; j < numServicios - 1; j++)
                servicios[j] = servicios[j + 1];
            numServicios--;
            printf("Servicio eliminado.\n");
            return;
        }
    }
    printf("No encontrado.\n");
}

// ---- SQLite ----

void guardarServicio(Servicio s) {
    if (!db) return;
    sqlite3_stmt *stmt;
    const char *sql =
        "INSERT OR REPLACE INTO servicios (id, nombre, descripcion, duracion, precio) "
        "VALUES (?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return;

    sqlite3_bind_int (stmt, 1, s.id);
    sqlite3_bind_text(stmt, 2, s.nombre,      -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, s.descripcion, -1, SQLITE_STATIC);
    sqlite3_bind_int (stmt, 4, s.duracion);
    sqlite3_bind_double(stmt, 5, (double)s.precio);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void guardarServicios() {
    for (int i = 0; i < numServicios; i++)
        guardarServicio(servicios[i]);
}

static int callbackServicio(void *unused, int cols, char **valores, char **nombres) {
    (void)unused; (void)cols; (void)nombres;

    if (numServicios >= capacidadServicios) {
        capacidadServicios *= 2;
        servicios = realloc(servicios, capacidadServicios * sizeof(Servicio));
    }

    Servicio s;
    s.id = atoi(valores[0]);
    strncpy(s.nombre,      valores[1] ? valores[1] : "", 49);
    strncpy(s.descripcion, valores[2] ? valores[2] : "", 99);
    s.duracion = valores[3] ? atoi(valores[3]) : 0;
    s.precio   = valores[4] ? (float)atof(valores[4]) : 0.0f;

    servicios[numServicios++] = s;
    return 0;
}

void cargarServicios() {
    if (!db) return;
    numServicios = 0;
    char *err = NULL;
    sqlite3_exec(db,
        "SELECT id, nombre, descripcion, duracion, precio FROM servicios;",
        callbackServicio, NULL, &err);
    if (err) { printf("Error cargando servicios: %s\n", err); sqlite3_free(err); }
}

void liberarServicios() {
    free(servicios);
    servicios = NULL;
    numServicios = 0;
    capacidadServicios = 0;
}
