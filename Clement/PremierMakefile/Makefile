CFILES = $(wildcard *.c)
HFILES = $(wildcard *.h)
OFILES = $(patsubst %.c,%.o,$(CFILES))
GCC_FLAGS = -W -Wextra -Wshadow -Werror -Wall -g -Og -MMD
LD_FLAGS = -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lm -lSDL2 -fsanitize=address

all: main

%.o: %.c $(HFILES)
	gcc $(GCC_FLAGS) -c $<

main: $(OFILES) $(HFILES)
	gcc $(LD_FLAGS) -o main $(OFILES)

clean:
	-rm $(OFILES)