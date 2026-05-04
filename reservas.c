#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reservas.h"
#include "clientes.h"
#include "peluqueras.h"
#include "servicios.h"
#include "basededatos.h"

Reserva *reservas = NULL;
int numReservas = 0;
int capacidadReservas = 0;

void menuReservas() {
    int op;
    do {
        printf("\n--- RESERVAS ---\n");
        printf("1. Crear\n2. Listar\n3. Buscar\n4. Cancelar\n0. Volver\n");
        scanf("%d", &op);

        if      (op == 1) crearReserva();
        else if (op == 2) listarReservas();
        else if (op == 3) buscarReserva();
        else if (op == 4) cancelarReserva();

    } while (op != 0);
}

void inicializarReservas() {
    capacidadReservas = 10;
    numReservas = 0;
    reservas = malloc(capacidadReservas * sizeof(Reserva));
    if (!reservas) { printf("Error de memoria\n"); exit(1); }
}

// Funciones auxiliares de validacion
static int existeCliente(int id) {
    for (int i = 0; i < numClientes; i++)
        if (clientes[i].id == id) return 1;
    return 0;
}

static int existePeluquera(int id) {
    for (int i = 0; i < numPeluqueras; i++)
        if (peluqueras[i].id == id) return 1;
    return 0;
}

static int existeServicio(int id) {
    for (int i = 0; i < numServicios; i++)
        if (servicios[i].id == id) return 1;
    return 0;
}

void crearReserva() {
    if (numReservas >= capacidadReservas) {
        capacidadReservas *= 2;
        Reserva *temp = realloc(reservas, capacidadReservas * sizeof(Reserva));
        if (!temp) { printf("Error de memoria\n"); return; }
        reservas = temp;
    }

    Reserva r;
    printf("ID reserva: ");   scanf("%d", &r.idReserva);
    printf("ID cliente: ");   scanf("%d", &r.idCliente);
    if (!existeCliente(r.idCliente))  { printf("Cliente no existe.\n");   return; }

    printf("ID peluquera: "); scanf("%d", &r.idPeluquera);
    if (!existePeluquera(r.idPeluquera)) { printf("Peluquera no existe.\n"); return; }

    printf("ID servicio: ");  scanf("%d", &r.idServicio);
    if (!existeServicio(r.idServicio))  { printf("Servicio no existe.\n");  return; }

    printf("Fecha (dd/mm/aaaa): "); scanf("%19s", r.fecha);
    printf("Hora (hh:mm): ");       scanf("%9s",  r.hora);

    reservas[numReservas++] = r;
    guardarReserva(r);
    printf("Reserva creada.\n");
}

void buscarReserva() {
    int id;
    printf("ID reserva: ");
    scanf("%d", &id);
    for (int i = 0; i < numReservas; i++) {
        if (reservas[i].idReserva == id) {
            printf("Cliente: %d | Peluquera: %d | Servicio: %d\nFecha: %s  Hora: %s\n",
                reservas[i].idCliente, reservas[i].idPeluquera,
                reservas[i].idServicio, reservas[i].fecha, reservas[i].hora);
            return;
        }
    }
    printf("Reserva no encontrada.\n");
}

void listarReservas() {
    if (numReservas == 0) { printf("No hay reservas.\n"); return; }
    for (int i = 0; i < numReservas; i++) {
        printf("[%d] Cliente:%d Peluquera:%d Servicio:%d %s %s\n",
            reservas[i].idReserva, reservas[i].idCliente,
            reservas[i].idPeluquera, reservas[i].idServicio,
            reservas[i].fecha, reservas[i].hora);
    }
}

void cancelarReserva() {
    int id;
    printf("ID reserva: ");
    scanf("%d", &id);
    for (int i = 0; i < numReservas; i++) {
        if (reservas[i].idReserva == id) {
            // Borrar de la BD
            if (db) {
                sqlite3_stmt *stmt;
                sqlite3_prepare_v2(db, "DELETE FROM reservas WHERE id_reserva = ?;", -1, &stmt, NULL);
                sqlite3_bind_int(stmt, 1, id);
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
            // Borrar del array en memoria
            for (int j = i; j < numReservas - 1; j++)
                reservas[j] = reservas[j + 1];
            numReservas--;
            printf("Reserva cancelada.\n");
            return;
        }
    }
    printf("Reserva no encontrada.\n");
}

// ---- SQLite ----

void guardarReserva(Reserva r) {
    if (!db) return;
    sqlite3_stmt *stmt;
    const char *sql =
        "INSERT OR REPLACE INTO reservas "
        "(id_reserva, id_cliente, id_peluquera, id_servicio, fecha, hora) "
        "VALUES (?, ?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return;

    sqlite3_bind_int (stmt, 1, r.idReserva);
    sqlite3_bind_int (stmt, 2, r.idCliente);
    sqlite3_bind_int (stmt, 3, r.idPeluquera);
    sqlite3_bind_int (stmt, 4, r.idServicio);
    sqlite3_bind_text(stmt, 5, r.fecha, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, r.hora,  -1, SQLITE_STATIC);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void guardarReservas() {
    for (int i = 0; i < numReservas; i++)
        guardarReserva(reservas[i]);
}

static int callbackReserva(void *unused, int cols, char **valores, char **nombres) {
    (void)unused; (void)cols; (void)nombres;

    if (numReservas >= capacidadReservas) {
        capacidadReservas *= 2;
        reservas = realloc(reservas, capacidadReservas * sizeof(Reserva));
    }

    Reserva r;
    r.idReserva   = atoi(valores[0]);
    r.idCliente   = atoi(valores[1]);
    r.idPeluquera = atoi(valores[2]);
    r.idServicio  = atoi(valores[3]);
    strncpy(r.fecha, valores[4] ? valores[4] : "", 19);
    strncpy(r.hora,  valores[5] ? valores[5] : "",  9);

    reservas[numReservas++] = r;
    return 0;
}

void cargarReservas() {
    if (!db) return;
    numReservas = 0;
    char *err = NULL;
    sqlite3_exec(db,
        "SELECT id_reserva, id_cliente, id_peluquera, id_servicio, fecha, hora FROM reservas;",
        callbackReserva, NULL, &err);
    if (err) { printf("Error cargando reservas: %s\n", err); sqlite3_free(err); }
}

void liberarReservas() {
    free(reservas);
    reservas = NULL;
    numReservas = 0;
    capacidadReservas = 0;
}
