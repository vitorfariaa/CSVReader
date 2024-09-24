parametrosCompilacao = -Wall -lm
nomePrograma=csvreader

all: $(nomePrograma)

$(nomePrograma): csvreader.o io.o
	gcc -o $(nomePrograma) csvreader.o io.o $(parametrosCompilacao)

csvreader.o: csvreader.c 
	gcc -c csvreader.c $(parametrosCompilacao)

io.o: io.h io.c
	gcc -c io.c $(parametrosCompilacao)

clean:
	rm -f *.o

purge:
	rm -f *.o *~ *.gch $(nomePrograma)

.PHONY: purge clean
