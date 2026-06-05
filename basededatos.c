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
    
    migrarDesdeTXT();

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


void migrarDesdeTXT() {
    //ver si ya hay datos en la BD
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM peluqueras;", -1, &stmt, NULL);
    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    
    if (count > 0) {
        printf("La BD ya tiene datos. No se migra nada.\n");
        return;
    }
    
    printf("BD vacia. Migrando datos desde ficheros TXT...\n");
    
    FILE *f = fopen("peluqueras.txt", "r");
    if (f) {
        char linea[300];
        int migradas = 0;
        while (fgets(linea, sizeof(linea), f)) {
            int id;
            char nombre[50], especialidad[50], telefono[20];
            float horas;
            if (sscanf(linea, "%d,%49[^,],%49[^,],%19[^,],%f", &id, nombre, especialidad, telefono, &horas) == 5) {
                char sql[500];
                sprintf(sql, "INSERT INTO peluqueras (id, nombre, especialidad, telefono, horas_trabajadas) VALUES (%d, '%s', '%s', '%s', %.1f);",
                        id, nombre, especialidad, telefono, horas);
                sqlite3_exec(db, sql, NULL, NULL, NULL);
                migradas++;
            }
        }
        fclose(f);
        printf("  Migradas %d peluqueras\n", migradas);
    }
    
    f = fopen("clientes.txt", "r");
    if (f) {
        char linea[300];
        int migradas = 0;
        while (fgets(linea, sizeof(linea), f)) {
            int id;
            char nombre[50], telefono[20];
            if (sscanf(linea, "%d,%49[^,],%19[^\n]", &id, nombre, telefono) == 3) {
                char sql[500];
                sprintf(sql, "INSERT INTO clientes (id, nombre, telefono) VALUES (%d, '%s', '%s');",
                        id, nombre, telefono);
                sqlite3_exec(db, sql, NULL, NULL, NULL);
                migradas++;
            }
        }
        fclose(f);
        printf("  Migrados %d clientes\n", migradas);
    }

    f = fopen("servicios.txt", "r");
    if (f) {
        char linea[300];
        int migradas = 0;
        while (fgets(linea, sizeof(linea), f)) {
            int id, duracion;
            char nombre[50], descripcion[100];
            float precio;
            if (sscanf(linea, "%d,%49[^,],%99[^,],%d,%f", &id, nombre, descripcion, &duracion, &precio) == 5) {
                char sql[600];
                sprintf(sql, "INSERT INTO servicios (id, nombre, descripcion, duracion, precio) VALUES (%d, '%s', '%s', %d, %.2f);",
                        id, nombre, descripcion, duracion, precio);
                sqlite3_exec(db, sql, NULL, NULL, NULL);
                migradas++;
            }
        }
        fclose(f);
        printf("  Migrados %d servicios\n", migradas);
    }
    
    printf("Migracion completada\n");
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