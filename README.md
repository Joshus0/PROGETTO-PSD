#  Sistema di Gestione Manutenzioni Condominiali

> **Progetto per l'esame di Programmazione e Strutture Dati**
> *A.A. 2025/2026 (Traccia 1)*

![C](https://img.shields.io/badge/Language-C99-blue.svg)
![Build](https://img.shields.io/badge/Build-Makefile-orange.svg)
![Platform](https://img.shields.io/badge/Platform-Cross--Platform-lightgrey.svg)

Il sistema offre una soluzione software completa per la gestione degli interventi tecnici in un condominio. Implementato interamente in **C Standard**, il programma si focalizza sull'efficienza delle strutture dati per la gestione di anagrafiche, pianificazione temporale e risoluzione dei conflitti.

---

## 👥 Team di Sviluppo
* **Joshua Sarnelli**
* **Sabato Pio**
* **Raffaele Pio Villani**

---

## 🛠️ Funzionalità Core
* 📝 **Gestione Richieste:** Registrazione e archiviazione delle segnalazioni.
* 👨‍🔧 **Anagrafica Tecnici:** Gestione completa del database operatori.
* 📅 **Planning Intelligente:** Algoritmo di verifica conflitti orari per gli interventi.
* 📊 **Reporting:** Generazione automatica di report sull'attività manutentiva.

---

## 🚀 Guida Rapida

### Prerequisiti
Assicurati di avere installato un compilatore **GCC** e l'utility **Make**.

### Compilazione e Avvio
Dalla cartella principale del progetto, apri il terminale ed esegui:

```bash
make all
```

> [!TIP]
> **Nota per utenti Windows**
> Se utilizzi un ambiente Windows (MinGW, PowerShell o IDE specifici) e il comando `make` non viene riconosciuto, utilizza invece:
> ```bash
> mingw32-make
> ```

---

## 📁 Struttura del Repository

| Cartella | Descrizione |
| :--- | :--- |
| `src/` | File sorgente (.c) |
| `include/` | Header file (.h) |
| `obj/` | File oggetto generati durante la compilazione |
| `bin/` | Eseguibile finale |

---

## 🛠 Specifiche Tecniche
* **Linguaggio:** C (Standard C89/C99)
* **Gestione Dati:** Strutture dati dinamiche per l'ottimizzazione della memoria.
* **Build System:** `Makefile` per l'automazione della compilazione.