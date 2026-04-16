//la base de datos se va a usar para decidir que se guarda cuando se guarda y donde se guarda, coordina todo
//la diferencia con fichero.c es que fichero abre archivos y escribe en ellos, IMPORTANTE no mezclarlos porque luego es mas dificil pasarlo a SQL si queremos
#include <stdio.h>
#include "basededatos.h"

//El modulo basededatos sirve para coordinar todo, centralizando la carga y guardado de datos de todos los modulos y
// por eso llamamos a todos los modulos y los usamos al cargar guardar y cerrar.
#include "clientes.h"
#include "peluqueras.h"
#include "servicios.h"
#include "reservas.h"
#include "ficheros.h"

void inicializarBD(){
    //todavia no hay q cargar datos, es para mas adelante caundo haya estructuras
    //hay q conectar la base de datos a esto.
}

void cargarBD(){

    cargarClientes();
    cargarPeluqueras();
    cargarServicios();
    cargarReservas();
}


void guardarBD(){

    guardarClientes();
    guardarPeluqueras();
    guardarServicios();
    guardarReservas();
}

void cerrarBD(){
    
    liberarClientes();
    liberarPeluqueras();
    liberarServicios();
    liberarReservas();
}