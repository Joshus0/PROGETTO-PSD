# Compilatore e Flag (Standard C89 e avvertimenti attivati)
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Iinclude

# --- RILEVAMENTO UNIVERSALE DEL SISTEMA OPERATIVO ---
ifeq ($(OS),Windows_NT)
    TARGET = condominio.exe
    TEST_TARGET = test_condominio.exe
	TEST_SRC = tests/main_test.c
    CLEAN_CMD = powershell -Command "Remove-Item -Force *.exe -ErrorAction Ignore"
else
	TARGET = condominio
	TEST_TARGET = test_condominio
	CLEAN_CMD = rm -f $(TARGET) $(TEST_TARGET)
endif
# ----------------------------------------------------

# --- DEFINIZIONE ESPLICITA DEI FILE ---
# In CORE_SRCS mettiamo tutti i moduli che servono a entrambi.
CORE_SRCS = src/interventi.c src/gestione_file.c
MAIN_SRC = src/main.c
TEST_SRC = tests/main_test.c

# Target principale: compila solo il programma base
all:
	$(CC) $(CFLAGS) $(MAIN_SRC) $(CORE_SRCS) -o $(TARGET)

# Esegue il programma principale
run: all
	./$(TARGET)

# --- TARGET PER I TEST ---
# Compila usando main_test.c invece di main.c, poi lo esegue automaticamente
test:
	$(CC) $(CFLAGS) $(TEST_SRC) $(CORE_SRCS) -o $(TEST_TARGET)
	./$(TEST_TARGET)

# Pulisce la cartella da entrambi gli eseguibili
clean:
	-$(CLEAN_CMD)