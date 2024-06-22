CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -MMD -Og -g
SDL2_CFLAGS = $(shell sdl2-config --cflags)
SDL2_LIBS = $(shell sdl2-config --libs) -lSDL2_image

SRCS = main.c structureQuoridor.c
OBJS = $(SRCS:%.c=build/%.o)
DEP = $(OBJS:.o=.d)
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(SDL2_LIBS)

build/%.o: %.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(SDL2_CFLAGS) -c $< -o $@

clean:
	rm -rf build core *.o $(EXEC)

-include $(DEP)
