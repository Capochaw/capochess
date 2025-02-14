# Configuración del compilador y flags
CXX = g++
CXXFLAGS = -Wall -Iinc -g `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_ttf -lSDL2_image

# Directorios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INCLUDE_DIR = inc

# Nombre del ejecutable
TARGET = $(BIN_DIR)/capochess

# Encontrar todos los archivos .cpp en src/
SRC_FILES = $(shell find $(SRC_DIR) -name "*.cpp")

# Generar los archivos .o en obj/
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Regla principal
all: $(BIN_DIR) $(OBJ_DIR) $(TARGET)

# Crear carpetas si no existen
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compilar el ejecutable final
$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ_FILES) $(LDFLAGS)

# Regla para compilar archivos .cpp en .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar archivos generados
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
