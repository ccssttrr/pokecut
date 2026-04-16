#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "peluqueras.h"
#include "basededatos.h"

Peluquera *peluqueras = NULL;
int numPeluqueras = 0;
int capacidadPeluqueras = 0;

void menuPeluqueras() {
    int op;
    do {
        printf("\n--- PELUQUERAS ---\n");
        printf("1. Alta\n2. Buscar\n3. Listar\n0. Volver\n");
        scanf("%d", &op);

        if      (op == 1) altaPeluquera();
        else if (op == 2) buscarPeluquera();
        else if (op == 3) listarPeluqueras();

    } while (op != 0);
}

void inicializarPeluqueras() {
    capacidadPeluqueras = 10;
    numPeluqueras = 0;
    peluqueras = malloc(capacidadPeluqueras * sizeof(Peluquera));
    if (!peluqueras) { printf("Error de memoria\n"); exit(1); }
}

void altaPeluquera() {
    if (numPeluqueras >= capacidadPeluqueras) {
        capacidadPeluqueras *= 2;
        Peluquera *temp = realloc(peluqueras, capacidadPeluqueras * sizeof(Peluquera));
        if (!temp) { printf("Error de memoria\n"); exit(1); }
        peluqueras = temp;
    }

    Peluquera p;
    printf("ID: ");           scanf("%d",  &p.id);
    printf("Nombre: ");       scanf("%49s", p.nombre);
    printf("Especialidad: "); scanf("%49s", p.especialidad);
    printf("Telefono: ");     scanf("%19s", p.telefono);
    p.horasTrabajadas = 0;

    peluqueras[numPeluqueras++] = p;
    guardarPeluquera(p);
    printf("Peluquera registrada.\n");
}

void buscarPeluquera() {
    int id;
    printf("ID peluquera: ");
    scanf("%d", &id);
    for (int i = 0; i < numPeluqueras; i++) {
        if (peluqueras[i].id == id) {
            printf("Nombre: %s\nEspecialidad: %s\nTelefono: %s\nHoras: %d\n",
                peluqueras[i].nombre, peluqueras[i].especialidad,
                peluqueras[i].telefono, peluqueras[i].horasTrabajadas);
            return;
        }
    }
    printf("No encontrada.\n");
}

void listarPeluqueras() {
    if (numPeluqueras == 0) { printf("No hay peluqueras.\n"); return; }
    for (int i = 0; i < numPeluqueras; i++) {
        printf("[%d] %s - %s - Horas: %d\n",
            peluqueras[i].id, peluqueras[i].nombre,
            peluqueras[i].especialidad, peluqueras[i].horasTrabajadas);
    }
}

void modificarPeluquera() {
    int id;
    printf("ID peluquera: ");
    scanf("%d", &id);
    for (int i = 0; i < numPeluqueras; i++) {
        if (peluqueras[i].id == id) {
            printf("Nuevo nombre: ");       scanf("%49s", peluqueras[i].nombre);
            printf("Nueva especialidad: "); scanf("%49s", peluqueras[i].especialidad);
            printf("Nuevo telefono: ");     scanf("%19s", peluqueras[i].telefono);
            guardarPeluquera(peluqueras[i]);
            printf("Peluquera modificada.\n");
            return;
        }
    }
    printf("No encontrada.\n");
}

void ficharEntrada(int id) {
    for (int i = 0; i < numPeluqueras; i++) {
        if (peluqueras[i].id == id) {
            printf("Entrada registrada.\n");
            return;
        }
    }
}

void ficharSalida(int id) {
    for (int i = 0; i < numPeluqueras; i++) {
        if (peluqueras[i].id == id) {
            peluqueras[i].horasTrabajadas += 8;
            guardarPeluquera(peluqueras[i]);
            printf("Salida registrada.\n");
            return;
        }
    }
}

// ---- SQLite ----

void guardarPeluquera(Peluquera p) {
    if (!db) return;
    sqlite3_stmt *stmt;
    const char *sql =
        "INSERT OR REPLACE INTO peluqueras (id, nombre, especialidad, telefono, horas_trabajadas) "
        "VALUES (?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return;

    sqlite3_bind_int (stmt, 1, p.id);
    sqlite3_bind_text(stmt, 2, p.nombre,       -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, p.especialidad, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, p.telefono,     -1, SQLITE_STATIC);
    sqlite3_bind_int (stmt, 5, p.horasTrabajadas);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void guardarPeluqueras() {
    for (int i = 0; i < numPeluqueras; i++)
        guardarPeluquera(peluqueras[i]);
}

static int callbackPeluquera(void *unused, int cols, char **valores, char **nombres) {
    (void)unused; (void)cols; (void)nombres;

    if (numPeluqueras >= capacidadPeluqueras) {
        capacidadPeluqueras *= 2;
        peluqueras = realloc(peluqueras, capacidadPeluqueras * sizeof(Peluquera));
    }

    Peluquera p;
    p.id = atoi(valores[0]);
    strncpy(p.nombre,       valores[1] ? valores[1] : "", 49);
    strncpy(p.especialidad, valores[2] ? valores[2] : "", 49);
    strncpy(p.telefono,     valores[3] ? valores[3] : "", 19);
    p.horasTrabajadas = valores[4] ? atoi(valores[4]) : 0;

    peluqueras[numPeluqueras++] = p;
    return 0;
}

void cargarPeluqueras() {
    if (!db) return;
    numPeluqueras = 0;
    char *err = NULL;
    sqlite3_exec(db,
        "SELECT id, nombre, especialidad, telefono, horas_trabajadas FROM peluqueras;",
        callbackPeluquera, NULL, &err);
    if (err) { printf("Error cargando peluqueras: %s\n", err); sqlite3_free(err); }
}

void liberarPeluqueras() {
    free(peluqueras);
    peluqueras = NULL;
    numPeluqueras = 0;
    capacidadPeluqueras = 0;
}
