#include "interventi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//*ADT del progetto
//-------------------------------------------------------------------------------------//

struct NodoAgenda {          //? Rappresenta un singolo appuntamento(Richiesta->Tecnico)
    int codiceRichiesta;    
    char data[11];
    int fasciaOraria;
    struct NodoAgenda* next;
};

struct NodoTecnico {               //? Profilo del tecnico
    int idTecnico;
    char nome[MAX_STR];
    char specializzazione[MAX_STR];
    int caricoLavoro;
    int disponibilita; /*1=disponibile, 0=non disponibile*/
    NodoAgenda* agenda;//Puntatore alla lista personale di impegni del tecnico
    struct NodoTecnico* next;
};

struct NodoRichiesta {          //? Singolo Ticket di guasto
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
//-------------------------------------------------------------------------------------//

//? Inizializza a NULL:
//?         - Il vettore delle urgenze che conterrà tutte le richieste
//?         -La testa della lista dei tecnici

void inizializzaSistema(NodoRichiesta* code[], NodoTecnico** listaT) {
    int i;
    for (i = 0; i < LIVELLI_URGENZA; i++) {
        code[i] = NULL;
    }
    *listaT = NULL;
}

//?Inserisce una richiesta ordinata per urgenza

void inserisciRichiesta(NodoRichiesta* code[], int urgenza, char* app, char* tipo, char* desc, char* data) {

    static int counterRichieste = 1; //Salviamo questo valore per avere codici richiesta sempre differenti
    
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
        
        nuovo->next = code[urgenza];//Il "next" della nuova richiesta punterà all'elemento in testa all'array di rispettiva urgenza
        code[urgenza] = nuovo;//La nuova richiesta viene messa in testa all'array
    }
}

void inserisciTecnico(NodoTecnico** testa, int id, char* nome, char* spec) {//Doppio puntatore al NodoTecnico per prendere l'indirizzo dell'elemento in testa
    NodoTecnico* nuovo = (NodoTecnico*)malloc(sizeof(NodoTecnico));
    
    if (nuovo != NULL) {
        nuovo->idTecnico = id;
        strcpy(nuovo->nome, nome);
        strcpy(nuovo->specializzazione, spec);
        nuovo->disponibilita = 1; /* Di default un nuovo tecnico è disponibile */
        nuovo->caricoLavoro = 0;
        nuovo->agenda = NULL;
        
        nuovo->next = *testa;//Il "next" del nuovo tecnico punterà al primo tecnico in testa
        *testa = nuovo;//La lista aggiorna il nuovo tecnico in testa
    }
}


//*GETTER E SETTER
//------------------------------------------------------------------------------------
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

int getDisponibilitaTecnico(NodoTecnico* t) {
    if (t != NULL) return t->disponibilita;
    return 0;
}

void setDisponibilitaTecnico(NodoTecnico* t, int disponibile) {
    if (t != NULL) t->disponibilita = disponibile;
}

NodoRichiesta* getNextRichiesta(NodoRichiesta* req) {
    if (req != NULL) return req->next;
    return NULL;
}

NodoTecnico* getNextTecnico(NodoTecnico* t) {
    if (t != NULL) return t->next;
    return NULL;
}

void setDataChiusura(NodoRichiesta* req, char* data) {
    if (req != NULL && data != NULL) {
        strcpy(req->dataChiusura, data);
    }
}
//------------------------------------------------------------------------------------

//Cerca una richiesta per codice analizzando ogni livello di urgenza

NodoRichiesta* trovaRichiestaPerCodice(NodoRichiesta* code[], int codice) {
    int indiceUrgenza;
    NodoRichiesta* nodoCorrente;

    /* Scorriamo tutti i livelli, partendo dall'urgenza 0 fino a LIVELLI_URGENZA-1 */
    for (indiceUrgenza = 0; indiceUrgenza < LIVELLI_URGENZA; indiceUrgenza++) {
        
        /* Impostiamo il nodo corrente alla testa della coda specifica */
        nodoCorrente = code[indiceUrgenza];
        
        /* Scorriamo l'intera coda finché non troviamo la fine (NULL) */
        while (nodoCorrente != NULL) {
            
            /* Se il codice corrisponde a quello cercato, fermiamo tutto e restituiamo il nodo */
            if (nodoCorrente->codiceRichiesta == codice) {
                return nodoCorrente; 
            }
            
            /* Altrimenti, passiamo al nodo successivo nella coda */
            nodoCorrente = nodoCorrente->next; 
        }
    }
    
    /* Se i cicli finiscono e non abbiamo trovato nulla, la richiesta non esiste */
    return NULL; 
}
//------------------------------------------------------------------------------------


int verificaConflitto(NodoTecnico* t, char* data, int fasciaOraria) {
    NodoAgenda* curr;
    
    if (t == NULL) return 0;
    
    curr = t->agenda;//variabile di supporto per non perdere la testa(letteralmente)
    while (curr != NULL) {
        if (strcmp(curr->data, data) == 0 && curr->fasciaOraria == fasciaOraria) {//Perchè si verifichi un conflitto dobbiamo confrontare data e ora contemporaneamente
            return 1; 
        }
        curr = curr->next;
    }
    return 0; 
}

int assegnaRichiesta(NodoRichiesta* code[], NodoTecnico* listaTecnici, int codiceRichiesta, char* data, int fasciaOraria) {
    NodoRichiesta* req;
    NodoTecnico* curr;

    req = trovaRichiestaPerCodice(code, codiceRichiesta);
    if (req == NULL) return 0;

    /* Scorriamo i tecnici cercando uno compatibile e disponibile */
    curr = listaTecnici;
    while (curr != NULL) {
        if (strcmp(curr->specializzazione, req->tipologia) == 0 && curr->disponibilita == 1) {
            if (aggiungiImpegnoAgenda(curr, codiceRichiesta, data, fasciaOraria) == 1) {
                req->stato = Pianificata;
                return 1;
            }
        }
        curr = curr->next;
    }
    return 0;
}

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
        
        t->caricoLavoro++; 
        
        return 1; 
    }
    return 0;
}



void stampaStatoGlobale(NodoRichiesta* code[], NodoTecnico* listaT) {
    int i;
    NodoRichiesta* currR;
    NodoTecnico* currT;
    NodoAgenda* currA;
    const char* nomiStato[] = {"Aperta", "Pianificata", "InLavorazione", "Conclusa", "Annullata"};

    printf("\n###########################################################\n");
    printf("            REPORT COMPLETO DELLO STATO DEL SISTEMA          \n");
    printf("###########################################################\n");

    /* --- SEZIONE TECNICI --- */
    printf("\n--- DATABASE TECNICI ---\n");
    currT = listaT;
    while (currT != NULL) {
        printf("[ID UNICO: %d] Nome: %s | Spec: %s | Carico: %d\n", 
               currT->idTecnico, currT->nome, currT->specializzazione, currT->caricoLavoro);
        
        /* Stampa l'agenda collegata al tecnico */
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

    /* --- SEZIONE RICHIESTE --- */
    printf("\n--- DATABASE RICHIESTE (DIVISE PER URGENZA) ---\n");
    for (i = LIVELLI_URGENZA - 1; i >= 0; i--) { /* Stampiamo dalle piu' urgenti */
        currR = code[i];
        if (currR != NULL) {
            printf("[LIVELLO URGENZA %d]\n", i);
            while (currR != NULL) {
                printf("  [CODICE UNICO: %d]\n", currR->codiceRichiesta);
                printf("  Appartamento: %s\n", currR->appartamento);
                printf("  Tipologia: %s\n", currR->tipologia);
                printf("  Descrizione: %s\n", currR->descrizione);
                printf("  Aperta il: %s | Chiusa il: %s\n", currR->dataRichiesta, currR->dataChiusura);             
                
                printf("  Stato attuale: %s\n", nomiStato[currR->stato]);
                
                printf("  ---------------------------------\n");
                currR = currR->next;
            }
        }
    }
    printf("###########################################################\n\n");
}