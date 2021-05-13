all: teste

teste: main.o huffman.o
	gcc -o teste main.o huffman.o

main.o: main.c huffman.h
	gcc -c -o main.o main.c -Wall

huffman.o: huffman.c
	gcc -c -o huffman.o huffman.c -Wall

clean:
	rm main.o huffman.o teste
