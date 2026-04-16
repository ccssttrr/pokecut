#ifndef SERVICIOS_H
#define SERVICIOS_H

typedef struct {
    int id;
    char nombre[50];
    char descripcion[100];
    int duracion;   //en minutos, luego para mostrarlo podemos convertirlo a horas
    float precio;
} Servicio;

//hacemos los servicios dinamicos y no fijos
extern Servicio *servicios;
extern int numServicios;
extern int capacidadServicios;

void menuServicios();
void inicializarServicios();
void altaServicio();
void buscarServicio();

void modificarServicio();
void listarServicios();
void eliminarServicio();

void guardarServicios();
void cargarServicios();

void liberarServicios();

#endif