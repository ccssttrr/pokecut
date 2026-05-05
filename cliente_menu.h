#ifndef CLIENTE_MENU_H
#define CLIENTE_MENU_H

#include "cliente_cache.h"

void menuPrincipal(Cache& cache);
void verServicios(Cache& cache);
void verPeluqueras(Cache& cache);
void consultarHorarios(Cache& cache);
void hacerReserva(Cache& cache, int idCliente);
void verMisReservas(Cache& cache, int idCliente);
void cancelarReserva(Cache& cache, int idCliente);

#endif