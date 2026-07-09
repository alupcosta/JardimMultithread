#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "jardim.h"

// Imprime a interface no terminal
void imprimeJardim() {
    sem_wait(&mutex_estado); // Trava acesso
    
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
    
    sem_post(&mutex_estado); // Libera acesso
}

// Controla o ciclo de vida das flores
void* flor_thread(void* arg) {
    int id = *(int*)arg;
    
    while (1) {
        // Semente
        sem_wait(&mutex_estado);
        estado_canteiro[id] = SEMENTE;
        sem_post(&mutex_estado);
        imprimeJardim();
        sleep(rand() % 3 + 1); // Espera 1 a 3s

        // Crescendo
        sem_wait(&mutex_estado);
        estado_canteiro[id] = CRESCENDO;
        sem_post(&mutex_estado);
        imprimeJardim();
        sleep(rand() % 3 + 2); // Espera 2 a 4s

        // Florida
        sem_wait(&mutex_estado);
        estado_canteiro[id] = FLORIDA;
        sem_post(&mutex_estado);
        imprimeJardim();
        sleep(rand() % 3 + 2); // Mantém florida antes de reiniciar
    }
    return NULL;
}