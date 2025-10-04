# Diretórios
SRC_DIR := src
BIN_DIR := bin
OBJ_DIR := obj

# Arquivos fonte e objetos
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Nome do executável
TARGET := $(BIN_DIR)/nemertea

# Compilador e flags
CXX := g++
CXXFLAGS := -Wall -O2 -std=c++23 -Isrc

# Regras
all: $(BIN_DIR) $(OBJ_DIR) $(TARGET)

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean