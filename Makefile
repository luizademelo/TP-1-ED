CC = g++ 
LIBS = -lm
SRC = src
OBJ = obj
INC = include
BIN = bin
OBJS = $(OBJ)/poker.o
HDRS = $(INC)/poker.hpp 

TARGET = $(BIN)/tp1 

all: $(TARGET)

$(TARGET): $(SRC)/poker.cpp $(SRC)/main.cpp 
	$(CC) $(SRC)/poker.cpp $(SRC)/main.cpp -o $(TARGET)
