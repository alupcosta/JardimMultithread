#ifndef JARDIM_H
#define JARDIM_H

#include <pthread.h>
#include <semaphore.h>

#define N_CANTEIROS 5 

typedef enum {
    SEMENTE,
    CRESCENDO,
    FLORIDA,
    MURCHA
} EstadoCanteiro;

extern EstadoCanteiro estado_canteiro[N_CANTEIROS];
extern sem_t mutex_estado;

void imprimeJardim();
void* flor_thread(void* arg);

#endif