#ifndef SERVICIOS_H
#define SERVICIOS_H

typedef struct {
    int   id;
    char  nombre[50];
    char  descripcion[100];
    int   duracion;
    float precio;
} Servicio;

extern Servicio *servicios;
extern int numServicios;
extern int capacidadServicios;

void menuServicios();
void inicializarServicios();
void altaServicio();
void buscarServicio();
void listarServicios();
void modificarServicio();
void eliminarServicio();

void guardarServicio(Servicio s);
void guardarServicios();
void cargarServicios();
void liberarServicios();

#endif
