#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/interventi.h"
#include "../include/gestione_file.h"

/* ? Funzione per pulire lo stream di input ed evitare letture errate nei cicli */
void pulisciBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    NodoRichiesta* codeRichieste[LIVELLI_URGENZA];
    NodoTecnico* listaTecnici;
    int scelta = -1;

    /* ? Variabili temporanee per catturare gli input dell'utente tramite menu */
    char bufferApp[MAX_STR], bufferTipo[MAX_STR], bufferDesc[255], bufferData[11];
    int urgenza, fascia;

    /* ? Setup iniziale delle strutture dati */
    inizializzaSistema(codeRichieste, &listaTecnici);

    printf("=========================================\n");
    printf("  SISTEMA GESTIONE MANUTENZIONE CONDOMINIO \n");
    printf("=========================================\n");

    /* ? Importazione dei database esterni */
    printf("Caricamento database...\n");
    caricaTecniciDaFile("tests/tecnici.txt", &listaTecnici);
    caricaRichiesteDaFile("tests/richieste.txt", codeRichieste);
    printf("Database caricato con successo.\n");

    /* * CICLO DI INTERFACCIA UTENTE (CLI) */
    while (scelta != 0) {
        printf("\n--- MENU PRINCIPALE ---\n");
        printf("1. Inserisci Nuova Richiesta\n");
        printf("2. Visualizza Stato Globale (Report Completo)\n");
        printf("3. Prova Assegnazione Manuale (Test Conflitti)\n");
        printf("0. Esci dal Programma\n");
        printf("Seleziona un'opzione: ");

        /* ? Controllo validita' input numerico per la scelta del menu */
        if (scanf("%d", &scelta) != 1) {
            printf("Errore: Inserisci un numero valido.\n");
            pulisciBuffer();
            continue;
        }
        pulisciBuffer();

        switch (scelta) {
            case 1:
                /* * INSERIMENTO MANUALE RICHIESTA */
                printf("\n-- INSERIMENTO RICHIESTA --\n");
                printf("Appartamento (es. Scala A): ");
                fgets(bufferApp, MAX_STR, stdin);
                bufferApp[strcspn(bufferApp, "\n")] = 0; /* Rimuove il newline */

                printf("Tipologia (es. Idraulico): ");
                fgets(bufferTipo, MAX_STR, stdin);
                bufferTipo[strcspn(bufferTipo, "\n")] = 0;

                printf("Descrizione: ");
                fgets(bufferDesc, 255, stdin);
                bufferDesc[strcspn(bufferDesc, "\n")] = 0;

                printf("Data (DD/MM/YYYY): ");
                fgets(bufferData, 11, stdin);
                bufferData[strcspn(bufferData, "\n")] = 0;

                printf("Urgenza (0=Min, 4=Max): ");
                scanf("%d", &urgenza);
                pulisciBuffer();

                /* ? Validazione dell'indice di urgenza prima dell'inserimento */
                if (urgenza >= 0 && urgenza < LIVELLI_URGENZA) {
                    inserisciRichiesta(codeRichieste, urgenza, bufferApp, bufferTipo, bufferDesc, bufferData);
                    printf("\n[OK] Richiesta inserita con successo nel secchiello %d!\n", urgenza);
                } else {
                    printf("\n[ERRORE] Livello urgenza non valido.\n");
                }
                break;

            case 2:
                /* * VISUALIZZAZIONE COMPLETA DELLA MEMORIA DINAMICA */
                /* ? Richiama la funzione di diagnostica per stampare tecnici, agende e richieste */
                stampaStatoGlobale(codeRichieste, listaTecnici);
                break;

            case 3:
                /* * MODULO DI TEST PER LA LOGICA DEI CONFLITTI */
                if (listaTecnici == NULL) {
                    printf("\n[ERRORE] Nessun tecnico caricato. Controlla il file tecnici.txt\n");
                    break;
                }
                printf("\n-- TEST ASSEGNAZIONE E CONFLITTI --\n");
                printf("Utilizzo tecnico in testa: %s\n", getNomeTecnico(listaTecnici));
                
                printf("Inserisci Data (es. 10/10/2026): ");
                fgets(bufferData, 11, stdin);
                bufferData[strcspn(bufferData, "\n")] = 0;
                
                printf("Fascia Oraria (1=Mattina, 2=Pomeriggio, 3=Sera): ");
                scanf("%d", &fascia);
                pulisciBuffer();

                /* ? Verifica se il tecnico e' libero chiamando la logica di gestione agenda */
                if (aggiungiImpegnoAgenda(listaTecnici, 999, bufferData, fascia) == 1) {
                    printf("\n[SUCCESSO] Impegno registrato! Il tecnico e' libero.\n");
                    printf("Carico di lavoro aggiornato: %d\n", getCaricoLavoro(listaTecnici));
                } else {
                    printf("\n[BLOCCATO] Conflitto rilevato! Tecnico occupato in questa data/fascia.\n");
                }
                break;

            case 0:
                printf("Uscita in corso... Arrivederci!\n");
                break;

            default:
                printf("Opzione non valida. Riprova.\n");
        }
    }

    return 0;
}