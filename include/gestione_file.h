#ifndef GESTIONE_FILE_H
#define GESTIONE_FILE_H

#include "interventi.h"

void caricaTecniciDaFile(const char* nomeFile, NodoTecnico** listaTecnici);
void caricaRichiesteDaFile(const char* nomeFile, NodoRichiesta* codeRichieste[]);

#endif