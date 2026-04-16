#ifndef RESERVAS_H
#define RESERVAS_H

typedef struct {
    int idReserva;
    int idCliente;
    int idPeluquera;
    int idServicio;
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

void guardarReservas();
void cargarReservas();

void liberarReservas();


int existeCliente(int id);
int existePeluquera(int id);
int existeServicio(int id);

char* obtenerNombreCliente(int idCliente);
char* obtenerNombrePeluquera(int idPeluquera);
char* obtenerNombreServicio(int idServicio);

extern Reserva *reservas;
extern int numReservas;
extern int capacidadReservas;

#endif