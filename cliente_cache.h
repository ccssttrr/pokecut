#ifndef CLIENTE_CACHE_H
#define CLIENTE_CACHE_H

#include <string>
#include <vector>
#include <map>

using namespace std;

struct ClienteInfo {
    int id;
    string nombre;
    string telefono;
};

struct PeluqueraInfo {
    int id;
    string nombre;
    string especialidad;
    string telefono;
    float horasTrabajadas;
};

struct ServicioInfo {
    int id;
    string nombre;
    string descripcion;
    int duracion;
    float precio;
};

struct ReservaInfo {
    int id;
    string fecha;
    string hora;
    int idServicio;
    string nombreServicio;
};

class Cache {
private:
    vector<ClienteInfo> clientes;
    vector<PeluqueraInfo> peluqueras;
    vector<ServicioInfo> servicios;
    map<int, vector<ReservaInfo>> reservasPorCliente;
    map<string, vector<string>> horariosCache;
    
    bool clientesCargados;
    bool peluquerasCargados;
    bool serviciosCargados;
    
public:
    Cache();
    ~Cache();
    
    void cargarClientes();
    void cargarPeluqueras();
    void cargarServicios();
    
    const vector<ClienteInfo>& getClientes() const;
    const vector<PeluqueraInfo>& getPeluqueras() const;
    const vector<ServicioInfo>& getServicios() const;
    
    ClienteInfo buscarCliente(int id) const;
    PeluqueraInfo buscarPeluquera(int id) const;
    ServicioInfo buscarServicio(int id) const;
    
    bool crearReserva(int idCliente, int idPeluquera, int idServicio, const string& fecha, const string& hora);
    vector<ReservaInfo> getMisReservas(int idCliente);
    bool cancelarReserva(int idReserva);
    
    vector<string> getHorarios(const string& fecha);
    
    void limpiarCache();
    
    bool hayClientesEnCache() const { return clientesCargados && !clientes.empty(); }
    bool hayPeluquerasEnCache() const { return peluquerasCargados && !peluqueras.empty(); }
    bool hayServiciosEnCache() const { return serviciosCargados && !servicios.empty(); }
};

#endif