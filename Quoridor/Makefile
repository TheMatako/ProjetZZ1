CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -MMD -Og -g -fsanitize=address
SDL2_CFLAGS = $(shell sdl2-config --cflags)
SDL2_LIBS = $(shell sdl2-config --libs) -lSDL2_image

SRCS = main.c structureQuoridor.c Rim/barrierInteraction.c
OBJS = $(SRCS:%.c=build/%.o)
DEPS = $(OBJS:.o=.d)

EXEC = main

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(SDL2_CFLAGS) $(OBJS) -o $@ $(SDL2_LIBS)

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(SDL2_CFLAGS) -c $< -o $@

clean:
	rm -rf build $(EXEC)

-include $(DEPS)

