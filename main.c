#include "sistema.h"
#include "basededatos.h"
#include "menu.h"
#include "config.h"
#include <stdlib.h>

int main(){
    
    inicializarSistema();
    inicializarBD();
    cargarBD();
    
    if (!login()) {
        cerrarBD();
        return 0;
    }
    menuPrincipal();

    guardarBD();
    cerrarSistema();

    return 0;
}