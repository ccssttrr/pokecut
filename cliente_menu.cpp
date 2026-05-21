#include <iostream>
#include <iomanip>
#include <limits>
#include "cliente_menu.h"
#include "cliente_red.h"
#include "cliente_cache.h"

using namespace std;

extern ClienteRed cliente;

int leerEnteroSeguro() {
    int valor;

    while (!(cin >> valor) || valor < 0) {
        cin.clear();
        cin.ignore(1000, '\n');

        cout << "Entrada invalida. Introduce un numero positivo: ";
    }

    cin.ignore(1000, '\n');
    return valor;
}

void menuPrincipal(Cache& cache) {
    int opcion;
    int idCliente;
    
    do {
        cout << "\n================================" << endl;
        cout << "       PELUQAPP - CLIENTE       \n" << endl;
        cout << " 1. Ver catalogo de servicios   " << endl;
        cout << " 2. Ver peluqueras              " << endl;
        cout << " 3. Consultar horarios          " << endl;
        cout << " 4. Hacer reserva               " << endl;
        cout << " 5. Mis reservas                " << endl;
        cout << " 6. Cancelar reserva            " << endl;
        cout << " 0. Salir                       " << endl;
        cout << "================================" << endl;
        cout << "Opcion: ";
        opcion = leerEnteroSeguro();
        
        switch (opcion) {
            case 1:
                verServicios(cache);
                break;
            case 2:
                verPeluqueras(cache);
                break;
            case 3:
                consultarHorarios(cache);
                break;
            case 4:
                cout << "ID Cliente: ";
                idCliente = leerEnteroSeguro();
                hacerReserva(cache, idCliente);
                break;
            case 5:
                cout << "ID Cliente: ";
                idCliente = leerEnteroSeguro();
                verMisReservas(cache, idCliente);
                break;
            case 6:
                cout << "ID Cliente: ";
                idCliente = leerEnteroSeguro();
                cancelarReserva(cache, idCliente);
                break;
        }
    } while (opcion != 0);
}

void verServicios(Cache& cache) {
    cout << "\n=== CATALOGO DE SERVICIOS ===" << endl;
    cout << left << setw(5) << "ID"
         << setw(30) << "Nombre"
         << setw(10) << "Precio"
         << setw(10) << "Duracion" << endl;
    cout << string(55, '-') << endl;
    
    for (const auto& s : cache.getServicios()) {
        cout << left << setw(5) << s.id
             << setw(30) << s.nombre
             << setw(10) << s.precio << " EUR"
             << setw(10) << s.duracion << " min" << endl;
    }
}

void verPeluqueras(Cache& cache) {
    cout << "\n=== PELUQUERAS ===" << endl;
    
    for (const auto& p : cache.getPeluqueras()) {
        cout << "ID: " << p.id << endl;
        cout << "Nombre: " << p.nombre << endl;
        cout << "Especialidad: " << p.especialidad << endl;
        cout << "Telefono: " << p.telefono << endl;
        cout << "---" << endl;
    }
}

void consultarHorarios(Cache& cache) {
    string fecha;
    cout << "Fecha (dd/mm/aaaa): ";
    getline(cin, fecha);
    
    vector<string> horarios = cache.getHorarios(fecha);
    
    cout << "\nHorarios disponibles para " << fecha << ":" << endl;
    for (const auto& h : horarios) {
        cout << "  - " << h << endl;
    }
}

void hacerReserva(Cache& cache, int idCliente) {
    verServicios(cache);
    
    int idServicio;
    cout << "ID del servicio: ";
    idServicio = leerEnteroSeguro();
    
    verPeluqueras(cache);
    
    int idPeluquera;
    cout << "ID de la peluquera: ";
    idPeluquera = leerEnteroSeguro();
    
    string fecha, hora;
    cout << "Fecha (dd/mm/aaaa): ";
    getline(cin, fecha);
    cout << "Hora (hh:mm): ";
    getline(cin, hora);
    
    if (cache.crearReserva(idCliente, idPeluquera, idServicio, fecha, hora)) {
        cout << "[OK] Reserva creada con exito" << endl;
    } else {
        cout << "[ERROR] No se pudo crear la reserva" << endl;
    }
}

void verMisReservas(Cache& cache, int idCliente) {
    vector<ReservaInfo> reservas = cache.getMisReservas(idCliente);
    
    if (reservas.empty()) {
        cout << "No tienes reservas" << endl;
        return;
    }
    
    cout << "\n=== MIS RESERVAS ===" << endl;
    for (const auto& r : reservas) {
        cout << "ID: " << r.id << endl;
        cout << "Fecha: " << r.fecha << endl;
        cout << "Hora: " << r.hora << endl;
        cout << "Servicio: " << r.nombreServicio << endl;
        cout << "---" << endl;
    }
}

void cancelarReserva(Cache& cache, int idCliente) {
    verMisReservas(cache, idCliente);
    
    int idReserva;
    cout << "ID de la reserva a cancelar: ";
    idReserva = leerEnteroSeguro(); 

    if (cache.cancelarReserva(idReserva)) {
        cout << "[OK] Reserva cancelada correctamente" << endl;
    } else {
        cout << "[ERROR] No se pudo cancelar la reserva" << endl;
    }
}