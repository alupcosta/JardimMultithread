#ifndef JARDIM_H
#define JARDIM_H

#include <pthread.h>
#include <semaphore.h>

#define N_CANTEIROS 5 
#define CAP_CESTA 3

typedef enum {
    SEMENTE,
    CRESCENDO,
    FLORIDA,
    MURCHA
} EstadoCanteiro;

extern EstadoCanteiro estado_canteiro[N_CANTEIROS];
extern int cesta_atual;
extern int deposito_total;
extern sem_t mutex_estado;
extern sem_t precisa_atencao;

void imprimeJardim();
void* flor_thread(void* arg);
void* jardineiro_thread(void* arg);

#endif