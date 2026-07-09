#include <stdio.h>
#include <stdlib.h>
#include "jardim.h"

void imprimeJardim() {
    system("clear"); 
    printf("JARDIM\n\n");  
    printf("  C00   C01   C02   C03   C04\n");
    for (int i = 0; i < N_CANTEIROS; i++) {
        switch (estado_canteiro[i]) {
            case SEMENTE:   printf("[   ] "); break;
            case CRESCENDO: printf("[ . ] "); break;
            case FLORIDA:   printf("[ * ] "); break;
            case MURCHA:    printf("[ x ] "); break;
        }
    }
    printf("\n\n");
    printf("Legenda: [   ] Semente | [ . ] Crescendo | [ * ] Florida | [ x ] Murcha\n\n");
}