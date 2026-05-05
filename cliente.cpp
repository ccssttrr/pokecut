#include <iostream>
#include <string>
#include "cliente_red.h"
#include "cliente_cache.h"
#include "cliente_menu.h"

using namespace std;

ClienteRed cliente;

int main() {
    Cache cache;
    
    cout << "================================" << endl;
    cout << "     PELUQAPP - CLIENTE         " << endl;
    cout << "================================" << endl;
    
    cout << "Conectando al servidor..." << endl;
    if (!cliente.conectar("127.0.0.1", 8081)) {
        cout << "[ERROR] No se pudo conectar al servidor" << endl;
        cout << "[INFO] Asegurate de que el servidor este ejecutandose." << endl;
        cout << "Presiona Enter para salir..." << endl;
        cin.get();
        return 1;
    }
    
    cout << "[OK] Conectado al servidor" << endl;
    
    cout << "Cargando datos desde el servidor..." << endl;
    cache.cargarClientes();
    cache.cargarPeluqueras();
    cache.cargarServicios();
    
    menuPrincipal(cache);
    
    cliente.desconectar();
    cout << "[INFO] Desconectado del servidor" << endl;
    
    return 0;
}