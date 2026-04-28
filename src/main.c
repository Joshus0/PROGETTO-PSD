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
        printf("4. Assegna Richiesta a Tecnico\n");
        printf("5. Aggiorna Stato Richiesta\n");
        printf("6. Cerca Richiesta (per Codice o Tipologia)\n");
        printf("7. Genera Report Statistico\n");
        printf("8. Registra Nuovo Tecnico\n");
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
                    if (inserisciRichiesta(codeRichieste, urgenza, bufferApp, bufferTipo, bufferDesc, bufferData) == 1) {
                        printf("\n[OK] Richiesta inserita con successo!\n");
                    }
                } else {
                    printf("\n[ERRORE] Livello urgenza non valido.\n");
                }
                break;

            case 2:
                printf("\n--- MENU VISUALIZZAZIONE RICHIESTE ---\n");
                printf("1. Stato Globale del Sistema (Tutto)\n");
                printf("2. Filtra per Stato della richiesta\n");
                printf("3. Filtra per Tipologia di guasto\n");
                printf("4. Filtra per Appartamento\n");
                printf("5. Visualizza Agenda per Tecnico\n");
                printf("Scegli un'opzione: ");
                
                int subScelta;
                scanf("%d", &subScelta);
                pulisciBuffer();

                switch (subScelta) {
                    case 1:
                        stampaStatoGlobale(codeRichieste, listaTecnici);
                        break;
                        
                    case 2: {
                        printf("Inserisci stato (0=Aperta, 1=Pianificata, 2=InLavorazione, 3=Conclusa, 4=Annullata): ");
                        int st;
                        scanf("%d", &st);
                        pulisciBuffer();
                        if (st >= 0 && st <= 4) {
                            stampaRichiestePerStato(codeRichieste, (StatoRichiesta)st);
                        } else {
                            printf("[ERRORE] Stato non valido.\n");
                        }
                        break;
                    }
                    case 3:
                        printf("Inserisci tipologia da cercare (es. Idraulico): ");
                        fgets(bufferTipo, MAX_STR, stdin);
                        bufferTipo[strcspn(bufferTipo, "\n")] = 0;
                        stampaRichiestePerTipo(codeRichieste, bufferTipo);
                        break;

                    case 4:
                        printf("Inserisci l'appartamento da cercare (es. Scala A): ");
                        fgets(bufferApp, MAX_STR, stdin);
                        bufferApp[strcspn(bufferApp, "\n")] = 0;
                        stampaRichiestePerAppartamento(codeRichieste, bufferApp);
                        break;

                    case 5: {
                        printf("Inserisci ID del tecnico: ");
                        int idTec;
                        scanf("%d", &idTec);
                        pulisciBuffer();
                        stampaRichiestePerTecnico(codeRichieste, listaTecnici, idTec);
                        break;
                    }
                    default:
                        printf("\n[ERRORE] Opzione di visualizzazione non valida.\n");
                }
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


            case 4:
                printf("\n-- ASSEGNAZIONE RICHIESTA --\n");
                printf("Codice richiesta da assegnare: ");
                {
                    int codiceReq;
                    scanf("%d", &codiceReq);
                    pulisciBuffer();

                    printf("Data intervento (DD/MM/YYYY): ");
                    fgets(bufferData, 11, stdin);
                    bufferData[strcspn(bufferData, "\n")] = 0;

                    printf("Fascia oraria (1=Mattina, 2=Pomeriggio, 3=Sera): ");
                    scanf("%d", &fascia);
                    pulisciBuffer();

                    /* Il main non accede ai dati, passa i puntatori opachi alla funzione del modulo */
                    int idAssegnato = assegnaRichiesta(codeRichieste, listaTecnici, codiceReq, bufferData, fascia);

                    if (idAssegnato > 0) 
                    {
                        printf("\n[OK] Tecnico compatibile trovato! Intervento pianificato al Tecnico ID: %d.\n", idAssegnato);
                    } else if (idAssegnato == 0) 
                    {
                        printf("\n[ERRORE] Nessun tecnico compatibile disponibile o conflitto orario per tutti i tecnici adatti.\n");
                    } else 
                    {
                        printf("\n[ERRORE] Il codice richiesta inserito non e' stato trovato nel sistema.\n");
                    }
                break;
                }

            case 5:
                printf("\n-- AGGIORNAMENTO STATO RICHIESTA --\n");
                printf("Codice richiesta: ");
                {
                int codiceUpd;
                scanf("%d", &codiceUpd);
                pulisciBuffer();

                NodoRichiesta* reqDaAggiornare = trovaRichiestaPerCodice(codeRichieste, codiceUpd);
                if (reqDaAggiornare == NULL) {
                    printf("\n[ERRORE] Richiesta non trovata.\n");
                    break;
                }
                printf("Nuovo stato (0=Aperta, 1=Pianificata, 2=InLavorazione, 3=Conclusa, 4=Annullata): ");
                int nuovoStato;
                scanf("%d", &nuovoStato);
                pulisciBuffer();

                if (nuovoStato < 0 || nuovoStato > 4) {
                    printf("\n[ERRORE] Stato non valido.\n");
                    break;
                }

                setStatoRichiesta(reqDaAggiornare, (StatoRichiesta)nuovoStato);

                if (nuovoStato == 3) { /* Conclusa */
                    printf("Data di chiusura (DD/MM/YYYY): ");
                    fgets(bufferData, 11, stdin);
                    bufferData[strcspn(bufferData, "\n")] = 0;
                setDataChiusura(reqDaAggiornare, bufferData);
                }

                printf("\n[OK] Stato aggiornato con successo.\n");
                break;
                }

            case 6:
                printf("\n--- RICERCA RICHIESTE ---\n");
                printf("1. Cerca per Codice Univoco\n");
                printf("2. Cerca per Tipologia di Guasto\n");
                printf("Scegli un'opzione: ");
                
                int sceltaRicerca;
                scanf("%d", &sceltaRicerca);
                pulisciBuffer();

                if (sceltaRicerca == 1) {
                    printf("Inserisci il codice della richiesta da cercare: ");
                    int codRicerca;
                    scanf("%d", &codRicerca);
                    pulisciBuffer();
                    
                    /* Richiama la nuova funzione che gestisce la stampa nascondendo i dati */
                    stampaRichiestaPerCodice(codeRichieste, codRicerca);
                    
                } else if (sceltaRicerca == 2) {
                    printf("Inserisci la tipologia da cercare (es. Idraulico): ");
                    fgets(bufferTipo, MAX_STR, stdin);
                    bufferTipo[strcspn(bufferTipo, "\n")] = 0;
                    
                    /* Ricicliamo la funzione creata per le visualizzazioni */
                    stampaRichiestePerTipo(codeRichieste, bufferTipo);
                    
                } else {
                    printf("\n[ERRORE] Opzione di ricerca non valida.\n");
                }
                break;
            
            case 7:
                printf("\nGenerazione del report in corso...\n");
                generaReportStatistico(codeRichieste, listaTecnici);
                break;

            case 8:
                printf("\n-- REGISTRAZIONE NUOVO TECNICO --\n");
                
                /* Dichiariamo le variabili necessarie in questo blocco */
                int nuovoId;
                char bufferNome[MAX_STR];
                char bufferSpec[MAX_STR];

                printf("Inserisci un ID univoco per il tecnico (numero intero): ");
                scanf("%d", &nuovoId);
                pulisciBuffer();

               /* --- NUOVO CONTROLLO UNICITA' ID --- */
                if (esisteTecnico(listaTecnici, nuovoId) == 1) {
                    printf("\n[ERRORE] Operazione annullata. Esiste gia' un tecnico registrato con l'ID %d.\n", nuovoId);
                    break; /* Esce dallo switch e torna al menu principale */
                }
                
                printf("Inserisci Nome e Cognome: ");
                fgets(bufferNome, MAX_STR, stdin);
                bufferNome[strcspn(bufferNome, "\n")] = 0;

                printf("Inserisci Specializzazione (es. Idraulico, Elettricista): ");
                fgets(bufferSpec, MAX_STR, stdin);
                bufferSpec[strcspn(bufferSpec, "\n")] = 0;

                /* Passiamo i dati al modulo interventi.c che si occuperà di creare la struttura */
                inserisciTecnico(&listaTecnici, nuovoId, bufferNome, bufferSpec);

                printf("\n[OK] Tecnico '%s' (Specializzazione: %s) registrato con successo con ID %d!\n", 
                       bufferNome, bufferSpec, nuovoId);
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