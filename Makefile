# Compilador y flags
CC = gcc
CFLAGS = -Wall -Iinc -lSDL2 -lSDL2_ttf -lSDL2_image -lunistring -g
# CFLAGS = -Wall -Iinc -Isrc/buffers (cada vez que quiera meter una carpeta mas dentro del src, linkear asi) -lSDL2 -lSDL2_ttf -lSDL2_image -lunistring -g

# Directorios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INCLUDE_DIR = inc

# Nombre del ejecutable
TARGET = $(BIN_DIR)/capochess

# Encontrar todos los archivos .c dentro de src y sus subdirectorios
SRC_FILES = $(shell find $(SRC_DIR) -name "*.c")
# Los archivos .o se generarán en la carpeta obj
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Reglas
all: $(BIN_DIR) $(OBJ_DIR) $(TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compilar el ejecutable final
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $(OBJ_FILES) -ly -lfl

# Regla genérica para compilar archivos .c
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar archivos generados
clean:
	rm -rf $(OBJ_DIR)

.PHONY: all clean
