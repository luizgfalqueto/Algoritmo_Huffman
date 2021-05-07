#ifndef HUFFMAN_H
#define HUFFMAN_H

typedef struct nodeHuff
{
    char caracter;
    int freq;
    struct nodeHuff *left, *right, *next;
} NodeHuff;

typedef struct queueHuff
{
    int sizeQueue;
    NodeHuff *first;
} Queue;

NodeHuff *createNodehHuff(char c, int frequencia);

Queue *createQueue();

void freeQueue(Queue *queue);

int isEmptyQueue(Queue *queue);

int findQueue(Queue *queue, char *c);

void pushQueue(Queue *queue, NodeHuff *no);

NodeHuff *popQueue(Queue *queue);

NodeHuff *arvoreHuff(char *vetor);

void printQueue(Queue *queue);

void lerArquivo(FILE *file, unsigned *vetor);

Queue *buildQueue(Queue *queue, unsigned *vetor);

Queue *buildTree(Queue *queue);

void comprimir(FILE *entrada, Queue *queue, FILE *saida);

int geraCodigo(FILE *saida, NodeHuff *raiz, unsigned c);

#endif //HUFFMAN_H