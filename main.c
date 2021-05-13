/*
TRABALHO DE IMPLEMENTACAO DO CÓDIGO DE HUFFMAN (COMPRESSÃO E DESCOMPRESSÃO DE ARQUIVOS)
AUTOR: LUIZ GUSTAVO FALQUETO e ERIC RABELO
DATA CRIACAO: 12/05/2021
ARQUIVO: main.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "huffman.h"

int main()
{
    int op = 7;
    char arquivoentrada[50];
    char arquivosaida[50];

    while (op != 0)
    {
        printf("\nEscolha a operacao a qual deseja realizar:\n");
        printf("1 - Comprimir arquivo.\n");
        printf("2 - Exibir ocorrencias e codigo de cada caractere\n");
        printf("3 - Descomprimir arquivo.\n");
        printf("0 - Sair.\n");
        scanf("%d", &op);
        switch (op)
        {
        case 1:
            printf("Digite o nome do arquivo de entrada: ");
            scanf("%s", arquivoentrada);
            printf("Digite o nome do arquivo de saida: ");
            scanf("%s", arquivosaida);
            comprimir(arquivoentrada, arquivosaida);
            break;
        case 2:
            printf("As informacoes serao exibidas no seguinte formato:\n\ncaractere [ocorrencias]: sequencia de bits\n\n");
            printf("Digite o nome do arquivo de entrada: ");
            scanf("%s", arquivoentrada);
            exibirCaracteres(arquivoentrada);
            break;
        case 3:
            printf("Digite o nome do arquivo comprimido: ");
            scanf("%s", arquivoentrada);
            printf("Digite o nome do arquivo de saida: ");
            scanf("%s", arquivosaida);
            descomprimir(arquivoentrada, arquivosaida);
            break;
        case 0:
            op = 0;
            break;
        default:
            break;
        }
    }
    return 0;
}
