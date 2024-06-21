<<<<<<< HEAD
SRC=structureQuoridor.c
#SRC=$(wildcard *.c)  # en commentaire, je ne suis pas un grand amateur
EXE=structureQuoridor.out
=======
CFILES = $(wildcard *.c)
HFILES = $(wildcard *.h)
DFILES = $(wildcard *.d)
OFILES = $(patsubst %.c,%.o,$(CFILES))
GCC_FLAGS = -W -Wextra -Wshadow -Werror -Wall -g -Og -MMD
LD_FLAGS = -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lm -lSDL2
>>>>>>> 00e9cd73e8e79d591ba7b492fa2635d375545a57

CC=gcc
CFLAGS:=-Wall -Wextra -MMD -Og -g $(sdl2-config --cflags)
#CFLAGS:=-Wall -Wextra -MMD -O2 $(sdl2-config --cflags)  # pour la version release
LDFLAGS:=-lSDL2_image -lSDL2_ttf -lSDL2_gfx -lm -lSDL2

OBJ=$(addprefix build/,$(SRC:.c=.o))
DEP=$(addprefix build/,$(SRC:.c=.d))

all: $(OBJ)
	$(CC) -o $(EXE) $^ $(LDFLAGS)

build/%.o: %.c
	@mkdir -p build 
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf build core *.o

-include $(DEP)
