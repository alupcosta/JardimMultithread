#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "jardim.h"

EstadoCanteiro estado_canteiro[N_CANTEIROS];
int cesta_atual[N_JARDINEIROS] = {0};
int deposito_total = 0;
sem_t mutex_estado;
sem_t mutex_tesoura;
sem_t precisa_atencao;

int main() {
    srand(time(NULL)); // Inicializa gerador de números aleatórios
    pthread_t threads_flores[N_CANTEIROS];
    pthread_t threads_jardineiros[N_JARDINEIROS];
    int ids_canteiros[N_CANTEIROS];
    int ids_jardineiros[N_JARDINEIROS];

    sem_init(&mutex_estado, 0, 1); // Mutex inicia livre (1)
    sem_init(&mutex_tesoura, 0, 1); // Mutex inicia livre (1)
    sem_init(&precisa_atencao, 0, 0); // Não há flores floridas inicialmente (0)

    // Setup inicial dos canteiros
    for (int i = 0; i < N_CANTEIROS; i++) {
        estado_canteiro[i] = SEMENTE;
        ids_canteiros[i] = i;
    }
    imprimeJardim(); // Mostra jardim vazio

    // Inicia as threads das flores
    for (int i = 0; i < N_CANTEIROS; i++) {
        pthread_create(&threads_flores[i], NULL, flor_thread, &ids_canteiros[i]);
    }

    // Inicia as threads dos jardineiros
    for (int i = 0; i < N_JARDINEIROS; i++) {
        ids_jardineiros[i] = i;
        pthread_create(&threads_jardineiros[i], NULL, jardineiro_thread, &ids_jardineiros[i]);
    }

    // Mantém o programa rodando
    for (int i = 0; i < N_CANTEIROS; i++) {
        pthread_join(threads_flores[i], NULL);
    }
    for (int i = 0; i < N_JARDINEIROS; i++) {
        pthread_join(threads_jardineiros[i], NULL);
    }

    // Limpeza
    sem_destroy(&mutex_estado);
    sem_destroy(&precisa_atencao);
    sem_destroy(&mutex_tesoura);

    return 0;
}