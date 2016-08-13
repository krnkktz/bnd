CC=gcc
FLAGS=-lX11 -Wall -Wextra -std=c89 -pedantic -Wmissing-prototypes \
	  -Wstrict-prototypes -Wold-style-definition -D_POSIX_C_SOURCE

SOURCES_C=client.c
SOURCES_D=daemon.c notify.c log.c

OUTPUT_C=bnd
OUTPUT_D=bndd

all: $(OUTPUT_C) $(OUTPUT_D)


$(OUTPUT_C): $(SOURCES_C)
	$(CC) $(SOURCES_C) $(FLAGS) -o $(OUTPUT_C)

$(OUTPUT_D): $(SOURCES_D)
	$(CC) $(SOURCES_D) $(FLAGS) -o $(OUTPUT_D)




clean:
	@rm -fv $(OUTPUT_C) $(OUTPUT_D)

install-client: $(OUTPUT_C)
	@cp -v $(OUTPUT_C) /usr/local/bin/bnd

install-daemon: $(OUTPUT_D)
	@cp -v $(OUTPUT_D) /usr/local/bin/bndd

install: install-client install-daemon

