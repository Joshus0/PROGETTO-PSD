#include "interventi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct NodoAgenda {
    int codiceRichiesta;
    char data[11];
    int fasciaOraria;
    struct NodoAgenda* next;
};

struct NodoTecnico {
    int idTecnico;
    char nome[MAX_STR];
    char specializzazione[MAX_STR];
    int caricoLavoro;
    NodoAgenda* agenda;
    struct NodoTecnico* next;
};

struct NodoRichiesta {
    int codiceRichiesta;
    char appartamento[MAX_STR];
    char tipologia[MAX_STR];
    char descrizione[255];
    char dataRichiesta[11];
    int urgenza;
    StatoRichiesta stato;
    struct NodoRichiesta* next;
};

void inizializzaSistema(NodoRichiesta* code[], NodoTecnico** listaT) {
    int i;
    for (i = 0; i < LIVELLI_URGENZA; i++) {
        code[i] = NULL;
    }
    *listaT = NULL;
}

void inserisciRichiesta(NodoRichiesta* code[], int urgenza, char* app, char* tipo, char* desc, char* data) {
    static int counterRichieste = 1; 
    
    NodoRichiesta* nuovo = (NodoRichiesta*)malloc(sizeof(NodoRichiesta));
    
    if (nuovo != NULL) {
        nuovo->codiceRichiesta = counterRichieste++;
        strcpy(nuovo->appartamento, app);
        strcpy(nuovo->tipologia, tipo);
        strcpy(nuovo->descrizione, desc);
        strcpy(nuovo->dataRichiesta, data);
        nuovo->urgenza = urgenza;
        nuovo->stato = Aperta;
        
        nuovo->next = code[urgenza];
        code[urgenza] = nuovo;
    }
}

void inserisciTecnico(NodoTecnico** testa, int id, char* nome, char* spec) {
    NodoTecnico* nuovo = (NodoTecnico*)malloc(sizeof(NodoTecnico));
    
    if (nuovo != NULL) {
        nuovo->idTecnico = id;
        strcpy(nuovo->nome, nome);
        strcpy(nuovo->specializzazione, spec);
        nuovo->caricoLavoro = 0;
        nuovo->agenda = NULL;
        
        nuovo->next = *testa;
        *testa = nuovo;
    }
}

int getCodiceRichiesta(NodoRichiesta* req) {
    if (req != NULL) return req->codiceRichiesta;
    return -1;
}

StatoRichiesta getStatoRichiesta(NodoRichiesta* req) {
    if (req != NULL) return req->stato;
    return Annullata; 
}

void setStatoRichiesta(NodoRichiesta* req, StatoRichiesta nuovoStato) {
    if (req != NULL) {
        req->stato = nuovoStato;
    }
}

int getIdTecnico(NodoTecnico* t) {
    if (t != NULL) return t->idTecnico;
    return -1;
}

char* getNomeTecnico(NodoTecnico* t) {
    if (t != NULL) return t->nome;
    return NULL;
}

int getCaricoLavoro(NodoTecnico* t) {
    if (t != NULL) return t->caricoLavoro;
    return 0;
}