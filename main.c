#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "huffman.c"

int main()
{
    FILE *saida, *pont_arq = fopen("arquivo.txt", "rb");
    if (!pont_arq)
    {
        printf("Erro ao ler arquivo!\n");
        fclose(pont_arq);
        exit(1);
    }
    else
    {
        printf("Arquivo lido com sucesso!\n\n");
    }

    Queue *queue = createQueue();

    comprimir(pont_arq, queue, saida);
    //descomprimir

    fclose(pont_arq);
    return 0;
}