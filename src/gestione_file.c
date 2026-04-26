#include "../include/gestione_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void caricaTecniciDaFile(const char* nomeFile, NodoTecnico** listaTecnici) {
    FILE* file = fopen(nomeFile, "r");
    char buffer[256];//buffer per salvare temporaneamente
    char *idStr, *nome, *spec;

    if (file == NULL) {
        return;
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;//Rimuoviamo "\n" alla fine di "spec" e aggiungiamo il terminatore di stringa

        idStr = strtok(buffer, ";");//strtok taglia la stringa fino a ";"
        nome = strtok(NULL, ";");//Mettendo NULL come punto di partenza fa si che riprenda da dove si è fermato precedentemente
        spec = strtok(NULL, ";");

        if (idStr != NULL && nome != NULL && spec != NULL) {
            inserisciTecnico(listaTecnici, atoi(idStr), nome, spec); //atoi(idStr) serve per convertire esplicitamente da ASCII a INT
        }
    }
    
    fclose(file);
}


//? Come implementazione è identica alla funzione precedente

void caricaRichiesteDaFile(const char* nomeFile, NodoRichiesta* codeRichieste[]) {
    FILE* file = fopen(nomeFile, "r");
    char buffer[512];
    char *urgenzaStr, *app, *tipo, *desc, *data;

    if (file == NULL) {
        return;
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;

        urgenzaStr = strtok(buffer, ";");
        app = strtok(NULL, ";");
        tipo = strtok(NULL, ";");
        desc = strtok(NULL, ";");
        data = strtok(NULL, ";");

        if (urgenzaStr != NULL && app != NULL && tipo != NULL && desc != NULL && data != NULL) {
            inserisciRichiesta(codeRichieste, atoi(urgenzaStr), app, tipo, desc, data);
        }
    }
    
    fclose(file);
}