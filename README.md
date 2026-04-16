# pokecut

Proyecto de Programacion IV.

Requiere MinGW instalado.

Importante para que compile correctamente es necesario añadir -D_WIN32_WINNT=0x0601 al inicio del gcc.
El gcc deberia de quedar de esta manera: gcc -D_WIN32_WINNT=0x0601 basededatos.c clientes.c config.c ficheros.c informes.c main.c menu.c peluqueras.c reservas.c servicios.c sistema.c sqlite3.c -o pelu
