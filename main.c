#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "huffmanOrganized.c"

int main()
{
    printf("Começando programa...\n");

    compress("arquivo.txt", "saida.hx");
    decompress("saida.hx", "saidaDescomprimida.txt");

    printf("\n\nFechando arquivos!\n");

    printf("\n\nEncerrando programa!\n");
    return 0;
}
