CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -MMD -Og -g -lm -fsanitize=address
CFILES = $(wildcard *.c)
HFILES = $(wildcard *.h)
DFILES = $(wildcard *.d)
OFILES = $(patsubst %.c,%.o,$(CFILES))
OUTFILES = $(wildcard *.out)
SDL2_CFLAGS = $(shell sdl2-config --cflags)
SDL2_LIBS = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf

SRCS = main.c LasVegas.c SDL.c MCTS.c
OBJS = $(SRCS:%.c=build/%.o)
DEPS = $(OBJS:.o=.d)

EXEC = main

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(SDL2_CFLAGS) -o $@ $(SDL2_LIBS) $(OBJS)

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(SDL2_CFLAGS) -c $< -o $@

clean:
	rm -rf build $(DFILES) $(OFILES) $(OUTFILES)

-include $(DEPS)