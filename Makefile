# Compilatore e Flag (Standard C89 e avvertimenti attivati)
CC = gcc
CFLAGS = -ansi -Wall -Wextra -Iinclude

# --- RILEVAMENTO UNIVERSALE DEL SISTEMA OPERATIVO ---
ifeq ($(OS),Windows_NT)
    # Impostazioni se il PC è Windows
    TARGET = condominio.exe
    CLEAN_CMD = cmd /C del /Q /F $(TARGET)
else
    # Impostazioni se il PC è Mac o Linux
    TARGET = condominio
    CLEAN_CMD = rm -f $(TARGET)
endif
# ----------------------------------------------------

# Target principale: compila tutto quello che trova in src/
all:
	$(CC) $(CFLAGS) src/*.c -o $(TARGET)

# Esegue il programma dopo averlo compilato (usa ./ per compatibilità Mac/Linux)
run: all
	./$(TARGET)

# Pulisce la cartella. Il trattino iniziale '-' significa "ignora gli errori"
clean:
	-$(CLEAN_CMD)

# Target per i test
test:
	@echo "Esecuzione dei test non ancora configurata"