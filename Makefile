
SOURCES=main.c
FLAGS=-lX11 -Wall -Wextra -std=c89 -pedantic -Wmissing-prototypes \
	  -Wstrict-prototypes -Wold-style-definition

all: $(SOURCES)
	gcc $(SOURCES) $(FLAGS)

