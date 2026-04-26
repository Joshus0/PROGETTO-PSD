#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/interventi.h"
#include "../include/gestione_file.h"

void pulisciBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    NodoRichiesta* codeRichieste[LIVELLI_URGENZA];
    NodoTecnico* listaTecnici;
    int scelta = -1;

    /* Dati temporanei per gli input */
    char bufferApp[MAX_STR], bufferTipo[MAX_STR], bufferDesc[255], bufferData[11];
    int urgenza, fascia;

    inizializzaSistema(codeRichieste, &listaTecnici);

    printf("=========================================\n");
    printf("  SISTEMA GESTIONE MANUTENZIONE CONDOMINIO \n");
    printf("=========================================\n");

    /* Caricamento iniziale dai file */
    printf("Caricamento database...\n");
    caricaTecniciDaFile("tests/tecnici.txt", &listaTecnici);
    caricaRichiesteDaFile("tests/richieste.txt", codeRichieste);
    printf("Database caricato con successo.\n");

    while (scelta != 0) {
        printf("\n--- MENU PRINCIPALE ---\n");
        printf("1. Inserisci Nuova Richiesta\n");
        printf("2. Visualizza Dati in Memoria (Test Lettura)\n");
        printf("3. Prova Assegnazione Manuale (Test Conflitti)\n");
        printf("0. Esci dal Programma\n");
        printf("Seleziona un'opzione: ");

        if (scanf("%d", &scelta) != 1) {
            printf("Errore: Inserisci un numero valido.\n");
            pulisciBuffer();
            continue;
        }
        pulisciBuffer();

        switch (scelta) {
            case 1:
                printf("\n-- INSERIMENTO RICHIESTA --\n");
                printf("Appartamento (es. Scala A): ");
                fgets(bufferApp, MAX_STR, stdin);
                bufferApp[strcspn(bufferApp, "\n")] = 0;

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

                if (urgenza >= 0 && urgenza < LIVELLI_URGENZA) {
                    inserisciRichiesta(codeRichieste, urgenza, bufferApp, bufferTipo, bufferDesc, bufferData);
                    printf("\n[OK] Richiesta inserita con successo nel secchiello %d!\n", urgenza);
                } else {
                    printf("\n[ERRORE] Livello urgenza non valido.\n");
                }
                break;

            case 2:
                /* Ora usiamo la nuova funzione che stampa TUTTO */
                stampaStatoGlobale(codeRichieste, listaTecnici);
                break;

            case 3:
                /* Test manuale per l'algoritmo dei conflitti */
                if (listaTecnici == NULL) {
                    printf("\n[ERRORE] Nessun tecnico caricato. Assicurati che tecnici.txt esista.\n");
                    break;
                }
                printf("\n-- TEST ASSEGNAZIONE E CONFLITTI --\n");
                printf("Proviamo ad assegnare un lavoro al tecnico in testa: %s\n", getNomeTecnico(listaTecnici));
                
                printf("Inserisci Data (es. 10/10/2026): ");
                fgets(bufferData, 11, stdin);
                bufferData[strcspn(bufferData, "\n")] = 0;
                
                printf("Inserisci Fascia Oraria (1=Mattina, 2=Pomeriggio, 3=Sera): ");
                scanf("%d", &fascia);
                pulisciBuffer();

                /* Chiamiamo la tua funzione che incapsula anche verificaConflitto */
                if (aggiungiImpegnoAgenda(listaTecnici, 999, bufferData, fascia) == 1) {
                    printf("\n[SUCCESSO] Impegno registrato! Il tecnico e' libero.\n");
                    printf("Il suo carico di lavoro ora e': %d\n", getCaricoLavoro(listaTecnici));
                } else {
                    printf("\n[BLOCCATO] Conflitto rilevato! Il tecnico e' gia' occupato in quella data e fascia oraria.\n");
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