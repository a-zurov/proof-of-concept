CC = g++ -std=c++17
TARGET = sfinae
OBJDIR := obj
SRC = $(wildcard *.cpp)
HEAD = $(wildcard *.h)
PREF_OBJ = ./$(OBJDIR)/
OBJ = $(patsubst %.cpp, $(PREF_OBJ)%.o, $(SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)

$(PREF_OBJ)%.o: %.cpp $(HEAD)
	$(CC) -c $< -o $@

clean:
	rm -rf $(PREF_OBJ)*.o $(TARGET)

$(shell mkdir -p $(OBJDIR))
