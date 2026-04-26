# Sistema di Gestione degli Interventi di Manutenzione in un Condominio

Progetto per l'esame di Programmazione e Strutture Dati - A.A. 2025/2026 (Traccia 1).

**Team di Sviluppo:**
* Joshua Sarnelli
* Sabato Pio 
* Raffaele Pio Villani

## Descrizione del Sistema
Il programma, scritto in C Standard (C99), gestisce gli interventi di manutenzione di un condominio. Permette di registrare nuove richieste, gestire l'anagrafica dei tecnici, pianificare gli interventi (con verifica dei conflitti orari) e generare report.

## Piattaforma e Prerequisiti
* **Compilatore:** GCC compatibile con standard C89/C99
* **Tool di build:** Make

## Istruzioni di Compilazione e Avvio
Il progetto utilizza un `Makefile` per automatizzare il processo di build e testing. Dalla root directory del progetto, aprire il terminale ed eseguire i seguenti comandi:

**1. Compilazione del programma principale:**
```bash
make all 

#! NOTA PER GLI UTENTI WINDOWS

Il comando "make all" potrebbe non funzionare nel proprio IDE, usare in cambio:

mingw32-make