#ifndef JARDIM_H
#define JARDIM_H

#define N_CANTEIROS 5

typedef enum {
    SEMENTE,
    CRESCENDO,
    FLORIDA,
    MURCHA
} EstadoCanteiro;

extern EstadoCanteiro estado_canteiro[N_CANTEIROS];

void imprimeJardim();

#endif