#include <iostream>
#include <cstring>
#include <sstream>
#include "cliente_red.h"

#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

using namespace std;

ClienteRed::ClienteRed() : socket_fd(-1), conectado(false) {
    #ifdef _WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
    #endif
}

ClienteRed::~ClienteRed() {
    desconectar();
    #ifdef _WIN32
        WSACleanup();
    #endif
}

bool ClienteRed::conectar(const string& ip, int puerto) {
    if (conectado) desconectar();
    
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        ultimoError = "Error al crear socket";
        return false;
    }
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(puerto);
    
    #ifdef _WIN32
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        if (addr.sin_addr.s_addr == INADDR_NONE) {
            ultimoError = "IP inválida";
            return false;
        }
    #else
        inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
    #endif
    
    if (connect(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        ultimoError = "Error al conectar al servidor";
        #ifdef _WIN32
            closesocket(socket_fd);
        #else
            close(socket_fd);
        #endif
        socket_fd = -1;
        return false;
    }
    
    conectado = true;
    return true;
}

void ClienteRed::desconectar() {
    if (socket_fd != -1) {
        #ifdef _WIN32
            closesocket(socket_fd);
        #else
            close(socket_fd);
        #endif
        socket_fd = -1;
    }
    conectado = false;
}

bool ClienteRed::estaConectado() const {
    return conectado;
}

string ClienteRed::enviarComando(const string& comando) {
    if (!conectado) {
        return "ERROR|No conectado al servidor";
    }
    
    #ifdef _WIN32
        send(socket_fd, comando.c_str(), (int)comando.length(), 0);
        send(socket_fd, "\n", 1, 0);
    #else
        write(socket_fd, comando.c_str(), comando.length());
        write(socket_fd, "\n", 1);
    #endif
    
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    
    #ifdef _WIN32
        int bytes = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
    #else
        int bytes = read(socket_fd, buffer, sizeof(buffer) - 1);
    #endif
    
    if (bytes <= 0) {
        return "ERROR|Servidor no responde";
    }
    
    string respuesta(buffer);
    while (!respuesta.empty() && (respuesta.back() == '\n' || respuesta.back() == '\r')) {
        respuesta.pop_back();
    }
    
    return respuesta;
}

string ClienteRed::getUltimoError() const {
    return ultimoError;
}