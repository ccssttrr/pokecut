#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "peluqueras.h"
#include "basededatos.h"
#include "sqlite3.h"

Peluquera *peluqueras = NULL;
int numPeluqueras = 0;
int capacidadPeluqueras = 0;

//para la resta de las horas trabajadas
typedef struct {
    int idPeluquera;
    int horaEntrada;
    int minutoEntrada;
    int fichado;  //0 = no fichado, 1= fichado
} EstadoFichaje;

EstadoFichaje fichajes[100];
int numFichajes = 0;

void menuPeluqueras() {
    int op;
    int id;
    do {
        printf("\n--- GESTION DE PELUQUERAS ---\n");
        printf("1. Alta\n");
        printf("2. Buscar\n");
        printf("3. Modificar\n");
        printf("4. Listar\n");
        printf("5. Fichar entrada\n");
        printf("6. Fichar salida\n");
        printf("0. Volver\n");
        printf("Opcion: ");
        scanf("%d", &op);
        getchar();

        if (op == 1) altaPeluquera();
        else if (op == 2) buscarPeluquera();
        else if (op == 3) modificarPeluquera();
        else if (op == 4) listarPeluqueras();
        else if (op == 5) {
            printf("ID de la peluquera: ");
            scanf("%d", &id);
            getchar();
            ficharEntrada(id);
        }
        else if (op == 6) {
            printf("ID de la peluquera: ");
            scanf("%d", &id);
            getchar();
            ficharSalida(id);
        }


    } while (op != 0);
}

void inicializarPeluqueras() {
    capacidadPeluqueras = 10;
    numPeluqueras = 0;
    peluqueras = malloc(capacidadPeluqueras * sizeof(Peluquera));
    if (!peluqueras) { printf("Error de memoria\n"); exit(1); }
}

void altaPeluquera() {
    if (numPeluqueras >= capacidadPeluqueras) {
        capacidadPeluqueras *= 2;
        Peluquera *temp = realloc(peluqueras, capacidadPeluqueras * sizeof(Peluquera));
        if (!temp) { printf("Error de memoria\n"); exit(1); }
        peluqueras = temp;
    }

    Peluquera p;
    printf("ID: ");           scanf("%d",  &p.id);
    printf("Nombre: ");       scanf("%49s", p.nombre);
    printf("Especialidad: "); scanf("%49s", p.especialidad);
    printf("Telefono: ");     scanf("%19s", p.telefono);
    p.horasTrabajadas = 0;

    peluqueras[numPeluqueras++] = p;
    guardarPeluquera(p);
    printf("Peluquera registrada.\n");
}

void buscarPeluquera(){
    int id;
    printf("ID peluquera: ");
    scanf("%d", &id);
    getchar();
    
    for (int i = 0; i < numPeluqueras; i++){
        if (peluqueras[i].id == id) {
            printf("\n--- PELUQUERA ENCONTRADA ---\n");
            printf("ID: %d\n", peluqueras[i].id);
            printf("Nombre: %s\n", peluqueras[i].nombre);
            printf("Especialidad: %s\n", peluqueras[i].especialidad);
            printf("Telefono: %s\n", peluqueras[i].telefono);
            printf("Horas trabajadas: %.2f\n", peluqueras[i].horasTrabajadas);
            return;
        }
    }
    printf("Peluquera no encontrada.\n");
}

void listarPeluqueras(){
    if (numPeluqueras == 0){ 
        printf("No hay peluqueras \n"); 
        return; 
    }
    
    printf("\n--- LISTADO DE PELUQUERAS ---\n");
    for (int i = 0; i < numPeluqueras; i++) {
        printf("\n--- Peluquera %d ---\n", i+1);
        printf("ID: %d\n", peluqueras[i].id);
        printf("Nombre: %s\n", peluqueras[i].nombre);
        printf("Especialidad: %s\n", peluqueras[i].especialidad);
        printf("Telefono: %s\n", peluqueras[i].telefono);
        printf("Horas trabajadas: %.2f\n", peluqueras[i].horasTrabajadas);
    }
}

void modificarPeluquera(){
    int id;
    printf("ID peluquera: ");
    scanf("%d", &id);
    for (int i = 0; i < numPeluqueras; i++){
        if (peluqueras[i].id == id) {
            printf("Nuevo nombre: ");       scanf("%49s", peluqueras[i].nombre);
            printf("Nueva especialidad: "); scanf("%49s", peluqueras[i].especialidad);
            printf("Nuevo telefono: ");     scanf("%19s", peluqueras[i].telefono);
            guardarPeluquera(peluqueras[i]);
            printf("Peluquera modificada.\n");
            return;
        }
    }
    printf("No encontrada.\n");
}

int buscarFichajeActivo(int idPeluquera){
    for (int i = 0; i < numFichajes; i++){
        if (fichajes[i].idPeluquera == idPeluquera && fichajes[i].fichado == 1) {
            return i;
        }
    }
    return -1;
}

void obtenerHoraActual(int *hora, int *minuto){
    printf("Hora actual (hh mm): ");
    scanf("%d %d", hora, minuto);
    getchar();
}

float calcularHorasTrabajadas(int horaEntrada, int minEntrada, int horaSalida, int minSalida, int descansoMinutos){
    int totalMinutosEntrada = horaEntrada * 60 + minEntrada;
    int totalMinutosSalida = horaSalida * 60 + minSalida;
    
    int minutosTrabajados = totalMinutosSalida - totalMinutosEntrada - descansoMinutos;
    
    if (minutosTrabajados < 0) {
        printf("Error: La hora de salida es anterior a la de entrada\n");
        return 0;
    }
    
    return minutosTrabajados / 60.0;
}

void ficharEntrada(int id) {
    int encontrada = -1;
    for (int i = 0; i < numPeluqueras; i++) {
        if (peluqueras[i].id == id) {
            encontrada = i;
            break;
        }
    }
    
    if (encontrada == -1) {
        printf("Peluquera con ID %d no encontrada\n", id);
        return;
    }
    
    //verificar si ya tiene un fichaje activo
    if (buscarFichajeActivo(id) != -1) {
        printf("Error: La peluquera ya tiene una entrada registrada sin salida\n");
        return;
    }
    
    int hora, minuto;
    printf("--- FICHAJE DE ENTRADA ---\n");
    printf("Peluquera: %s (ID: %d)\n", peluqueras[encontrada].nombre, id);
    obtenerHoraActual(&hora, &minuto);
    
    if (numFichajes >= 100) {
        printf("Error: Demasiados fichajes activos\n");
        return;
    }
    
    fichajes[numFichajes].idPeluquera = id;
    fichajes[numFichajes].horaEntrada = hora;
    fichajes[numFichajes].minutoEntrada = minuto;
    fichajes[numFichajes].fichado = 1;
    numFichajes++;
    
    printf("Entrada registrada para %s a las %02d:%02d\n", peluqueras[encontrada].nombre, hora, minuto);
}

void ficharSalida(int id) {
    int encontrada = -1;
    for (int i = 0; i < numPeluqueras; i++) {
        if (peluqueras[i].id == id) {
            encontrada = i;
            break;
        }
    }
    
    if (encontrada == -1) {
        printf("Peluquera con ID %d no encontrada\n", id);
        return;
    }
    
    int idxFichaje = buscarFichajeActivo(id);
    if (idxFichaje == -1) {
        printf("Error: La peluquera no tiene una entrada registrada\n");
        return;
    }
    
    int hora, minuto;
    int descansoMinutos = 0;
    char descansoInput[10];
    
    printf("--- FICHAJE DE SALIDA ---\n");
    printf("Peluquera: %s (ID: %d)\n", peluqueras[encontrada].nombre, id);
    obtenerHoraActual(&hora, &minuto);
    
    //preguntar por tiempo de descanso
    printf("¿Ha tenido descanso? (s/n): ");
    fgets(descansoInput, sizeof(descansoInput), stdin);
    descansoInput[strcspn(descansoInput, "\n")] = '\0';
    
    if (strcmp(descansoInput, "s") == 0 || strcmp(descansoInput, "S") == 0) {
        int horasDescanso, minutosDescanso;
        printf("Tiempo de descanso (horas minutos): ");
        scanf("%d %d", &horasDescanso, &minutosDescanso);
        getchar();
        descansoMinutos = horasDescanso * 60 + minutosDescanso;
        printf("Descanso registrado: %d hora(s) y %d minuto(s)\n", horasDescanso, minutosDescanso);
    } else {
        printf("No se registró descanso\n");
    }
    
    //calculo de horas trabajadas
    float horasTrabajadas = calcularHorasTrabajadas(
        fichajes[idxFichaje].horaEntrada, 
        fichajes[idxFichaje].minutoEntrada,
        hora, minuto, 
        descansoMinutos
    );
    
    if (horasTrabajadas > 0) {
        peluqueras[encontrada].horasTrabajadas += horasTrabajadas;
        guardarPeluquera(peluqueras[encontrada]);
        
        printf("Salida registrada para %s\n", peluqueras[encontrada].nombre);
        printf("Hora entrada: %02d:%02d\n", fichajes[idxFichaje].horaEntrada, fichajes[idxFichaje].minutoEntrada);
        printf("Hora salida: %02d:%02d\n", hora, minuto);
        printf("Descanso: %d minuto(s)\n", descansoMinutos);
        printf("Horas trabajadas hoy: %.2f\n", horasTrabajadas);
        printf("Total horas acumuladas: %.2f\n", peluqueras[encontrada].horasTrabajadas);
    }
    
    for (int i = idxFichaje; i < numFichajes - 1; i++){
        fichajes[i] = fichajes[i + 1];
    }
    numFichajes--;
}

// ---- SQLite ----

void guardarPeluquera(Peluquera p) {
    if (!db) return;
    sqlite3_stmt *stmt;
    const char *sql =
        "INSERT OR REPLACE INTO peluqueras (id, nombre, especialidad, telefono, horas_trabajadas) "
        "VALUES (?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return;

    sqlite3_bind_int (stmt, 1, p.id);
    sqlite3_bind_text(stmt, 2, p.nombre, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, p.especialidad, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, p.telefono, -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 5, p.horasTrabajadas);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error guardando peluquera: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
}

void guardarPeluqueras() {
    for (int i = 0; i < numPeluqueras; i++)
        guardarPeluquera(peluqueras[i]);
}

static int callbackPeluquera(void *unused, int cols, char **valores, char **nombres) {
    (void)unused; (void)cols; (void)nombres;

    if (numPeluqueras >= capacidadPeluqueras) {
        capacidadPeluqueras *= 2;
        peluqueras = realloc(peluqueras, capacidadPeluqueras * sizeof(Peluquera));
    }

    Peluquera p;
    p.id = atoi(valores[0]);
    strncpy(p.nombre,       valores[1] ? valores[1] : "", 49);
    strncpy(p.especialidad, valores[2] ? valores[2] : "", 49);
    strncpy(p.telefono,     valores[3] ? valores[3] : "", 19);
    p.horasTrabajadas = valores[4] ? atof(valores[4]) : 0.0f;  // atof para float

    peluqueras[numPeluqueras++] = p;
    return 0;
}

void cargarPeluqueras() {
    if (!db) return;
    numPeluqueras = 0;
    char *err = NULL;
    sqlite3_exec(db,
        "SELECT id, nombre, especialidad, telefono, horas_trabajadas FROM peluqueras;",
        callbackPeluquera, NULL, &err);
    if (err) { printf("Error cargando peluqueras: %s\n", err); sqlite3_free(err); }
}

void liberarPeluqueras() {
    free(peluqueras);
    peluqueras = NULL;
    numPeluqueras = 0;
    capacidadPeluqueras = 0;
}