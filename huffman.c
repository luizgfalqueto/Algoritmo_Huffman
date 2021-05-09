#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
        if (raiz->caracter != '+')
            printf("[%c | %d]\n", raiz->caracter, raiz->freq); //Imprime o caractere e a frequência do nó
        else
            printf("[%d]\n", raiz->freq);
        printTree(raiz->left, nivel + 1);
    }
}

void defineCodigo(Queue *queue, NodeHuff *node, NodeHuff *node2, int tamanho, int *v)
{
    if (queue->first != NULL)
    { //se a árvore nao for vazia
        if (node->left)
        { //se tiver filho a esquerda, chama de forma recursiva passando o proximo no a esquerda
            v[tamanho - 1] = 0;
            defineCodigo(queue, node->left, node2, tamanho + 1, v);
        }
        if (node->right)
        { //se tiver filho a direita, chama de forma recursiva passando o proximo no a direita
            v[tamanho - 1] = 1;
            defineCodigo(queue, node->right, node2, tamanho + 1, v);
        }
        else
        { //caso seja um nó folha
            if (node->caracter != '+')
            { //se o nó for um caracter usado no arquivo
                NodeHuff *aux = node2;
                while (aux->next != NULL && node->caracter != aux->caracter)
                { //Encontrando os caracteres correspondentes da arvore e da fila
                    aux = aux->next;
                }
                aux->codigo = (int *)malloc(tamanho * sizeof(int)); //Alocando espaço do vetor para a sequencia de bit correspondente
                for (int i = 0; i < tamanho - 1; i++)
                {
                    aux->codigo[i] = v[i]; //copiando o vetor
                }
                aux->bit = tamanho - 1; //Guardando o tamanho do vetor de bits
                free(node);
                return;
            }

            if (node->caracter == '+')
            { //se o caracter nao for usado no arquivo
                free(node);
                return;
            }
        }
    }
    //free(v);
    return;
}

void exibirCaracteres(Queue *queue)
{
    if (!queue || !queue->first)
        printf("Nao possui nenhum caracter ou a fila é nula");
    else
    {
        NodeHuff *aux = queue->first;
        while (aux != NULL)
        {
            printf("%c [%d]: ", aux->caracter, aux->freq);
            for (int i = 0; i < aux->bit; i++)
            {
                printf("%d", aux->codigo[i]);
            }
            printf("\n");
            aux = aux->next;
        }
    }
}

void comprimir(FILE *entrada, Queue *queue, Queue *queue2, FILE *saida)
{
    unsigned vetorC[256] = {0};
    int tamanho = 1;                               //contador pra saber quanto de memória reallocar
    int *v = (int *)malloc(tamanho * sizeof(int)); //vetor usado na função define código para guardar o percurso até o caractere

    lerArquivo(entrada, vetorC);

    queue = buildQueue(queue, vetorC); //Essa queue vai ser a que vai ser usada para montar a árvore com os nós
    queue = buildTree(queue);

    queue2 = buildQueue(queue2, vetorC); //Essa queue vai servir apenas como fila, para guardar a sequencia de bit que o caracter vai ter

    printTree(queue->first, heightAvl(queue->first));

    defineCodigo(queue, queue->first, queue2->first, tamanho, v);

    printQueue(queue2);
    printf("Terminou de ler os codigos\n");

    exibirCaracteres(queue2);
}