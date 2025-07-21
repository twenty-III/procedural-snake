CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -Iinclude
LDFLAGS = -Llib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
TARGET = main
SRC = main.cpp

run: $(TARGET)
	./$(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
