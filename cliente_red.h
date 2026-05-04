#ifndef CLIENTE_RED_H
#define CLIENTE_RED_H

#include <string>

class ClienteRed {
private:
    int socket_fd;
    bool conectado;
    std::string ultimoError;
    
public:
    ClienteRed();
    ~ClienteRed();
    
    bool conectar(const std::string& ip, int puerto);
    void desconectar();
    bool estaConectado() const;
    
    std::string enviarComando(const std::string& comando);
    std::string getUltimoError() const;
};

#endif