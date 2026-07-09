#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "jardim.h"

EstadoCanteiro estado_canteiro[N_CANTEIROS];
int cesta_atual = 0;
int deposito_total = 0;
sem_t mutex_estado;
sem_t precisa_atencao;

int main() {
    srand(time(NULL)); // Inicializa gerador de números aleatórios
    pthread_t threads_flores[N_CANTEIROS];
    pthread_t thread_jardineiro;
    int ids_canteiros[N_CANTEIROS];

    sem_init(&mutex_estado, 0, 1); // Mutex inicia livre (1)
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

    // Inicia a thread do jardineiro
    pthread_create(&thread_jardineiro, NULL, jardineiro_thread, NULL);

    // Mantém o programa rodando
    for (int i = 0; i < N_CANTEIROS; i++) {
        pthread_join(threads_flores[i], NULL);
    }
    pthread_join(thread_jardineiro, NULL);

    // Limpeza
    sem_destroy(&mutex_estado);
    sem_destroy(&precisa_atencao);

    return 0;
}