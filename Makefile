CC = g++
OUT = chip8
ODIR = ./obj
SDIR = ./src
OUTDIR = ./bin
IDIR = ./include
LDIR = ./lib
libs = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL_ttf
OBJS = $(patsubst src/%.cpp, obj/%.o, $(wildcard src/*.cpp)) $(patsubst src/Chip8/%.cpp, obj/%.o, $(wildcard src/Chip8/*.cpp))
vpath %.cpp $(SDIR):$(SDIR)/Chip8

$(ODIR)/%.o : %.cpp
	$(CC) -c -I $(IDIR) -o $@ $^

$(OUTDIR)/% : $(wildcard obj/*.o)
	$(CC) -L $(LDIR) -o $@ $^ $(libs)

.PHONY : run

run :
	$(OUTDIR)/$(OUT)