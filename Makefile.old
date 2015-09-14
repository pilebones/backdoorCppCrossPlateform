IS_WINDOWS=yes
DEBUG=yes
CC=g++

ifeq ($(IS_WINDOWS),yes)
	LDWINFLAGS=-lws2_32 -lwsock32
else
	LDWINFLAGS=-lws2_32 -lwsock32
endif

ifeq ($(DEBUG),yes)
	CFLAGS=-W -Wall -ansi -pedantic -g
	LDFLAGS=$(LDWINFLAGS) 
else
	CFLAGS=-W -Wall -ansi -pedantic
	LDFLAGS=$(LDWINFLAGS) 
endif

EXEC=client
SRC= $(wildcard *.cpp)
OBJ= $(SRC:.c=.o)

all: $(EXEC)
ifeq ($(DEBUG),yes)
	@echo "Génération en mode debug"
else
	@echo "Génération en mode release"
endif

client: $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)

main.o: client.h

%.o: %.cpp
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)