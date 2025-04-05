CC = gcc
CFLAGS = -Wall -pthread
LIBS = -lrt -lpthread -lm

CONTROL_DIR = Control
DISPLAY_DIR = Display
SENSORS_DIR = Sensors
BIN_DIR = bin

# Definição dos arquivos objeto
OBJS = $(CONTROL_DIR)/control.o $(DISPLAY_DIR)/display.o $(SENSORS_DIR)/sensors.o

# Definição dos executáveis dentro do diretório bin
EXECS = $(BIN_DIR)/control $(BIN_DIR)/display $(BIN_DIR)/sensors

# Regra principal
all: $(BIN_DIR) $(EXECS)

# Criar diretório bin se não existir
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Regras para cada executável
$(BIN_DIR)/control: $(CONTROL_DIR)/control.c $(CONTROL_DIR)/control.h
	$(CC) $(CFLAGS) $(CONTROL_DIR)/control.c -o $(BIN_DIR)/control $(LIBS)

$(BIN_DIR)/sensors: $(SENSORS_DIR)/sensors.c $(SENSORS_DIR)/sensors.h
	$(CC) $(CFLAGS) $(SENSORS_DIR)/sensors.c -o $(BIN_DIR)/sensors $(LIBS)

$(BIN_DIR)/display: $(DISPLAY_DIR)/display.c $(DISPLAY_DIR)/display.h
	$(CC) $(CFLAGS) $(DISPLAY_DIR)/display.c -o $(BIN_DIR)/display $(LIBS)

# Limpeza
clean:
	rm -rf $(BIN_DIR)/*
