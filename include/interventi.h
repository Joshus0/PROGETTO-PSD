#ifndef INTERVENTI_H
#define INTERVENTI_H

#define LIVELLI_URGENZA 5
#define MAX_STR 100

/* ? Definizione degli stati possibili per un ticket */
typedef enum {
    Aperta, 
    Pianificata, 
    InLavorazione, 
    Conclusa, 
    Annullata
} StatoRichiesta;

/* ? Tipi opachi per l'incapsulamento (Information Hiding) */
typedef struct NodoAgenda NodoAgenda;
typedef struct NodoTecnico NodoTecnico;
typedef struct NodoRichiesta NodoRichiesta;

/* * LOGICA DI SISTEMA */
/* ------------------------------------------------------------------------------------- */

/* Post-condizione: I puntatori dell'array code e la testa listaT puntano a NULL. */
void inizializzaSistema(NodoRichiesta* code[], NodoTecnico** listaT);


/* * FUNZIONI RICHIESTE */
/* ------------------------------------------------------------------------------------- */

/* Post-condizione: Nuova richiesta allocata in testa all'array code[urgenza]. Stato iniziale impostato su 'Aperta'. */
void inserisciRichiesta(NodoRichiesta* code[], int urgenza, char* app, char* tipo, char* desc, char* data);

/* Post-condizione: Output = intero univoco della richiesta (o -1 se req e' NULL). */
int getCodiceRichiesta(NodoRichiesta* req);

/* Post-condizione: Output = StatoRichiesta attuale (o Annullata se req e' NULL). */
StatoRichiesta getStatoRichiesta(NodoRichiesta* req);

/* Post-condizione: Il campo 'stato' del NodoRichiesta viene sovrascritto con nuovoStato. */
void setStatoRichiesta(NodoRichiesta* req, StatoRichiesta nuovoStato);


/* * FUNZIONI TECNICI */
/* ------------------------------------------------------------------------------------- */

/* Post-condizione: Nuovo tecnico allocato in testa a listaT. caricoLavoro = 0, agenda = NULL. */
void inserisciTecnico(NodoTecnico** testa, int id, char* nome, char* spec);

/* Post-condizione: Output = intero ID del tecnico (o -1 se t e' NULL). */
int getIdTecnico(NodoTecnico* t);

/* Post-condizione: Output = puntatore alla stringa del nome (o NULL se t e' NULL). */
char* getNomeTecnico(NodoTecnico* t);

/* Post-condizione: Output = intero che rappresenta gli interventi in agenda (o 0 se t e' NULL). */
int getCaricoLavoro(NodoTecnico* t);


/* * LOGICA ASSEGNAZIONE E DIAGNOSTICA */
/* ------------------------------------------------------------------------------------- */

/* Post-condizione: Output = 1 (Vero) se esiste gia' un impegno per quella data/orario, 0 (Falso) altrimenti. */
int verificaConflitto(NodoTecnico* t, char* data, int fasciaOraria);

/* Post-condizione: Output = 1 (Successo: nodo allocato nell'agenda, caricoLavoro incrementato di 1) 
                    oppure 0 (Fallimento: tecnico NULL, memoria insufficiente o conflitto rilevato). */
int aggiungiImpegnoAgenda(NodoTecnico* t, int codiceRichiesta, char* data, int fasciaOraria);

/* Post-condizione: Stampa a video l'intero contenuto della memoria, evidenziando ID unici e relazioni. */
void stampaStatoGlobale(NodoRichiesta* code[], NodoTecnico* listaT);

#endif