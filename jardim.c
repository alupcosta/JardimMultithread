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
    for (int i = 0; i < N_JARDINEIROS; i++) {
        printf("JARDINEIRO %d:\nCesta: [", i);
        for(int j = 0; j < CAP_CESTA; j++) {
            if(j < cesta_atual[i]) printf("*");
            else printf(" ");
        }
        printf("] %d/%d\n", cesta_atual[i], CAP_CESTA);
    }
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
        
        // Murchando
        sleep(TEMPO_MURCHAR);
        int murchou = 0;
        sem_wait(&mutex_estado); // Trava acesso
        if (estado_canteiro[id] == FLORIDA) {
            estado_canteiro[id] = MURCHA;
            murchou = 1;
        }
        sem_post(&mutex_estado); // Libera acesso
        if (murchou) imprimeJardim();

        // Impede que o ciclo recomece antes de ser colhida
        while(estado_canteiro[id] != SEMENTE) { 
            usleep(500000);
        }
    }
    return NULL;
}

// Controla o jardineiro
void* jardineiro_thread(void* arg) {
    int id_jardineiro = *(int*)arg;

    while(1) {
        sem_wait(&precisa_atencao); // Espera por uma flor florida
        sem_wait(&mutex_tesoura); // Trava acesso a tesoura
        sem_wait(&mutex_estado); // Trava acesso ao canteiro

        // Busca uma flor florida
        int alvo = -1;
        for (int i = 0; i < N_CANTEIROS; i++) {
            if (estado_canteiro[i] == FLORIDA || estado_canteiro[i] == MURCHA) {
                alvo = i;
                break;
            }
        }

        sem_post(&mutex_estado); // Libera acesso ao canteiro

        if (alvo != -1) {
            sleep(1); // Tempo de colheita
            sem_wait(&mutex_estado); // Trava acesso ao canteiro

            // Colhe a flor florida ou limpa a flor murcha
            if (estado_canteiro[alvo] == FLORIDA) {
                estado_canteiro[alvo] = SEMENTE;
                cesta_atual[id_jardineiro]++;
                sem_post(&mutex_estado); // Libera acesso ao canteiro
                imprimeJardim();

                // Se a cesta estiver cheia, deposita as flores
                if (cesta_atual[id_jardineiro] == CAP_CESTA) {
                    sleep(2); 
                    sem_wait(&mutex_estado);
                    deposito_total += cesta_atual[id_jardineiro];
                    cesta_atual[id_jardineiro] = 0;
                    sem_post(&mutex_estado); // Libera acesso ao canteiro
                    imprimeJardim();
                }
            } 
            else if (estado_canteiro[alvo] == MURCHA) {
                // Perdeu a flor. Apenas limpa o canteiro sem colocar na cesta.
                estado_canteiro[alvo] = SEMENTE;
                sem_post(&mutex_estado); // Libera acesso ao canteiro
                imprimeJardim();
            } 
            else {
                sem_post(&mutex_estado); // Libera acesso ao canteiro
            }
        }

        sem_post(&mutex_tesoura); // Libera acesso a tesoura
    }
    return NULL;
}