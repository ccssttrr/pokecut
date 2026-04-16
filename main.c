#include "sistema.h"
#include "basededatos.h"
#include "menu.h"
#include "config.h"

int main(){
    inicializarSistema();
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