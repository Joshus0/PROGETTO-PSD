#include "interventi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ADT del progetto */
/* ------------------------------------------------------------------------------------- */

struct NodoAgenda {          /* ? Rappresenta un singolo appuntamento (Richiesta -> Tecnico) */
    int codiceRichiesta;    
    char data[11];
    int fasciaOraria;
    struct NodoAgenda* next;
};

struct NodoTecnico {               /* ? Profilo del tecnico */
    int idTecnico;
    char nome[MAX_STR];
    char specializzazione[MAX_STR];
    int caricoLavoro;
    NodoAgenda* agenda;      /* Puntatore alla lista personale di impegni del tecnico */
    struct NodoTecnico* next;
};

struct NodoRichiesta {          /* ? Singolo Ticket di guasto */
    int codiceRichiesta;
    char appartamento[MAX_STR];
    char tipologia[MAX_STR];
    char descrizione[255];
    char dataRichiesta[11];
    char dataChiusura[11]; 
    int urgenza;
    StatoRichiesta stato;
    struct NodoRichiesta* next;
};
/* ------------------------------------------------------------------------------------- */


/* ? Inizializza a NULL:
   ?         - Il vettore delle urgenze che conterra' tutte le richieste
   ?         - La testa della lista dei tecnici
*/
void inizializzaSistema(NodoRichiesta* code[], NodoTecnico** listaT) {
    int i;
    for (i = 0; i < LIVELLI_URGENZA; i++) {
        code[i] = NULL;
    }
    *listaT = NULL;
}


/* ? Inserisce una richiesta ordinata per urgenza */
void inserisciRichiesta(NodoRichiesta* code[], int urgenza, char* app, char* tipo, char* desc, char* data) {

    static int counterRichieste = 1; /* Salviamo questo valore per avere codici richiesta sempre differenti */
    
    NodoRichiesta* nuovo = (NodoRichiesta*)malloc(sizeof(NodoRichiesta));
    
    if (nuovo != NULL) {
        nuovo->codiceRichiesta = counterRichieste++;
        strcpy(nuovo->appartamento, app);
        strcpy(nuovo->tipologia, tipo);
        strcpy(nuovo->descrizione, desc);
        strcpy(nuovo->dataRichiesta, data);
        strcpy(nuovo->dataChiusura, "");
        nuovo->urgenza = urgenza;
        nuovo->stato = Aperta;
        
        nuovo->next = code[urgenza]; /* Il "next" della nuova richiesta puntera' all'elemento in testa all'array */
        code[urgenza] = nuovo;       /* La nuova richiesta viene messa in testa all'array */
    }
}


/* ? Doppio puntatore al NodoTecnico per prendere l'indirizzo dell'elemento in testa */
void inserisciTecnico(NodoTecnico** testa, int id, char* nome, char* spec) {
    NodoTecnico* nuovo = (NodoTecnico*)malloc(sizeof(NodoTecnico));
    
    if (nuovo != NULL) {
        nuovo->idTecnico = id;
        strcpy(nuovo->nome, nome);
        strcpy(nuovo->specializzazione, spec);
        nuovo->caricoLavoro = 0;
        nuovo->agenda = NULL;
        
        nuovo->next = *testa; /* Il "next" del nuovo tecnico puntera' al primo tecnico in testa */
        *testa = nuovo;       /* La lista aggiorna il nuovo tecnico in testa */
    }
}


/* * GETTER E SETTER (Information Hiding) */
/* ------------------------------------------------------------------------------------ */
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
/* ------------------------------------------------------------------------------------ */


/* ? Scorre l'agenda del tecnico per evitare sovrapposizioni */
int verificaConflitto(NodoTecnico* t, char* data, int fasciaOraria) {
    NodoAgenda* curr;
    
    if (t == NULL) return 0;
    
    curr = t->agenda; /* variabile di supporto per non perdere la testa (letteralmente) */
    while (curr != NULL) {
        /* Perche' si verifichi un conflitto dobbiamo confrontare data e ora contemporaneamente */
        if (strcmp(curr->data, data) == 0 && curr->fasciaOraria == fasciaOraria) {
            return 1; 
        }
        curr = curr->next;
    }
    return 0; 
}


/* ? Aggiunge un impegno all'agenda del tecnico se non ci sono conflitti */
int aggiungiImpegnoAgenda(NodoTecnico* t, int codiceRichiesta, char* data, int fasciaOraria) {
    NodoAgenda* nuovo;
    
    if (t == NULL) return 0;

    if (verificaConflitto(t, data, fasciaOraria) == 1) {
        return 0; 
    }

    nuovo = (NodoAgenda*)malloc(sizeof(NodoAgenda));
    if (nuovo != NULL) {
        nuovo->codiceRichiesta = codiceRichiesta;
        strcpy(nuovo->data, data);
        nuovo->fasciaOraria = fasciaOraria;
        
        nuovo->next = t->agenda;
        t->agenda = nuovo;
        
        t->caricoLavoro++; /* Incrementiamo il carico per tracciare l'attivita' del tecnico */
        
        return 1; 
    }
    return 0;
}


/* ? Funzione di diagnostica: stampa l'intera memoria dinamica e le relazioni tra nodi */
void stampaStatoGlobale(NodoRichiesta* code[], NodoTecnico* listaT) {
    int i;
    NodoRichiesta* currR;
    NodoTecnico* currT;
    NodoAgenda* currA;

    printf("\n###########################################################\n");
    printf("            REPORT COMPLETO DELLO STATO DEL SISTEMA          \n");
    printf("###########################################################\n");

    /* * SEZIONE TECNICI: Scorre la lista e le agende collegate */
    printf("\n--- DATABASE TECNICI ---\n");
    currT = listaT;
    while (currT != NULL) {
        printf("[ID UNICO: %d] Nome: %s | Spec: %s | Carico: %d\n", 
               currT->idTecnico, currT->nome, currT->specializzazione, currT->caricoLavoro);
        
        if (currT->agenda != NULL) {
            printf("  -> AGENDA IMPEGNI:\n");
            currA = currT->agenda;
            while (currA != NULL) {
                printf("     * Data: %s | Fascia: %d | Collegato a Richiesta ID: %d\n", 
                       currA->data, currA->fasciaOraria, currA->codiceRichiesta);
                currA = currA->next;
            }
        } else {
            printf("  -> Agenda vuota (Nessun impegno pianificato).\n");
        }
        currT = currT->next;
        printf("-----------------------------------------------------------\n");
    }

    /* * SEZIONE RICHIESTE: Scorre l'array delle urgenze */
    printf("\n--- DATABASE RICHIESTE (DIVISE PER URGENZA) ---\n");
    for (i = LIVELLI_URGENZA - 1; i >= 0; i--) { /* Stampiamo partendo dalla massima urgenza */
        currR = code[i];
        if (currR != NULL) {
            printf("[LIVELLO URGENZA %d]\n", i);
            while (currR != NULL) {
                printf("  [CODICE UNICO: %d]\n", currR->codiceRichiesta);
                printf("  Appartamento: %s\n", currR->appartamento);
                printf("  Tipologia: %s\n", currR->tipologia);
                printf("  Aperta il: %s | Chiusa il: %s\n", currR->dataRichiesta, currR->dataChiusura);
                printf("  Stato attuale: %d\n", currR->stato);
                printf("  ---------------------------------\n");
                currR = currR->next;
            }
        }
    }
    printf("###########################################################\n\n");
}