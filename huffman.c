/*
TRABALHO DE IMPLEMENTACAO DO CÓDIGO DE HUFFMAN (COMPRESSÃO E DESCOMPRESSÃO DE ARQUIVOS)
AUTOR: LUIZ GUSTAVO FALQUETO e ERIC RABELO
DATA CRIACAO: 12/05/2021
ARQUIVO: huffman.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "huffman.h"

/*Função responsável por criar um nó
Recebe como parâmetros de entrada um char e uma frequência e um inteiro para representar se é pai
Retorna o nó criado*/
NodeHuff *createNodeHuff(char c, int frequencia, int pai)
{
    NodeHuff *ptr;
    if ((ptr = (NodeHuff *)malloc(sizeof(NodeHuff)))) //Aloca espaço na memória para o nó
    {
        ptr->caracter = c;                         //ptr->caracter recebe o char passado como parâmetro
        ptr->freq = frequencia;                    //ptr->freq recebe a frequência passada como parâmetro
        ptr->left = ptr->right = ptr->next = NULL; //left, right e next são apontados para NULL
        ptr->pai = pai;                            //ptr->pai indica se ele é um nó pai (soma do esq com dir) ou não
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

/*Funcao responsável calcular a altura de uma árvore
Recebe como parâmetro de entrada o ponteiro para um nó raiz
Retorna a altura da árvore*/
int heightTree(NodeHuff *raiz)
{
    if (!raiz)     //Se a raíz for nula
        return -1; //-1 para compensar a contagem da queue
    int tam_esq, tam_dir;

    tam_esq = 1 + heightTree(raiz->left);  //tamanho pela esquerda recebe 1 + o resultado da chamada recursiva pela esquerda
    tam_dir = 1 + heightTree(raiz->right); //tamanho pela esquerda recebe 1 + o resultado da chamada recursiva pela direita

    if (tam_dir > tam_esq) //Se o tamanho pela direita for maior
        return tam_dir;
    else //Se o tamanho pela esquerda for maior
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
void geraFrequencia(FILE *file, unsigned *vetor)
{
    unsigned char aux;

    while (fread(&aux, 1, 1, file) >= 1) //Realiza a leitura de caractere por caractere do arquivo
    {
        vetor[(unsigned char)aux]++;
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
            novo = createNodeHuff(i, vetor[i], 0); //O nó é criado armazenando seu caracter e sua frequencia
            pushQueue(queue, novo);                //o nó é inserido na fila ordenadamente
        }
    }
    return queue; //Retorna a fila ordenada
}

/*Função responsável por criar a árvore de Huffman
Recebe como parâmetros de entrada uma lista já ordenada
Retorna uma fila contendo um único nó que é a árvore de Huffman*/
Queue *buildTree(Queue *tree)
{
    if (tree->first == NULL || tree->first->next == NULL) //Se a fila possuir apenas 1 elemento ou for vazia
    {
        printf("A fila nao tem no minimo 2 elementos para criar a arvore de Huffman");
        return tree; // a fila é retornada
    }

    NodeHuff *left, *right, *dad;
    while (tree->sizeQueue > 1) //Enquanto ouver mais de 1 elemento na fila
    {
        left = popQueue(tree);                                  //Recebe o primeiro elemento da fila
        right = popQueue(tree);                                 //Recebe o segundo elemento da fila (que na verdade
                                                                //se tornou o primeiro, pois a função pop retira o nó da fila)
        dad = createNodeHuff('+', left->freq + right->freq, 1); //é criado um nó pai que terá a soma das frequencias
                                                                //de left e right
        dad->left = left;                                       //dad->left aponta para left
        dad->right = right;                                     //dad->right aponta para right
        pushQueue(tree, dad);                                   //O nó pai é inserido na fila novamente
    }
    return tree; //É retornada a fila contendo um único nó que é a árvore de Huffman
}

/*Função responsável por imprimir a árvore de Huffman
Recebe como parâmetros de entrada a raíz da árvore e a sua altura*/
void printTree(NodeHuff *raiz, int nivel)
{
    int i;
    if (raiz != NULL)
    {
        printTree(raiz->right, nivel + 1);
        for (i = 0; i < nivel; i++)
            printf("\t");
        if (raiz->pai == 0)
            printf("[%c | %d]\n", raiz->caracter, raiz->freq); //Imprime o caractere e a frequência do nó
        else
            printf("[%d]\n", raiz->freq);
        printTree(raiz->left, nivel + 1);
    }
}

/*Função responsável por exibir os caracteres, ocorrencias e sequencia de bits de um arquivo
Recebe como parâmetros de entrada o nome do arquivo de entrada que deseja ler*/
void exibirCaracteres(const char *arquivoentrada)
{
    //Realizando a abertura do arquivo de entrada
    FILE *entrada;
    entrada = fopen(arquivoentrada, "r");

    if (!entrada)
    {
        printf("Erro ao ler arquivo!\n");
        fclose(entrada);
        exit(1);
    }
    else
    {
        printf("Arquivo lido com sucesso!\n\n");
    }

    Queue *tree = createQueue();  //Essa queue vai ser a que vai ser usada para montar a árvore com os nós
    Queue *queue = createQueue(); //Essa queue vai servir apenas como fila, para guardar a sequencia de bit que
                                  //o caracter vai ter

    unsigned vetorC[256] = {0};      //Vetor que vai armazenar os caracteres e frequências
    geraFrequencia(entrada, vetorC); //Função que vai ler o arquivo de entrada e armazenar os caracteres e
                                     //frequências no vetor

    tree = buildQueue(tree, vetorC);   //Essa queue vai ser a que vai ser usada para montar a árvore com os nós
    tree = buildTree(tree);            //Constroi arvore com os nós da queue
    queue = buildQueue(queue, vetorC); //Essa queue vai servir apenas como fila, para guardar a sequencia de
                                       //bit que o caracter vai ter

    int tamanhovetor = 1;                                                //contador pra saber qual a posição do vetor salvar
    int *v = (int *)malloc((heightTree(tree->first) - 1) * sizeof(int)); //vetor usado na função define código para
                                                                         //guardar o percurso até o caractere
    geraCodigo(tree->first, queue->first, tamanhovetor, v);              //Define a sequencia de bits para cada caractere

    if (!queue || !queue->first) //Se a fila for vazia ou nula
        printf("Nao possui nenhum caracter ou a fila é nula");
    else
    {
        NodeHuff *aux = queue->first; //Ponteiro para um nó aponta para a raíz da fila
        while (aux != NULL)           //Enquanto o ponteiro não for nulo
        {
            printf("%c [%d]: ", aux->caracter, aux->freq); //Imprime o caractere e a frequencia
            for (int i = 0; i < aux->bit; i++)             //Laço para imprimir os valores contidos no vetor que armazena
                                                           //a sequencia de bits
            {
                printf("%d", aux->codigo[i]); //Imprime os valores correspondentes dos bits
            }
            printf("\n");
            aux = aux->next; //Ponteiro auxiliar aponta para o próximo
        }
    }
    free(v);             //Libera espaço da memória do vetor alocado anteriormente
    destroyQueue(queue); //Libera espaço de memória da queue usada para definir os codigos
    free(tree);          //Libera espaço de memoria para a queue usada para armazenar os codigos de bits
}

/*Função responsável por definir a sequencia de bits de cada caractere
Recebe como parâmetros de entrada o ponteiro pra uma fila, dois ponteiros para nós, uma variavel que guarda o tamanho e um vetor de inteiros*/
void geraCodigo(NodeHuff *node, NodeHuff *node2, int tamanho, int *v)
{
    if (!node) //se a árvore for vazia
        return;
    if (node->left) //se tiver filho a esquerda, chama de forma recursiva passando o proximo no a esquerda
    {
        v[tamanho - 1] = 0; //Recebe 0 na posição [tamanho-1] do vetor
        geraCodigo(node->left, node2, tamanho + 1, v);
    }
    if (node->right) //se tiver filho a direita, chama de forma recursiva passando o proximo no a direita
    {
        v[tamanho - 1] = 1; //Recebe 1 na posição [tamanho-1] do vetor
        geraCodigo(node->right, node2, tamanho + 1, v);
    }
    if (node->pai == 0) //se o nó for um caracter usado no arquivo
    {
        NodeHuff *aux = node2;                                       //Ponteiro para um nó auxiliar
        while (aux->next != NULL && node->caracter != aux->caracter) //Encontrando os caracteres correspondentes da arvore e da fila
        {
            aux = aux->next; //Ponteiro auxiliar aponta para o próximo
        }
        aux->codigo = (int *)malloc(tamanho * sizeof(int)); //Alocando espaço do vetor para a sequencia de bit correspondente
        for (int i = 0; i < tamanho - 1; i++)               //Laço responsável for percorrer o vetor v copiando as informações para o vetor de código
        {
            aux->codigo[i] = v[i]; //copiando o vetor
        }
        aux->bit = tamanho - 1; //Guardando o tamanho do vetor de bits
    }
    free(node);
    return;
}

/*Função responsável por calcular quantos bits são usados para comprimir o arquivo
Recebe como parâmetros de entrada o ponteiro para uma fila
Retorna o tamanho de bits*/
unsigned int getSizeCompressedText(Queue *queue)
{
    unsigned int size = 0;       //Variável que guarda a quantidade de bits
    if (!queue || !queue->first) //se a fila for vazia ou nula
        printf("Nao possui nenhum caracter ou a fila é nula");
    else
    {
        NodeHuff *aux = queue->first; //Ponteiro para um nó aponta para a raíz da fila
        while (aux != NULL)           //Enquanto o ponteiro não for nulo
        {
            size += aux->bit * aux->freq; //Size recebe a quantidade de bits de um caracter multiplicado pela freqûencia em que ele ocorre
            aux = aux->next;              //Ponteiro auxiliar aponta para o próximo
        }
    }
    return size; //Retorna a quantidade de bits
}

/*Função responsável por gerar o bit para busca do caractere na árvore
Recebe como parâmetros de entrada o ponteiro para um arquivo, um inteiro que grarda a posição e o ponteiro para um unsigned char
Retorna 1 ou 0*/
int geraBit(FILE *entrada, int posicao, unsigned char *aux)
{
    if (posicao % 8 == 0)
        fread(aux, 1, 1, entrada);            /*Se o resto da divisao de posição por 8 for 0, realiza a leitura de um bit
    Se o resultado for 0, retorna normal, caso nao for, retorna 1. 
    Através disso, é percorrida a árvore para saber qual caractere corresponde a sequencia de bit*/
    return !!((*aux) & (1 << (posicao % 8))); //Realiza o processo de movimentar o auxiliar no byte
}

/*Função responsável por realizar a leitura de um arquivo e comprimí-lo usando o método de Huffman
Recebe como parâmetros de entrada o nome do arquivo que deseja ler e o nome do arquivo que deseja salvar*/
void comprimir(const char *arquivoentrada, const char *arquivosaida)
{
    //Realizando a gravação do tempo gasto na operação
    clock_t inicio, final;
    double tempoGasto;
    inicio = clock();

    //Realizando a abertura dos arquivos de entrada e saída
    FILE *entrada, *saida;
    entrada = fopen(arquivoentrada, "r");
    saida = fopen(arquivosaida, "wb");

    if (!entrada || !saida)
    {
        printf("Erro ao ler arquivo!\n");
        fclose(entrada);
        exit(1);
    }
    else
    {
        printf("Arquivo lido com sucesso!\n\n");
    }

    Queue *tree = createQueue();  //Essa queue vai ser a que vai ser usada para montar a árvore com os nós
    Queue *queue = createQueue(); //Essa queue vai servir apenas como fila, para guardar a sequencia de bit que o caracter vai ter

    unsigned vetorC[256] = {0};                    //Vetor que vai armazenar os caracteres e frequências
    geraFrequencia(entrada, vetorC);               //Função que vai ler o arquivo de entrada e armazenar os caracteres e frequências no vetor
    fwrite(vetorC, 256, sizeof(vetorC[0]), saida); //Grava o vetor de caracteres e frequências no arquivo de saída
    fseek(saida, sizeof(unsigned int), SEEK_CUR);  //Move o ponteiro do stream para o tamanho de um unsigned int á frente, pois posteriormente,
                                                   //será salvo o tamanho de bits

    tree = buildQueue(tree, vetorC);   //Essa queue vai ser a que vai ser usada para montar a árvore com os nós
    tree = buildTree(tree);            //Constroi arvore com os nós da queue
    queue = buildQueue(queue, vetorC); //Essa queue vai servir apenas como fila, para guardar a sequencia de bit que o caracter vai ter

    int tamanhovetor = 1;                                                //contador pra saber qual a posição do vetor salvar
    int *v = (int *)malloc((heightTree(tree->first) - 1) * sizeof(int)); //vetor usado na função define código para guardar o percurso até o caractere
    geraCodigo(tree->first, queue->first, tamanhovetor, v);              //Define a sequencia de bits para cada caractere

    //Criando o vetor de saída do arqeuivo
    unsigned char aux = 0, c;
    unsigned tam = 0;
    NodeHuff *aux2;

    while (fread(&c, 1, 1, entrada) >= 1) //Enquanto houver texto a ser lido
    {
        aux2 = queue->first;
        while (aux2 != NULL) //Enquanto aux2 não for nulo
        {
            if (aux2->caracter == c) //Se os caracteres forem iguais
            {
                for (int j = 0; j < aux2->bit; j++) //Laço que percorre o vetor de códigos de aux2 movendo os bits para a posição necessária
                {
                    if (aux2->codigo[j] == 1) //Se o código for igual a 1, é necessário movimentar. Caso seja 0 não.
                    {
                        aux = aux | (1 << (tam % 8)); //Realiza o processo de movimentar o auxiliar no byte
                    }
                    tam++; //Aumenta o tamanho

                    //Se o resto da divisão de tam por 8 for 0, formou um byte, então salva no arquivo
                    if (tam % 8 == 0)
                    {
                        fwrite(&aux, 1, 1, saida); //Grava o byte no arquivo de saída
                        aux = 0;                   //Reseta a variável auxiliar
                    }
                }
                break; //se ja realizou a gravação do caractere, passa pro próximo
            }
            aux2 = aux2->next; //aux2 recebe aux2->next
        }
    }
    fwrite(&aux, 1, 1, saida); //Escreve no arquivo o que sobrou, caso nao tenha completado 1 byte

    fseek(saida, 256 * sizeof(unsigned int), SEEK_SET); //Move o ponteiro de stream para a posição que foi pulada anteriormente
    fwrite(&tam, 1, sizeof(unsigned), saida);           //Grava o tamanho de bits
    printf("Arquivo comprimido!\n");

    final = clock();
    tempoGasto = (double)(final - inicio) / CLOCKS_PER_SEC; //Calcula o tempo gasto para realizar a operação

    //Calcula o tamanho dos arquivos
    fseek(entrada, 0L, SEEK_END);
    double tamanhoEntrada = ftell(entrada);

    fseek(saida, 0L, SEEK_END);
    double tamanhoSaida = ftell(saida);

    printf("Arquivo de entrada: %s (%g bytes)\n", arquivoentrada, tamanhoEntrada / 1000);
    printf("Arquivo de saida: %s (%g bytes)\n", arquivosaida, tamanhoSaida / 1000);
    printf("Tempo gasto: %g segundos\n", tempoGasto);
    printf("Taxa de compressao: %d%%\n", (int)((100 * tamanhoSaida) / tamanhoEntrada));

    free(v);             //Libera espaço da memória do vetor alocado anteriormente
    free(tree);          //Libera espaço de memória da queue usada para definir os codigos
    destroyQueue(queue); //Libera espaço de memoria para a queue usada para armazenar os codigos de bits
    fclose(entrada);     //Fecha o arquivo de entrada
    fclose(saida);       //Fecha o arquivo de saída
}

/*Função responsável por realizar a leitura de um arquivo comprimido e retornar a forma original
Recebe como parâmetros de entrada o nome do arquivo que deseja ler e o nome do arquivo que deseja salvar*/
void descomprimir(const char *arquivoentrada, const char *arquivosaida)
{
    //Realiza a gravação do tempo gasto na operação
    clock_t inicio, final;
    double tempoGasto;
    inicio = clock();

    //Realiza a abertura dos arquivos de entrada e saída
    FILE *entrada, *saida;
    entrada = fopen(arquivoentrada, "rb");
    saida = fopen(arquivosaida, "wb");

    if (!entrada || !saida)
    {
        printf("Erro ao ler arquivo!\n");
        fclose(entrada);
        exit(1);
    }
    else
    {
        printf("Arquivo lido com sucesso!\n\n");
    }
    unsigned vetorC[256] = {0};                     //Vetor que irá conter os caracteres e frequências
    fread(vetorC, 256, sizeof(vetorC[0]), entrada); //Realiza a leitura do vetor guardado no arquivo
    unsigned tamanho;                               //Variável que contém o numero de bits
    fread(&tamanho, 1, sizeof(tamanho), entrada);   //Realiza a leitura da variável tamanho guardada no arquivo

    Queue *tree = createQueue();     //Cria uma fila
    tree = buildQueue(tree, vetorC); //Essa queue vai ser a que vai ser usada para montar a árvore com os nós
    tree = buildTree(tree);          //Transforma a queue em uma árvore de Huffman

    unsigned char aux = 0; //Variável auxiliar
    unsigned posicao = 0;  //Variável contadora
    NodeHuff *aux2;        //Cria um ponteiro para nó do tipo NodeHuff

    while (posicao < tamanho) //Enquanto a posição for menor que o tamanho de bits
    {
        aux2 = tree->first; //aux2 recebe a raiz da árvore

        while (aux2->left || aux2->right) //Enquanto o nó atual tiver filhos
        {
            if (geraBit(entrada, posicao++, &aux) == 1)
                aux2 = aux2->right; //Se a função geraBit retorna 1, aux2 anda pra direita
            else
                aux2 = aux2->left; //Senão, aux2 anda pra esquerda
        }

        fwrite(&(aux2->caracter), 1, 1, saida); //Se o nó for folha grava o caracter contido nele
    }
    printf("Arquivo Descomprimido!\n");

    final = clock();
    tempoGasto = (double)(final - inicio) / CLOCKS_PER_SEC; //Calcula o tempo gasto para realizar a operação

    //Calcula o tamanho dos arquivos
    fseek(entrada, 0L, SEEK_END);
    double tamanhoEntrada = ftell(entrada);

    fseek(saida, 0L, SEEK_END);
    double tamanhoSaida = ftell(saida);

    printf("Arquivo de entrada: %s (%g bytes)\n", arquivoentrada, tamanhoEntrada / 1000);
    printf("Arquivo de saida: %s (%g bytes)\n", arquivosaida, tamanhoSaida / 1000);
    printf("Tempo gasto: %g segundos\n", tempoGasto);
    printf("Taxa de descompressao: %d%%\n", (int)((100 * tamanhoSaida) / tamanhoEntrada));
    destroyTree(tree); //Libera espaço de memoria para a queue usada para montar a arvore de huffman
    fclose(entrada);   //Fecha o arquivo de entrada
    fclose(saida);     //Fecha o arquivo de saída
}

//Função que destroi nó por nó da fila
void auxDestroyQueue(NodeHuff *node)
{
    if (!node)
        return;
    auxDestroyQueue(node->next); // Recursão para o proximo nó
    free(node);                  // Libera o nó
    return;
}

//Função que destroi nó por nó da arvore
void auxDestroyTree(NodeHuff *node)
{
    if (!node)
        return;
    auxDestroyTree(node->left);  // Recursão para esquerda
    auxDestroyTree(node->right); // Recursão para a direita
    free(node);                  // Libera o nó
    return;
}

//Função que libera a cabeça da fila
void destroyQueue(Queue *queue)
{
    auxDestroyQueue(queue->first); //Libera todos os nós da fila
    free(queue);                   //Libera a fila
    return;
}

//Função que libera a raiz da arvore
void destroyTree(Queue *tree)
{
    auxDestroyQueue(tree->first); //Libera todos os nós da arvore
    free(tree);                   //Libera a fila
    return;
}