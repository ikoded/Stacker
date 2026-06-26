CXX = g++
CXXFLAGS = -Wall -std=c++20 -I./include

SOURCES = src/main.cpp src/GameBoard.cpp src/Player.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = stacker.exe

all : $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.PHONY: all clean