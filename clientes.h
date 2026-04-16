#ifndef CLIENTES_H
#define CLIENTES_H

//aqui no usamos el define porque quiero usar un array dinamico para que no sea limitado los clientes

typedef struct {
    int id;
    char nombre[50];
    char telefono[20];

} Cliente;

//puntero global que sirve para el array dinamico, el extern es para decirle que existen pero para que no los cree aqui, se crean en el .c
extern Cliente *clientes;
extern int numClientes;
extern int capacidadClientes;

void menuClientes();
void inicializarClientes();
void altaCliente();
void buscarCliente();
void listarClientes();
void editarCliente();

void guardarClientes();
void cargarClientes();

int generarNuevoIdCliente();
int existeIdCliente(int id);

//memoria
void liberarClientes();

int existeCliente(int id);



#endif