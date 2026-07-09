#include <stdio.h>
#include "jardim.h"

EstadoCanteiro estado_canteiro[N_CANTEIROS];

int main() {
    estado_canteiro[0] = SEMENTE;
    estado_canteiro[1] = CRESCENDO;
    estado_canteiro[2] = FLORIDA;
    estado_canteiro[3] = MURCHA;
    estado_canteiro[4] = SEMENTE;
    imprimeJardim();

    return 0;
}