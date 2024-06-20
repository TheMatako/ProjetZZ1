CFILES = $(wildcard *.c)
HFILES = $(wildcard *.h)
DFILES = $(wildcard *.d)
OFILES = $(patsubst %.c,%.o,$(CFILES))
GCC_FLAGS = -W -Wextra -Wshadow -Werror -Wall -g -Og -MMD
LD_FLAGS = -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lm -lSDL2 -fsanitize=address

all: structureQuoridor

%.o: %.c $(HFILES)
	gcc $(GCC_FLAGS) -c $<

structureQuoridor: $(OFILES) $(HFILES)
	gcc $(LD_FLAGS) -o x $(OFILES)

clean:
	-rm $(OFILES) $(DFILES)