#ifndef INTERVENTI_H
#define INTERVENTI_H

#define LIVELLI_URGENZA 5
#define MAX_STR 100

typedef enum {
    Aperta, 
    Pianificata, 
    InLavorazione, 
    Conclusa, 
    Annullata
} StatoRichiesta;

typedef struct NodoAgenda NodoAgenda;
typedef struct NodoTecnico NodoTecnico;
typedef struct NodoRichiesta NodoRichiesta;

void inizializzaSistema(NodoRichiesta* code[], NodoTecnico** listaT);

void inserisciRichiesta(NodoRichiesta* code[], int urgenza, char* app, char* tipo, char* desc, char* data);
int getCodiceRichiesta(NodoRichiesta* req);
StatoRichiesta getStatoRichiesta(NodoRichiesta* req);
void setStatoRichiesta(NodoRichiesta* req, StatoRichiesta nuovoStato);

void inserisciTecnico(NodoTecnico** testa, int id, char* nome, char* spec);
int getIdTecnico(NodoTecnico* t);
char* getNomeTecnico(NodoTecnico* t);
int getCaricoLavoro(NodoTecnico* t);

#endif