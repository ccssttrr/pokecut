#include <iostream>
#include <sstream>
#include <cstdlib>
#include "cliente_cache.h"
#include "cliente_red.h"

using namespace std;

extern ClienteRed cliente;

Cache::Cache() 
    : clientesCargados(false)
    , peluquerasCargados(false)
    , serviciosCargados(false) {
}

Cache::~Cache() {
    limpiarCache();
}

void Cache::limpiarCache() {
    clientes.clear();
    peluqueras.clear();
    servicios.clear();
    reservasPorCliente.clear();
    horariosCache.clear();
    clientesCargados = false;
    peluquerasCargados = false;
    serviciosCargados = false;
}

void Cache::cargarClientes() {
    string respuesta = cliente.enviarComando("GET_CLIENTES");
    
    if (respuesta.find("200|OK|") != 0) {
        cerr << "Error cargando clientes: " << respuesta << endl;
        return;
    }
    
    string datos = respuesta.substr(7);
    clientes.clear();
    
    stringstream ss(datos);
    string item;
    
    while (getline(ss, item, ';')) {
        if (item.empty()) continue;
        
        ClienteInfo c;
        stringstream item_ss(item);
        string campo;
        
        getline(item_ss, campo, ',');
        c.id = atoi(campo.c_str());
        getline(item_ss, c.nombre, ',');
        getline(item_ss, c.telefono, ',');
        
        clientes.push_back(c);
    }
    
    clientesCargados = true;
    cout << "Clientes cargados: " << clientes.size() << endl;
}

void Cache::cargarPeluqueras() {
    string respuesta = cliente.enviarComando("GET_PELUQUERAS");
    
    if (respuesta.find("200|OK|") != 0) {
        cerr << "Error cargando peluqueras: " << respuesta << endl;
        return;
    }
    
    string datos = respuesta.substr(7);
    peluqueras.clear();
    
    stringstream ss(datos);
    string item;
    
    while (getline(ss, item, ';')) {
        if (item.empty()) continue;
        
        PeluqueraInfo p;
        stringstream item_ss(item);
        string campo;
        
        getline(item_ss, campo, ',');
        p.id = atoi(campo.c_str());
        getline(item_ss, p.nombre, ',');
        getline(item_ss, p.especialidad, ',');
        getline(item_ss, p.telefono, ',');
        getline(item_ss, campo, ',');
        p.horasTrabajadas = atof(campo.c_str());
        
        peluqueras.push_back(p);
    }
    
    peluquerasCargados = true;
    cout << "Peluqueras cargadas: " << peluqueras.size() << endl;
}

void Cache::cargarServicios() {
    string respuesta = cliente.enviarComando("GET_SERVICIOS");
    
    if (respuesta.find("200|OK|") != 0) {
        cerr << "Error cargando servicios: " << respuesta << endl;
        return;
    }
    
    string datos = respuesta.substr(7);
    servicios.clear();
    
    stringstream ss(datos);
    string item;
    
    while (getline(ss, item, ';')) {
        if (item.empty()) continue;
        
        ServicioInfo s;
        stringstream item_ss(item);
        string campo;
        
        getline(item_ss, campo, ',');
        s.id = atoi(campo.c_str());
        getline(item_ss, s.nombre, ',');
        getline(item_ss, s.descripcion, ',');
        getline(item_ss, campo, ',');
        s.duracion = atoi(campo.c_str());
        getline(item_ss, campo, ',');
        s.precio = atof(campo.c_str());
        
        servicios.push_back(s);
    }
    
    serviciosCargados = true;
    cout << "Servicios cargados: " << servicios.size() << endl;
}

const vector<ClienteInfo>& Cache::getClientes() const {
    return clientes;
}

const vector<PeluqueraInfo>& Cache::getPeluqueras() const {
    return peluqueras;
}

const vector<ServicioInfo>& Cache::getServicios() const {
    return servicios;
}

ClienteInfo Cache::buscarCliente(int id) const {
    for (const auto& c : clientes) {
        if (c.id == id) return c;
    }
    ClienteInfo vacio;
    vacio.id = -1;
    return vacio;
}

PeluqueraInfo Cache::buscarPeluquera(int id) const {
    for (const auto& p : peluqueras) {
        if (p.id == id) return p;
    }
    PeluqueraInfo vacio;
    vacio.id = -1;
    return vacio;
}

ServicioInfo Cache::buscarServicio(int id) const {
    for (const auto& s : servicios) {
        if (s.id == id) return s;
    }
    ServicioInfo vacio;
    vacio.id = -1;
    return vacio;
}

bool Cache::crearReserva(int idCliente, int idPeluquera, int idServicio, const string& fecha, const string& hora) {
    stringstream comando;
    comando << "CREAR_RESERVA|" << idCliente << "|" << idPeluquera << "|" 
            << idServicio << "|" << fecha << "|" << hora;
    
    string respuesta = cliente.enviarComando(comando.str());
    
    cout << "DEBUG Respuesta reserva: " << respuesta << endl;
    
    if (respuesta.find("200|OK|") == 0) {
        reservasPorCliente.erase(idCliente);
        cout << "[OK] Reserva creada con exito" << endl;
        return true;
    }
    else if (respuesta.find("400|ERROR|") == 0) {
        string error = respuesta.substr(9);
        cout << "[ERROR] " << error << endl;
        return false;
    }
    else if (respuesta.find("409|ERROR|") == 0) {
        string error = respuesta.substr(9);
        cout << "[ERROR] " << error << endl;
        return false;
    }
    else {
        cout << "[ERROR] Respuesta desconocida: " << respuesta << endl;
        return false;
    }
}

vector<ReservaInfo> Cache::getMisReservas(int idCliente) {
    if (reservasPorCliente.find(idCliente) != reservasPorCliente.end()) {
        return reservasPorCliente[idCliente];
    }
    
    stringstream comando;
    comando << "MIS_RESERVAS|" << idCliente;
    string respuesta = cliente.enviarComando(comando.str());
    
    vector<ReservaInfo> reservas;
    
    if (respuesta.find("200|OK|") != 0) {
        if (respuesta.find("400|ERROR|") == 0) {
            string error = respuesta.substr(9);
            cout << "[ERROR] " << error << endl;
        } else {
            cout << "[ERROR] Error al obtener reservas: " << respuesta << endl;
        }
        return reservas;
    }
        
    string datos = respuesta.substr(7);
    if (datos.empty()) {
        return reservas;
    }
    
    stringstream ss(datos);
    string item;
    
    while (getline(ss, item, ';')) {
        if (item.empty()) continue;
        
        ReservaInfo r;
        stringstream item_ss(item);
        string campo;
        
        getline(item_ss, campo, ',');
        r.id = atoi(campo.c_str());
        getline(item_ss, r.fecha, ',');
        getline(item_ss, r.hora, ',');
        getline(item_ss, campo, ',');
        r.idServicio = atoi(campo.c_str());
        
        ServicioInfo s = buscarServicio(r.idServicio);
        r.nombreServicio = s.nombre;
        
        reservas.push_back(r);
    }
    
    reservasPorCliente[idCliente] = reservas;
    return reservas;
}

bool Cache::cancelarReserva(int idReserva) {
    stringstream comando;
    comando << "CANCELAR_RESERVA|" << idReserva;
    
    string respuesta = cliente.enviarComando(comando.str());
    
    if (respuesta.find("200|OK|") == 0) {
        cout << "[OK] Reserva cancelada correctamente" << endl;
        for (auto& par : reservasPorCliente) {
            for (auto it = par.second.begin(); it != par.second.end(); ++it) {
                if (it->id == idReserva) {
                    par.second.erase(it);
                    return true;
                }
            }
        }
        return true;
    }
    else if (respuesta.find("404|ERROR|") == 0) {
        string error = respuesta.substr(9);
        cout << "[ERROR] " << error << endl;
        return false;
    }
    else {
        cout << "[ERROR] " << respuesta << endl;
        return false;
    }
}

vector<string> Cache::getHorarios(const string& fecha) {
    if (horariosCache.find(fecha) != horariosCache.end()) {
        return horariosCache[fecha];
    }
    
    stringstream comando;
    comando << "GET_HORARIOS|" << fecha;
    string respuesta = cliente.enviarComando(comando.str());
    
    vector<string> horarios;
    
    if (respuesta.find("200|OK|") != 0) {
        if (respuesta.find("400|ERROR|") == 0) {
            string error = respuesta.substr(9);
            cout << "[ERROR] " << error << endl;
        } else {
            cout << "[ERROR] Error al obtener horarios: " << respuesta << endl;
        }
        return horarios;
    }
    
    string datos = respuesta.substr(7);
    stringstream ss(datos);
    string horario;
    
    while (getline(ss, horario, ',')) {
        if (!horario.empty()) {
            horarios.push_back(horario);
        }
    }
    
    if (horarios.empty()) {
        cout << "[INFO] No hay horarios disponibles para " << fecha << endl;
    }
    
    horariosCache[fecha] = horarios;
    return horarios;
}