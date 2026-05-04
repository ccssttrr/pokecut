#ifndef SERVIDOR_PROTOCOLO_H
#define SERVIDOR_PROTOCOLO_H

void atenderCliente(int socket_cliente);
char* procesarComando(const char* comando);

char* cmd_getClientes();
char* cmd_getPeluqueras();
char* cmd_getServicios();
char* cmd_getHorarios(const char* fecha);
char* cmd_crearReserva(int id_cliente, int id_peluquera, int id_servicio, const char* fecha, const char* hora);
char* cmd_misReservas(int id_cliente);
char* cmd_cancelarReserva(int id_reserva);
char* cmd_login(const char* usuario, const char* pass);

#endif