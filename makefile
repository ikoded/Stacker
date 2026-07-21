CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -Wpedantic -Werror

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