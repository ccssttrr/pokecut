#ifndef RESERVAS_H
#define RESERVAS_H

typedef struct {
    int  idReserva;
    int  idCliente;
    int  idPeluquera;
    int  idServicio;
    char fecha[20];
    char hora[10];
} Reserva;

extern Reserva *reservas;
extern int numReservas;
extern int capacidadReservas;

void menuReservas();
void inicializarReservas();
void crearReserva();
void buscarReserva();
void listarReservas();
void cancelarReserva();

void guardarReserva(Reserva r);
void guardarReservas();
void cargarReservas();
void liberarReservas();

#endif
