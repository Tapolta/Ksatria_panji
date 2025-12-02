TARGET = KsatriaPanji.exe
CXX = g++
CXXFLAGS = -ID:/libs/sdl3/include
LDFLAGS = -LD:/libs/sdl3/lib -lSDL3 -lSDL3_image -lSDL3_ttf

SRC = $(wildcard *.cpp src/*.cpp)

OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(LDFLAGS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	cmd /C del $(TARGET) *.o 2>nul