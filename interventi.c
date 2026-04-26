#include "interventi.h"
#include <stdlib.h>
#include <string.h>

/* Gestisce il periodo di tempo di un intervento.*/
/*Usiamo degli interi per le ore (0-23) e i minuti (0-59) 
per semplificare la logica di verifica dei conflitti temporali.*/

/*(PER JOSHUA)*/

typedef struct FasciaOraria {
    int ora_inizio;
    int minuto_inizio;
    int ora_fine;
    int minuto_fine;
};

/* STRUTTURA: Tecnico (Dati a dominio)
Rappresenta l'anagrafica di un tecnico e il suo stato attuale.*/
typedef struct Tecnico {
    char id[50];                    /* Codice identificativo*/
    char nome[100];                 /* Nome del tecnico */
    char specializzazione[100];     /* Es. "Elettricista", "Idraulico" */
    
    int disponibilita;              /* Flag booleano: 1 = disponibile, 0 = non disponibile */
    int carico_lavoro;              /* Contatore degli interventi attualmente in lavorazione */
    struct Tecnico *next;           /*Puntatore alla lista tecnici*/
};
