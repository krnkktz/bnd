
SOURCES_C=client.c
SOURCES_D=daemon.c
FLAGS=-lX11 -Wall -Wextra -std=c89 -pedantic -Wmissing-prototypes \
	  -Wstrict-prototypes -Wold-style-definition
OUTPUT_C=bnd
OUTPUT_D=bndd

all: client daemon

client: $(SOURCES_C)
	gcc $(SOURCES_C) $(FLAGS) -o $(OUTPUT_C)

daemon: $(SOURCES_D)
	gcc $(SOURCES_D) $(FLAGS) -o $(OUTPUT_D)

clean:
	rm -fv $(OUTPUT_C) $(OUTPUT_D)

