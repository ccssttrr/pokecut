#ifndef SERVIDOR_RED_H
#define SERVIDOR_RED_H

#define BUFFER_SIZE 4096

#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef int socklen_t;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

int crearSocketServidor(int puerto);
int aceptarCliente(int socket_servidor);
void cerrarSocketServidor(int socket_servidor);
void cerrarSocketCliente(int socket_cliente);
void enviarRespuesta(int socket_cliente, const char* respuesta);
char* recibirComando(int socket_cliente);

#endif