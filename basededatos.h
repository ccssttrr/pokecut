#ifndef BASEDEDATOS_H
#define BASEDEDATOS_H

#include "sqlite3.h"

extern sqlite3 *db;  // conexion global, la usan todos los modulos

void inicializarBD();
void cargarBD();
void guardarBD();
void cerrarBD();

#endif
