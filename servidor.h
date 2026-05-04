#ifndef SERVIDOR_H
#define SERVIDOR_H

#define PUERTO_DEFECTO 8080
#define BUFFER_SIZE 4096

extern int ejecutando;

void manejador_salida(int sig);
int iniciarServidor();
void atenderCliente(int socket_cliente);

#endif