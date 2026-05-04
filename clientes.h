#ifndef CLIENTES_H
#define CLIENTES_H

typedef struct {
    int  id;
    char nombre[50];
    char telefono[20];
    char email[100];
} Cliente;

extern Cliente *clientes;
extern int numClientes;
extern int capacidadClientes;

void menuClientes();
void inicializarClientes();
void altaCliente();
void buscarCliente();
void listarClientes();
void modificarCliente();

void guardarCliente(Cliente c);   // guarda uno solo
void guardarClientes();           // guarda todos
void cargarClientes();
void liberarClientes();

#endif
