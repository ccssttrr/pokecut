#ifndef BASEDEDATOS_H
#define BASEDEDATOS_H

#include "sqlite3.h"

extern sqlite3 *db;

void inicializarBD();
void cargarBD();
void guardarBD();
void cerrarBD();

void crearTablas();

#endif