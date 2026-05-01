#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/interventi.h"
#include "../include/gestione_file.h"

/* * ======================================================================
 * HELPER FUNCTION: Stampa l'esito del test
 * ======================================================================
 * Prende in input una condizione booleana (es. 1 se vero, 0 se falso) 
 * e il nome del test. Stampa [PASS] in verde o [FAIL] in rosso.
 */
void stampaEsitoTest(int condizione, const char* nomeTest) {
    if (condizione) {
        printf("[PASS] %s\n", nomeTest);
    } else {
        printf("[FAIL] %s\n", nomeTest);
    }
}

/* * ======================================================================
 * SUITE 1: TEST REGISTRAZIONE TECNICI
 * ======================================================================
 */
void testRegistrazioneTecnici() {
    NodoTecnico* listaTecnici = NULL;
    
    printf("\n--- ESECUZIONE TEST: TECNICI ---\n");

    /* Inseriamo un tecnico di prova */
    inserisciTecnico(&listaTecnici, 101, "Mario Rossi", "Idraulico");

    /* Verifichiamo che la lista non sia più vuota e che i getter funzionino */
    stampaEsitoTest(listaTecnici != NULL, "T01 - Inserimento tecnico in testa (Memoria allocata)");
    stampaEsitoTest(getIdTecnico(listaTecnici) == 101, "T02 - Verifica ID Tecnico (Getter)");
    stampaEsitoTest(strcmp(getNomeTecnico(listaTecnici), "Mario Rossi") == 0, "T03 - Verifica Nome Tecnico (Getter)");
    stampaEsitoTest(getCaricoLavoro(listaTecnici) == 0, "T04 - Verifica Carico Lavoro iniziale a 0");
}

/* * ======================================================================
 * SUITE 2: TEST REGISTRAZIONE RICHIESTE E RICERCA
 * ======================================================================
 */
void testRegistrazioneRichieste() {
    NodoRichiesta* codeRichieste[LIVELLI_URGENZA];
    NodoTecnico* listaTecnici = NULL;
    int codiceGenerato;
    NodoRichiesta* trovata;

    printf("\n--- ESECUZIONE TEST: RICHIESTE ---\n");

    inizializzaSistema(codeRichieste, &listaTecnici);

    /* Inseriamo una richiesta con urgenza MASSIMA (indice 4) */
    inserisciRichiesta(codeRichieste, 4, "Scala A", "Idraulico", "Tubo rotto", "15/05/2026");

    stampaEsitoTest(codeRichieste[4] != NULL, "R01 - Inserimento richiesta nel livello urgenza corretto (4)");
    stampaEsitoTest(getStatoRichiesta(codeRichieste[4]) == Aperta, "R02 - Stato iniziale impostato su 'Aperta'");

    /* Testiamo la ricerca per codice implementata nel core */
    codiceGenerato = getCodiceRichiesta(codeRichieste[4]);
    trovata = trovaRichiestaPerCodice(codeRichieste, codiceGenerato);

    stampaEsitoTest(trovata != NULL && getCodiceRichiesta(trovata) == codiceGenerato, "R03 - Ricerca richiesta per codice univoco");
    
    /* Cerchiamo un codice che non esiste (es. 999) */
    stampaEsitoTest(trovaRichiestaPerCodice(codeRichieste, 999) == NULL, "R04 - Ricerca codice inesistente restituisce NULL");
}

/* * ======================================================================
 * SUITE 3: TEST ASSEGNAZIONE AGENDA E CONFLITTI
 * ======================================================================
 */
void testAssegnazioneEConflitti() {
    NodoTecnico* listaTecnici = NULL;
    int esito1, esito2, esito3;

    printf("\n--- ESECUZIONE TEST: AGENDA E CONFLITTI ---\n");

    /* Prepariamo un tecnico fittizio */
    inserisciTecnico(&listaTecnici, 102, "Luigi Verdi", "Elettricista");

    /* Test A: Aggiunta primo impegno (Il tecnico è completamente libero) */
    esito1 = aggiungiImpegnoAgenda(listaTecnici, 1, "16/05/2026", 1); /* Data, Fascia 1 */
    stampaEsitoTest(esito1 == 1, "A01 - Aggiunta impegno con tecnico libero");
    stampaEsitoTest(getCaricoLavoro(listaTecnici) == 1, "A02 - Incremento automatico del carico di lavoro (1)");

    /* Test B: CONFLITTO STESSA DATA E STESSA FASCIA (Deve fallire) */
    esito2 = aggiungiImpegnoAgenda(listaTecnici, 2, "16/05/2026", 1);
    stampaEsitoTest(esito2 == 0, "A03 - Rilevamento conflitto corretto (Stessa data e fascia occupata)");

    /* Test C: Stessa data, ma FASCIA DIVERSA (Deve passare) */
    esito3 = aggiungiImpegnoAgenda(listaTecnici, 3, "16/05/2026", 2);
    stampaEsitoTest(esito3 == 1, "A04 - Aggiunta impegno stessa data ma fascia oraria differente");
    stampaEsitoTest(getCaricoLavoro(listaTecnici) == 2, "A05 - Carico di lavoro aggiornato a 2");
}

/* * ======================================================================
 * SUITE 4: TEST AGGIORNAMENTO STATO
 * ======================================================================
 */
void testAggiornamentoStato() {
    NodoRichiesta* codeRichieste[LIVELLI_URGENZA];
    NodoTecnico* listaTecnici = NULL;
    NodoRichiesta* req;

    printf("\n--- ESECUZIONE TEST: AGGIORNAMENTO STATI ---\n");

    inizializzaSistema(codeRichieste, &listaTecnici);
    inserisciRichiesta(codeRichieste, 0, "Box Auto", "Elettricista", "Luce fulminata", "18/05/2026");
    
    req = codeRichieste[0];

    /* Simulo una transizione di stato verso InLavorazione */
    setStatoRichiesta(req, InLavorazione);
    stampaEsitoTest(getStatoRichiesta(req) == InLavorazione, "S01 - Transizione di stato a 'InLavorazione'");

    /* Simulo chiusura intervento */
    setStatoRichiesta(req, Conclusa);
    stampaEsitoTest(getStatoRichiesta(req) == Conclusa, "S02 - Transizione di stato a 'Conclusa'");
}

/* * ======================================================================
 * MAIN TEST RUNNER
 * ======================================================================
 */
int main() {
    printf("=========================================\n");
    printf("    ESECUZIONE SUITE DI TEST AUTOMATICI  \n");
    printf("=========================================\n");

    /* Chiamo le suite di test una dopo l'altra */
    testRegistrazioneTecnici();
    testRegistrazioneRichieste();
    testAssegnazioneEConflitti();
    testAggiornamentoStato();

    printf("\n=========================================\n");
    printf("        TUTTI I TEST COMPLETATI          \n");
    printf("=========================================\n");

    return 0;
}