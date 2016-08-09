
SOURCES=main.c
FLAGS=-lX11 -Wall -Wextra -std=c89 -pedantic -Wmissing-prototypes \
	  -Wstrict-prototypes -Wold-style-definition
OUTPUT=bnd

all: $(SOURCES)
	gcc $(SOURCES) $(FLAGS) -o $(OUTPUT)

clean:
	rm -fv $(OUTPUT)
