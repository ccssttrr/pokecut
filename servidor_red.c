#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "servidor_red.h"
#include "servidor_log.h"
#ifdef _WIN32
    typedef int socklen_t;
#endif

#ifdef _WIN32
static int iniciarWinsock() {
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData);
}
#endif

int crearSocketServidor(int puerto) {
    #ifdef _WIN32
        if (iniciarWinsock() != 0) {
            return -1;
        }
    #endif
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return -1;
    }
    
    int opt = 1;
    #ifdef _WIN32
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
    #else
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    #endif
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(puerto);
    
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        #ifdef _WIN32
            closesocket(sock);
        #else
            close(sock);
        #endif
        return -1;
    }
    
    if (listen(sock, 5) < 0) {
        #ifdef _WIN32
            closesocket(sock);
        #else
            close(sock);
        #endif
        return -1;
    }
    
    return sock;
}

int aceptarCliente(int socket_servidor) {
    struct sockaddr_in addr_cliente;
    socklen_t addr_len = sizeof(addr_cliente);
    
    return accept(socket_servidor, (struct sockaddr*)&addr_cliente, &addr_len);
}

void cerrarSocketServidor(int socket_servidor) {
    if (socket_servidor != -1) {
        #ifdef _WIN32
            closesocket(socket_servidor);
            WSACleanup();
        #else
            close(socket_servidor);
        #endif
    }
}

void cerrarSocketCliente(int socket_cliente) {
    if (socket_cliente != -1) {
        #ifdef _WIN32
            closesocket(socket_cliente);
        #else
            close(socket_cliente);
        #endif
    }
}

void enviarRespuesta(int socket_cliente, const char* respuesta) {
    #ifdef _WIN32
        send(socket_cliente, respuesta, strlen(respuesta), 0);
        send(socket_cliente, "\n", 1, 0);
    #else
        send(socket_cliente, respuesta, strlen(respuesta), 0);
        send(socket_cliente, "\n", 1, 0);
    #endif
}

char* recibirComando(int socket_cliente) {
    static char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    
    #ifdef _WIN32
        int bytes = recv(socket_cliente, buffer, sizeof(buffer) - 1, 0);
    #else
        int bytes = read(socket_cliente, buffer, sizeof(buffer) - 1);
    #endif
    
    if (bytes <= 0) return NULL;
    
    buffer[bytes] = '\0';
    //para eliminar salto de línea
    char* newline = strchr(buffer, '\n');
    if (newline) *newline = '\0';
    
    return buffer;
}