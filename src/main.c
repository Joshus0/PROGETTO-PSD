#include <stdio.h>
#include "../include/interventi.h"

int main() {
    NodoRichiesta* codeRichieste[LIVELLI_URGENZA];
    NodoTecnico* listaTecnici;

    inizializzaSistema(codeRichieste, &listaTecnici);

    inserisciTecnico(&listaTecnici, 101, "Mario Rossi", "Idraulico");
    inserisciTecnico(&listaTecnici, 102, "Luigi Verdi", "Elettricista");

    inserisciRichiesta(codeRichieste, 4, "Scala A, Int 3", "Idraulico", "Perdita tubo principale", "15/05/2026");
    inserisciRichiesta(codeRichieste, 2, "Scala B, Int 5", "Elettricista", "Presa bruciata", "16/05/2026");

    printf("--- TEST INSERIMENTO E INFORMATION HIDING ---\n\n");

    printf("Ultimo tecnico inserito in testa:\n");
    printf("Nome: %s\n", getNomeTecnico(listaTecnici));
    printf("ID: %d\n", getIdTecnico(listaTecnici));
    printf("Carico di lavoro: %d\n\n", getCaricoLavoro(listaTecnici));

    printf("Ultima richiesta inserita con urgenza 4:\n");
    printf("Codice Richiesta: %d\n", getCodiceRichiesta(codeRichieste[4]));
    printf("Stato Attuale (0=Aperta, 1=Pianificata): %d\n\n", getStatoRichiesta(codeRichieste[4]));

    setStatoRichiesta(codeRichieste[4], Pianificata);

    printf("Modifica stato richiesta urgenza 4...\n");
    printf("Nuovo Stato: %d\n", getStatoRichiesta(codeRichieste[4]));

    return 0;
}