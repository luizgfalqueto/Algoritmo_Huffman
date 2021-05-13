/*
TRABALHO DE IMPLEMENTACAO DO CÓDIGO DE HUFFMAN (COMPRESSÃO E DESCOMPRESSÃO DE ARQUIVOS)
AUTOR: LUIZ GUSTAVO FALQUETO e ERIC RABELO
DATA CRIACAO: 12/05/2021
ARQUIVO: huffman.h
*/

#ifndef HUFFMAN_H
#define HUFFMAN_H

typedef struct nodeHuff // struct do nó utilizado tanto na fila de prioridades quanto na arvore
{
    unsigned char caracter;               // Caracter armazenado no nó
    int freq, bit;                        // Variaveis representando, respectivamente, a frequencia
                                          // do caracter no arquivo e a quantidade de bits ocupados no arquivo
    struct nodeHuff *left, *right, *next; // Ponteiros para os filhos da esquerda e direita (utilizados
                                          // pelo nó pai na arvore) e para o proximo nó (utilizado na fila)
    int *codigo;                          // Vetor que armazena o codigo do caracter do nó
    int pai;                              // Variavel que serve para definir se o nó na arvore é folha (possui um caracter do arquivo)
                                          // ou se é nó pai (usado na construção da arvore)
} NodeHuff;

typedef struct queueHuff // struct da fila de prioridades
{
    int sizeQueue;   // Variavel representando o tamanho da fila
    NodeHuff *first; // Ponteiro para o primeiro elemento da fila
} Queue;

NodeHuff *createNodehHuff(char c, int frequencia, int pai);

Queue *createQueue();

int heightTree(NodeHuff *raiz);

void freeQueue(Queue *queue);

int isEmptyQueue(Queue *queue);

void pushQueue(Queue *queue, NodeHuff *no);

NodeHuff *popQueue(Queue *queue);

NodeHuff *arvoreHuff(char *vetor);

void printQueue(Queue *queue);

void geraFrequencia(FILE *file, unsigned *vetor);

Queue *buildQueue(Queue *queue, unsigned *vetor);

Queue *buildTree(Queue *queue);

int geraBit(FILE *entrada, int posicao, unsigned char *aux);

void comprimir(const char *arquivoentrada, const char *arquivosaida);

void descomprimir(const char *arquivoentrada, const char *arquivosaida);

void geraCodigo(NodeHuff *node, NodeHuff *node2, int tamanho, int *v);

void exibirCaracteres(const char *arquivoentrada);

void destroyQueue(Queue *queue);

void auxDestroyQueue(NodeHuff *node);

void destroyTree(Queue *tree);

void auxDestroyTree(NodeHuff *node);

#endif //HUFFMAN_H