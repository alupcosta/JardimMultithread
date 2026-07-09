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
    
    printf("JARDINEIRO:\nCesta: [");
    for(int i = 0; i < CAP_CESTA; i++) {
        if(i < cesta_atual) printf("*");
        else printf(" ");
    }
    printf("] %d/%d\n", cesta_atual, CAP_CESTA);
    printf("DEPOSITO: %d flores guardadas\n", deposito_total);

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
        sem_post(&precisa_atencao); // Notifica jardineiro
        
        // Impede que o ciclo recomece antes de ser colhida
        while(estado_canteiro[id] == FLORIDA) { 
            usleep(500000);
        }
    }
    return NULL;
}

// Controla o jardineiro
void* jardineiro_thread(void* arg) {
    while(1) {
        sem_wait(&precisa_atencao); // Espera por uma flor florida
        sem_wait(&mutex_estado); // Trava acesso

        // Busca uma flor florida
        int alvo = -1;
        for (int i = 0; i < N_CANTEIROS; i++) {
            if (estado_canteiro[i] == FLORIDA) {
                alvo = i;
                break;
            }
        }

        sem_post(&mutex_estado); // Libera acesso

        if (alvo != -1) {
            sleep(1); // Tempo de colheita
            
            sem_wait(&mutex_estado); // Trava acesso

            estado_canteiro[alvo] = SEMENTE;
            cesta_atual++;

            sem_post(&mutex_estado); // Libera acesso
            imprimeJardim();

            // Se a cesta estiver cheia, deposita as flores
            if (cesta_atual == CAP_CESTA) {
                sleep(2); 
                sem_wait(&mutex_estado);
                deposito_total += cesta_atual;
                cesta_atual = 0;
                sem_post(&mutex_estado);
                imprimeJardim();
            }
        }
    }
    return NULL;
}