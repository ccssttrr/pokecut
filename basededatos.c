#include <stdio.h>
#include "basededatos.h"
#include "clientes.h"
#include "peluqueras.h"
#include "servicios.h"
#include "reservas.h"

sqlite3 *db = NULL;  // conexion global

// Crea las tablas si no existen todavia
static void crearTablas() {
    const char *sql =
        "CREATE TABLE IF NOT EXISTS clientes ("
        "   id       INTEGER PRIMARY KEY,"
        "   nombre   TEXT NOT NULL,"
        "   telefono TEXT NOT NULL,"
        "   email    TEXT"
        ");"

        "CREATE TABLE IF NOT EXISTS peluqueras ("
        "   id               INTEGER PRIMARY KEY,"
        "   nombre           TEXT NOT NULL,"
        "   especialidad     TEXT NOT NULL,"
        "   telefono         TEXT NOT NULL,"
        "   horas_trabajadas REAL NOT NULL DEFAULT 0"
        ");"

        "CREATE TABLE IF NOT EXISTS servicios ("
        "   id          INTEGER PRIMARY KEY,"
        "   nombre      TEXT NOT NULL,"
        "   descripcion TEXT,"
        "   duracion    INTEGER NOT NULL,"
        "   precio      REAL NOT NULL"
        ");"

        "CREATE TABLE IF NOT EXISTS reservas ("
        "   id_reserva   INTEGER PRIMARY KEY,"
        "   id_cliente   INTEGER NOT NULL,"
        "   id_peluquera INTEGER NOT NULL,"
        "   id_servicio  INTEGER NOT NULL,"
        "   fecha        TEXT NOT NULL,"
        "   hora         TEXT NOT NULL,"
        "   FOREIGN KEY (id_cliente)   REFERENCES clientes(id),"
        "   FOREIGN KEY (id_peluquera) REFERENCES peluqueras(id),"
        "   FOREIGN KEY (id_servicio)  REFERENCES servicios(id)"
        ");";

    char *err = NULL;
    if (sqlite3_exec(db, sql, NULL, NULL, &err) != SQLITE_OK) {
        printf("Error creando tablas: %s\n", err);
        sqlite3_free(err);
    }
}

void inicializarBD() {
    int rc = sqlite3_open("peluqueria.db", &db);
    if (rc != SQLITE_OK) {
        printf("Error: %s\n", sqlite3_errmsg(db));
        db = NULL;
        return;
    }

    char *err = NULL;
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER);", NULL, NULL, &err);
    if (err) {
        printf("Error forzando escritura: %s\n", err);
        sqlite3_free(err);
    }

    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", NULL, NULL, NULL);
    crearTablas();
}

void cargarBD() {
    cargarClientes();
    cargarPeluqueras();
    cargarServicios();
    cargarReservas();
}

void guardarBD() {
    // Con SQLite cada operacion ya guarda en el momento
    // Esta funcion se mantiene por compatibilidad con el resto del codigo
}

void cerrarBD() {
    liberarClientes();
    liberarPeluqueras();
    liberarServicios();
    liberarReservas();

    if (db) {
        sqlite3_close(db);
        db = NULL;
    }
}
