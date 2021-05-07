#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

/*Função responsável por criar um nó
Recebe como parâmetros de entrada um char e uma frequência
Retorna o nó criado*/
NodeHuff *createNodeHuff(char c, int frequencia)
{
    NodeHuff *ptr;
    if (ptr = (NodeHuff *)malloc(sizeof(NodeHuff))) //Aloca espaço na memória para o nó
    {
        ptr->caracter = c;                         //ptr->caracter recebe o char passado como parâmetro
        ptr->freq = frequencia;                    //ptr->freq recebe a frequência passada como parâmetro
        ptr->left = ptr->right = ptr->next = NULL; //left, right e next são apontados para NULL
        return ptr;                                //o nó é retornado
    }
    else             //Caso ocorra erro na alocação
        return NULL; //Retorna NULL
}

/*Função responsável por criar a cabeça de uma fila*/
Queue *createQueue()
{
    Queue *queue = (Queue *)malloc(sizeof(Queue)); //Aloca espaço na memória para a fila

    if (queue) //Se o espaço foi alocado
    {
        queue->sizeQueue = 0; //Seu tamanho é definido como 0
        queue->first = NULL;  //Primeiro elemento aponta para NULL
        return queue;         //Retorna a fila
    }
    else //Caso ocorra erro na alocação
    {
        return NULL; //Retorna NULL
    }
}

/*Funcao responsável por verificar se a fila é vazia
Recebe como parâmetro de entrada o ponteiro para uma fila*/
int isEmptyQueue(Queue *queue)
{
    if (queue != NULL && queue->first == NULL) //Se a fila estiver alocada e seu primeiro elemento for nulo
    {
        return 1; // retorna 1
    }
    else //Caso o primeiro elemento da fila seja um elemento
    {
        return 0; //retorna 0
    }
}

int heightAvl(NodeHuff *raiz)
{
    if (!raiz)
        return -1; //-1 para compensar a contagem da queue
    int tam_esq, tam_dir;

    tam_esq = 1 + heightAvl(raiz->left);
    tam_dir = 1 + heightAvl(raiz->right);

    if (tam_dir > tam_esq)
        return tam_dir;
    else
        return tam_esq;
}

/*Função que insere elementos de forma ordenada em uma fila
Recebe como parâmetros de entrada o ponteiro pra uma fila e o ponteiro para o nó a ser inserido*/
void pushQueue(Queue *queue, NodeHuff *ptr)
{

    if (!queue || !ptr) //Se a fila passada for nula ou o elemento a ser inserido for nulo, apenas retorna
        return;

    if (isEmptyQueue(queue) == 1) //Se a fila estiver vazia
    {
        ptr->next = queue->first; //ptr->next vai apontar para o primeiro elemento da fila (NULL)
        queue->first = ptr;       //o primeiro elemento da fila vai se tornar o elemento que esta´sendo inserido
        queue->sizeQueue++;       //é incrementado o tamanho da fila
        return;
    }
    else //Caso a fila não for vazia
    {
        NodeHuff *aux = queue->first; //ponteiro auxiliar vai apontar para o primeiro elemento da fila

        if (aux->freq > ptr->freq) //Se a frequencia do elemento a ser adicionado (ptr) for menor que a frequenia do primeiro elemento, ele se torna o primeiro da fila
        {
            ptr->next = aux;    //ptr->next vai receber o aux
            queue->first = ptr; //o priemiro elemento da fila vai se tornar o ptr
            queue->sizeQueue++; //é incrementado o tamanho da fila
            return;
        }

        if (aux->freq == ptr->freq) //Se a frequencia do primeiro elemento da fila for igual a frequencia do elemento a ser adicionado (ptr), o novo elemento é adicionado logo após o primeiro
        {
            ptr->next = aux->next; //ptr->next vai apontar para o segundo elemento da fila
            aux->next = ptr;       //aux->next vai receber ptr
            queue->sizeQueue++;    //é incrementado o tamanho da fila
            return;
        }

        while (aux->next != NULL && aux->next->freq < ptr->freq) //Laço para percorrer toda a fila enquanto a frequencia do elemento a ser inserido (ptr) for maior que a frequencia de aux->next
        {
            aux = aux->next;       //aux recebe aux->next
            if (aux->next == NULL) //se aux é o ultimo elemento da fila
            {
                aux->next = ptr;    //ptr é adicionado ao final da fila
                queue->sizeQueue++; //é incrementado o tamanho da fila
                return;
            }
        }

        ptr->next = aux->next; //ptr->next vai apontar para aux->next
        aux->next = ptr;       //aux->next vai receber ptr
        queue->sizeQueue++;    //é incrementado o tamanho da fila
        return;
    }
}

/*Função responsável por reapontar uma fila, retirando seu primeiro elemento
Recebe como parâmetros de entrada o ponteiro para uma fila
Retorna o elemento retirado da fila*/
NodeHuff *popQueue(Queue *queue)
{
    if (!queue || isEmptyQueue(queue) == 1) //Se a fila for nula ou vazia, retorna NULL
        return NULL;

    NodeHuff *aux = queue->first; //Ponteiro aux aponta para o primeiro elemento da fila
    queue->first = aux->next;     //O primeiro elemento da fila vai receber aux->next
    queue->sizeQueue--;           //O tamanho da fila é decrementado
    aux->next = NULL;             //aux->next vai receber NULL
    return aux;                   //É retornado o elemento que foi retirado da fila
}

/*Funcao Responsável por imprimir uma fila
Recebe como parâmetros um ponteiro para uma fila*/
void printQueue(Queue *queue)
{
    if (!queue || isEmptyQueue(queue) == 1) //Se a fila for nula ou vazia
    {
        if (isEmptyQueue(queue) == 1) //Se a fila não possuir nenhum elemento
        {
            printf("queue vazia!\n");
        }
        printf("queue Nula\n"); //Se a fila não foi alocada
    }

    NodeHuff *ptr = queue->first; //Ponteiro ptr aponta para o priemiro elemento da fila

    while (ptr != NULL) //Enquanto ptr não apontar para NULL
    {
        printf("[%c | %d]\n", ptr->caracter, ptr->freq); //Imprime o caractere e a frequência do nó
        ptr = ptr->next;                                 //ptr recebe ptr->next
    }
}

/*Função responsável por realizar a leitura de todos os caracteres de um arquivo
Recebe como parâmetros de entrada o ponteiro pra um arquivo e um ponteiro para um vetor unsigned*/
void lerArquivo(FILE *file, unsigned *vetor)
{
    unsigned char aux;

    while (fscanf(file, "%c", &aux) != EOF)
    {
        vetor[(unsigned)aux]++;
    }
    rewind(file); //aponta o indicador para o começo do arquivo
}

/*Função responsável por criar a fila ordenada para depois criar a árvore de Huffman
Recebe como parâmetros de entrada o ponteiro pra uma fila vazia e um ponteiro para o vetor de caracteres
Retorna a fila ordenada*/
Queue *buildQueue(Queue *queue, unsigned *vetor)
{

    NodeHuff *novo;
    for (int i = 0; i < 256; i++) //Laço percorre os 256 possiveis caracteres
    {
        if (vetor[i] != 0) //Se o carácter foi usado alguma vez
        {
            novo = createNodeHuff(i, vetor[i]); //O nó é criado armazenando seu caracter e sua frequencia
            pushQueue(queue, novo);             //o nó é inserido na fila ordenadamente
        }
    }
    return queue; //Retorna a fila ordenada
}

/*Função responsável por criar a árvore de Huffman
Recebe como parâmetros de entrada uma lista já ordenada
Retorna uma fila contendo um único nó que é a árvore de Huffman*/
Queue *buildTree(Queue *queue)
{
    if (queue->first == NULL || queue->first->next == NULL) //Se a fila possuir apenas 1 elemento ou for vazia
    {
        printf("A fila nao tem no minimo 2 elementos para criar a arvore de Huffman");
        return queue; // a fila é retornada
    }

    NodeHuff *left, *right, *dad;
    while (queue->sizeQueue > 1) //Enquanto ouver mais de 1 elemento na fila
    {
        left = popQueue(queue);                              //Recebe o primeiro elemento da fila
        right = popQueue(queue);                             //Recebe o segundo elemento da fila (que na verdade se tornou o primeiro, pois a função pop retira o nó da fila)
        dad = createNodeHuff('+', left->freq + right->freq); //é criado um nó pai que terá a soma das frequencias de left e right
        dad->left = left;                                    //dad->left aponta para left
        dad->right = right;                                  //dad->right aponta para right
        pushQueue(queue, dad);                               //O nó pai é inserido na fila novamente
    }
    return queue; //É retornada a fila contendo um único nó que é a árvore de Huffman
}

void printTree(NodeHuff *raiz, int nivel)
{
    int i;
    if (raiz != NULL)
    {
        printTree(raiz->right, nivel + 1);
        for (i = 0; i < nivel; i++)
            printf("\t");
        printf("[%c | %d]\n", raiz->caracter, raiz->freq); //Imprime o caractere e a frequência do nó
        printTree(raiz->left, nivel + 1);
    }
}

int geraCodigo(FILE *saida, NodeHuff *raiz, unsigned c)
{
    if (!(raiz->left || raiz->right) && raiz->caracter == c)
    {
        putc('\0', saida);
        return 1;
    }
    else
    {
        int find = 0;

        if (raiz->left)
        {
            putc('0', saida);
            find = geraCodigo(saida, raiz->left, c);
        }

        if (find == 0 && raiz->right)
        {
            putc('1', saida);
            find = geraCodigo(saida, raiz->right, c);
        }

        if (find == 0)
        {
            putc('\0', saida);
        }
        return find;
    }
}

void comprimir(FILE *entrada, Queue *queue, FILE *saida)
{
    unsigned vetorC[256] = {0};

    lerArquivo(entrada, vetorC);

    queue = buildQueue(queue, vetorC);

    queue = buildTree(queue);

    unsigned char aux;

    saida = fopen("saida.txt", "wb");
    if (!saida)
    {
        printf("Erro ao ler arquivo!\n");
        fclose(saida);
        exit(1);
    }
    else
    {
        printf("Arquivo lido com sucesso!\n\n");
    }

    while (fscanf(entrada, "%c", &aux) != EOF)
    {
        geraCodigo(saida, queue->first, aux);
    }
    rewind(entrada); //aponta o indicador para o começo do arquivo
    fclose(saida);
}