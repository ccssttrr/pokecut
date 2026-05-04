#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include "servidor_log.h"

static FILE* log_file = NULL;

void inicializarLog() {
    log_file = fopen("servidor.log", "a");
    if (!log_file) {
        printf("No se pudo abrir archivo de log\n");
    }
}

void escribirLog(const char* formato, ...) {
    if (!log_file) return;
    
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    
    fprintf(log_file, "[%04d-%02d-%02d %02d:%02d:%02d] ",
            tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
            tm->tm_hour, tm->tm_min, tm->tm_sec);
    
    va_list args;
    va_start(args, formato);
    vfprintf(log_file, formato, args);
    va_end(args);
    
    fprintf(log_file, "\n");
    fflush(log_file);
}

void cerrarLog() {
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}