#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "jardim.h"

EstadoCanteiro estado_canteiro[N_CANTEIROS];
sem_t mutex_estado;

int main() {
    srand(time(NULL)); // Inicializa gerador de números aleatórios
    pthread_t threads_flores[N_CANTEIROS];
    int ids_canteiros[N_CANTEIROS];

    sem_init(&mutex_estado, 0, 1); // Mutex inicia livre (1)

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

    // Mantém o programa rodando
    for (int i = 0; i < N_CANTEIROS; i++) {
        pthread_join(threads_flores[i], NULL);
    }

    // Limpeza
    sem_destroy(&mutex_estado);

    return 0;
}