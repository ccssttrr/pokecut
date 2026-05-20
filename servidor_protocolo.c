#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "servidor_protocolo.h"
#include "servidor_red.h"
#include "servidor_log.h"
#include "config.h"
#include "clientes.h"
#include "peluqueras.h"
#include "servicios.h"
#include "reservas.h"

void atenderCliente(int socket_cliente) {
    while (1) {
        char* comando = recibirComando(socket_cliente);
        if (comando == NULL) break;
        
        //para ignorar comandos vacios y que no salte el error
        if (strlen(comando) == 0) {
            free(comando);
            continue;
        }
        
        printf("Comando: %s\n", comando);
        escribirLog("Comando recibido: %s", comando);
        
        char* respuesta = procesarComando(comando);
        enviarRespuesta(socket_cliente, respuesta);
        
        free(respuesta);
        free(comando);
    }
}


char* procesarComando(const char* comando) {
    printf("DEBUG: Comando recibido: '%s'\n", comando);
    escribirLog("Comando recibido: %s", comando);
    
    char copia[512];
    strcpy(copia, comando);
    
    char* cmd = strtok(copia, "|");
    
    if (cmd == NULL) {
        printf("DEBUG: Comando vacío o NULL\n");
        return strdup("400|ERROR|Comando vacío");
    }
    
    printf("DEBUG: Comando parseado: '%s'\n", cmd);
    
    if (strcmp(cmd, "GET_CLIENTES") == 0) {
        return cmd_getClientes();
    }
    else if (strcmp(cmd, "GET_PELUQUERAS") == 0) {
        printf("DEBUG: Procesando GET_PELUQUERAS\n");
        return cmd_getPeluqueras();
    }
    else if (strcmp(cmd, "GET_SERVICIOS") == 0) {
        return cmd_getServicios();
    }
    else if (strcmp(cmd, "GET_HORARIOS") == 0) {
        char* fecha = strtok(NULL, "|");
        if (!fecha) return strdup("400|ERROR|Falta fecha");
        return cmd_getHorarios(fecha);
    }
    else if (strcmp(cmd, "CREAR_RESERVA") == 0) {
        char* id_cliente = strtok(NULL, "|");
        char* id_peluquera = strtok(NULL, "|");
        char* id_servicio = strtok(NULL, "|");
        char* fecha = strtok(NULL, "|");
        char* hora = strtok(NULL, "|");
        
        if (!id_cliente || !id_peluquera || !id_servicio || !fecha || !hora) {
            return strdup("400|ERROR|Faltan parámetros");
        }
        return cmd_crearReserva(atoi(id_cliente), atoi(id_peluquera),  atoi(id_servicio), fecha, hora);
    }
    else if (strcmp(cmd, "MIS_RESERVAS") == 0) {
        char* id_cliente = strtok(NULL, "|");
        if (!id_cliente) return strdup("400|ERROR|Falta ID cliente");
        return cmd_misReservas(atoi(id_cliente));
    }
    else if (strcmp(cmd, "CANCELAR_RESERVA") == 0) {
        char* id_reserva = strtok(NULL, "|");
        if (!id_reserva) return strdup("400|ERROR|Falta ID reserva");
        return cmd_cancelarReserva(atoi(id_reserva));
    }
    else if (strcmp(cmd, "LOGIN") == 0) {
        char* usuario = strtok(NULL, "|");
        char* pass = strtok(NULL, "|");
        if (!usuario || !pass) return strdup("400|ERROR|Faltan credenciales");
        return cmd_login(usuario, pass);
    }
    else if (strcmp(cmd, "PING") == 0) {
        return strdup("200|OK|pong");
    }
    else {
        char* resp = malloc(100);
        sprintf(resp, "400|ERROR|Comando desconocido: %s", cmd);
        return resp;
    }
}

char* cmd_getClientes() {
    if (numClientes == 0) {
        return strdup("200|OK|");  //vacio pero OK
    }
    
    //para calcular el tamaño que haga falta
    size_t tam = 50;  //"200|OK|" + espacio que haga falta
    for (int i = 0; i < numClientes; i++) {
        tam += strlen(clientes[i].nombre) + strlen(clientes[i].telefono) + 30;
    }
    
    char* buffer = (char*)malloc(tam);
    if (!buffer) return strdup("500|ERROR|Memoria insuficiente");
    
    strcpy(buffer, "200|OK|");
    
    for (int i = 0; i < numClientes; i++) {
        char temp[256];
        sprintf(temp, "%d,%s,%s;", 
                clientes[i].id, 
                clientes[i].nombre, 
                clientes[i].telefono);
        strcat(buffer, temp);
    }
    
    return buffer;
}

char* cmd_getPeluqueras() {
    if (numPeluqueras == 0) {
        return strdup("200|OK|");
    }
    
    //calcular el tamaño necesario
    size_t tam = 50;
    for (int i = 0; i < numPeluqueras; i++) {
        tam += strlen(peluqueras[i].nombre) + strlen(peluqueras[i].especialidad) + strlen(peluqueras[i].telefono) + 50;
    }
    
    char* buffer = (char*)malloc(tam);
    if (!buffer) return strdup("500|ERROR|Memoria insuficiente");
    
    strcpy(buffer, "200|OK|");
    
    for (int i = 0; i < numPeluqueras; i++) {
        char temp[512];
        sprintf(temp, "%d,%s,%s,%s,%.1f;", 
                peluqueras[i].id, 
                peluqueras[i].nombre, 
                peluqueras[i].especialidad,
                peluqueras[i].telefono,
                peluqueras[i].horasTrabajadas);
        strcat(buffer, temp);
    }
    
    return buffer;
}

char* cmd_getServicios() {
    if (numServicios == 0) {
        return strdup("200|OK|");
    }
    
    size_t tam = 50;
    for (int i = 0; i < numServicios; i++) {
        tam += strlen(servicios[i].nombre) + strlen(servicios[i].descripcion) + 50;
    }
    
    char* buffer = (char*)malloc(tam);
    if (!buffer) return strdup("500|ERROR|Memoria insuficiente");
    
    strcpy(buffer, "200|OK|");
    
    for (int i = 0; i < numServicios; i++) {
        char temp[512];
        sprintf(temp, "%d,%s,%s,%d,%.2f;", 
                servicios[i].id, 
                servicios[i].nombre, 
                servicios[i].descripcion,
                servicios[i].duracion,
                servicios[i].precio);
        strcat(buffer, temp);
    }
    
    return buffer;
}

char* cmd_getHorarios(const char* fecha) {
    (void)fecha;  // Evitar warning
    
    //por ahora devolvemos horarios fijos
    return strdup("200|OK|09:00,10:00,11:00,12:00,13:00,16:00,17:00,18:00,19:00");
}

char* cmd_crearReserva(int id_cliente, int id_peluquera, int id_servicio, const char* fecha, const char* hora) {
    //verificar que cliente, peluquera y servicio existen
    int cliente_ok = 0, peluquera_ok = 0, servicio_ok = 0;
    
    for (int i = 0; i < numClientes; i++) {
        if (clientes[i].id == id_cliente) { cliente_ok = 1; break; }
    }
    for (int i = 0; i < numPeluqueras; i++) {
        if (peluqueras[i].id == id_peluquera) { peluquera_ok = 1; break; }
    }
    for (int i = 0; i < numServicios; i++) {
        if (servicios[i].id == id_servicio) { servicio_ok = 1; break; }
    }
    
    if (!cliente_ok) return strdup("400|ERROR|Cliente no existe");
    if (!peluquera_ok) return strdup("400|ERROR|Peluquera no existe");
    if (!servicio_ok) return strdup("400|ERROR|Servicio no existe");
    
    //crear reserva
    //aqui se llamaria a la función de crear reserva de reservas.c
    
    char* respuesta = (char*)malloc(100);
    sprintf(respuesta, "200|OK|Reserva creada|%d", numReservas + 1);
    
    escribirLog("Reserva creada: Cliente=%d, Peluquera=%d, Servicio=%d, Fecha=%s, Hora=%s",
                id_cliente, id_peluquera, id_servicio, fecha, hora);
    
    return respuesta;
}

char* cmd_misReservas(int id_cliente) {
    char buffer[4096] = "200|OK|";
    char temp[256];
    int encontradas = 0;
    
    for (int i = 0; i < numReservas; i++) {
        if (reservas[i].idCliente == id_cliente) {
            sprintf(temp, "%d,%s,%s,%d;", 
                    reservas[i].idReserva,
                    reservas[i].fecha,
                    reservas[i].hora,
                    reservas[i].idServicio);
            strcat(buffer, temp);
            encontradas++;
        }
    }
    
    if (encontradas == 0) {
        return strdup("200|OK|");
    }
    
    return strdup(buffer);
}

char* cmd_cancelarReserva(int id_reserva) {
    for (int i = 0; i < numReservas; i++) {
        if (reservas[i].idReserva == id_reserva) {
            //eliminar reserva
            for (int j = i; j < numReservas - 1; j++) {
                reservas[j] = reservas[j + 1];
            }
            numReservas--;
            guardarReservas();
            escribirLog("Reserva %d cancelada", id_reserva);
            return strdup("200|OK|Reserva cancelada correctamente");
        }
    }
    
    return strdup("404|ERROR|Reserva no encontrada");
}

char* cmd_login(const char* usuario, const char* pass) {
    if (strcmp(usuario, config.adminUser) == 0 && 
        strcmp(pass, config.adminPass) == 0) {
        return strdup("200|OK|Login exitoso");
    }
    return strdup("401|ERROR|Usuario o contraseña incorrectos");
}