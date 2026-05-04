#ifndef SERVIDOR_LOG_H
#define SERVIDOR_LOG_H

void inicializarLog();
void escribirLog(const char* formato, ...);
void cerrarLog();

#endif