#ifndef PELUQUERAS_H
#define PELUQUERAS_H

typedef struct {
    int  id;
    char nombre[50];
    char especialidad[50];
    char telefono[20];
    int  horasTrabajadas;
} Peluquera;

extern Peluquera *peluqueras;
extern int numPeluqueras;
extern int capacidadPeluqueras;

void menuPeluqueras();
void inicializarPeluqueras();
void altaPeluquera();
void buscarPeluquera();
void listarPeluqueras();
void modificarPeluquera();
void ficharEntrada(int id);
void ficharSalida(int id);

void guardarPeluquera(Peluquera p);
void guardarPeluqueras();
void cargarPeluqueras();
void liberarPeluqueras();

#endif
