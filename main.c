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

    Queue *queue = createQueue();  //Essa queue vai ser a que vai ser usada para montar a árvore com os nós
    Queue *queue2 = createQueue(); //Essa queue vai servir apenas como fila, para guardar a sequencia de bit que o caracter vai ter

    comprimir(pont_arq, queue, queue2, saida);

    printf("\n\nFechando arquivos!\n");
    // fclose(saida);
    fclose(pont_arq);
    return 0;
}
