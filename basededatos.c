#include <stdio.h>
#include <string.h>
#include "basededatos.h"
#include "clientes.h"
#include "peluqueras.h"
#include "servicios.h"
#include "reservas.h"

sqlite3 *db = NULL;

void inicializarBD() {
    printf("Abriendo base de datos 'peluqueria.db'...\n");
    int rc = sqlite3_open("peluqueria.db", &db);
    if (rc != SQLITE_OK) {
        printf("Error al abrir BD: %s\n", sqlite3_errmsg(db));
        db = NULL;
        return;
    }
    printf("Base de datos abierta correctamente\n");
    
    printf("Activando claves foráneas...\n");
    char *err = NULL;
    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", NULL, NULL, &err);
    if (err) {
        printf("Error en PRAGMA: %s\n", err);
        sqlite3_free(err);
    }
    
    printf("Creando tablas si no existen...\n");
    crearTablas();
    
    // Inicializar las estructuras de datos
    inicializarClientes();
    inicializarPeluqueras();
    inicializarServicios();
    inicializarReservas();
}

void crearTablas() {
    const char *sql =
        "CREATE TABLE IF NOT EXISTS clientes ("
        "   id       INTEGER PRIMARY KEY,"
        "   nombre   TEXT NOT NULL,"
        "   telefono TEXT NOT NULL"
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
    int rc = sqlite3_exec(db, sql, NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        printf("    ❌ Error creando tablas: %s\n", err);
        sqlite3_free(err);
    } else {
        printf("Tablas verificadas/creadas correctamente\n");
    }
}

void cargarBD() {
    printf("Cargando clientes...\n");
    cargarClientes();
    printf("Cargando peluqueras...\n");
    cargarPeluqueras();
    printf(" servicios...\n");
    cargarServicios();
    printf("Cargando reservas...\n");
    cargarReservas();
    printf("Base de datos cargada completamente\n");
}

void guardarBD() {
    // SQLite guarda automáticamente, pero forzamos escritura
    if (db) {
        sqlite3_exec(db, "PRAGMA wal_checkpoint;", NULL, NULL, NULL);
    }
}

void cerrarBD() {
    printf("Liberando memoria de estructuras...\n");
    liberarClientes();
    liberarPeluqueras();
    liberarServicios();
    liberarReservas();

    if (db) {
        printf("Cerrando base de datos...\n");
        sqlite3_close(db);
        db = NULL;
        printf("Base de datos cerrada\n");
    }
}