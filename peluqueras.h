#ifndef PELUQUERAS_H
#define PELUQUERAS_H

typedef struct {
    int id;
    char nombre[50];
    char especialidad[50];
    char telefono[20];
    int horasTrabajadas;
} Peluquera;

void menuPeluqueras();
void inicializarPeluqueras();
void altaPeluquera();
void listarPeluqueras();
void buscarPeluquera();
void modificarPeluquera();

void ficharEntrada(int id);
void ficharSalida(int id);

/// para que se guarden losd atos
void guardarPeluqueras();
void cargarPeluqueras();

//memoria
void liberarPeluqueras();

//variables globales
extern Peluquera *peluqueras;
extern int numPeluqueras;
extern int capacidadPeluqueras;

#endif