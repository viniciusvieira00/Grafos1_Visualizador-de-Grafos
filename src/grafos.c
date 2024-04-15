#include <stdio.h>
#include <stdlib.h>
///////////////////////////////////////
// VARIAVEIS GLOBAL PILHA
int *itens, topo, tam;
///////////////////////////////////////
void Inicia_Pilha(int n) {
    itens = (int *) malloc(n*sizeof(int));
    if (itens==NULL) {
    	printf("Erro alocacao memoria itens pilha");
    	exit(0);
	}
    tam = n;
    topo = 0;
}
void push(int valor) { // empilha
    itens [ topo ] = valor;
    topo++;
}
void pop()   { topo--; } // desempilha
int top()    { return itens[ topo-1 ]; } // consulta topo
int empty()  { return ( topo == 0 ); } // pilha vazia?
///////////////////////////////////////////////
typedef struct {
	char item;
	int visitado;
} NO;
///////////////////////////////////////
// VARIAVEIS GLOBAL GRAFO   
NO *VetorNos; // vetor de nós
int **MatAdj; // matriz de adjacência
int nNOs;     // numero de nós
///////////////////////////////////////////////
void Inicia_Grafo(int max) {
    // max = quantidade maxima de Nos
    int i, j;
    VetorNos = (NO *) malloc(max*sizeof(NO));
    if (VetorNos==NULL) {
    	printf("Erro alocacao memoria VetorNos");
    	exit(0);
	}
    MatAdj = (int **) malloc(max*sizeof(int *));
    if (MatAdj==NULL) {
    	printf("Erro alocacao memoria MatAdj");
    	exit(0);
	}
    for(i=0; i<max; i++) {
    	MatAdj[i] = (int *) malloc(max*sizeof(int));
    	if (MatAdj[i]==NULL) {
    		printf("Erro alocacao memoria MatAdj");
    		exit(0);
		}
	}
    
    for (j=0; j<max; j++) // inicializando matriz de adjacencia
        for (i=0; i<max; i++)
            MatAdj[i][j] = 0;
    nNOs = 0;
    Inicia_Pilha(max);
}
void insereNO(char v) {
    VetorNos[nNOs].item = v;
    VetorNos[nNOs].visitado = 0;
    nNOs++;
}
void insereAresta(int inicio, int fim) {
    MatAdj[inicio][fim] = 1;
    MatAdj[fim][inicio] = 1;
}
void mostraNo(int v) {
    printf("%c ",VetorNos[v].item);
}
// Retorna Nó que não foram visitados e são adjacentes ao Nó passado como parametro
int NoNaoVisitado(int i) {
	int j;
    for (j=0; j<nNOs; j++)
        if (MatAdj[i][j]==1 && VetorNos[j].visitado==0)
            return j;
    return -1;
}

  // Algoritmo Busca por profundidade (dfs)
  // Esse algoritmo possui um laço até que a pilha esteja vazia, onde são realizadas 4 tarefas:
  //  1- Examinar o Nó do topo da pilha, usando metodo top
  //  2- Tentar encontrar um vizinho não visitado desse Nó
  //  3- Se não encontrar um, desempilha
  //  4- Se encontrar o Nó, visita ele (marca como visitado) e empilha-o
void dfs() {
     VetorNos[0].visitado = 1; // Começa visitando o Nó zero
     mostraNo(0); // Exibe o Nó zero
     push(0); // Empilha o Nó zero
     while(!empty()) { // enquanto a pilha NAO esta vazia
        int v = NoNaoVisitado(top());
        if ( v == -1) // Se nao houver Nó NAO visitado
           pop(); // desempilha
        else {
           VetorNos[v].visitado = 1; // Visita o Nó
           mostraNo(v); // Exibe o Nó
           push(v); // Empilha o Nó
        }
     } // fim while
     // Aqui a pilha esta vazia, chegou no final
	 int i;
     for (i=0; i<nNOs; i++) // redefine flags para uso posterior se necessário
         VetorNos[i].visitado = 0;
}

// Função para destruir uma aresta
void destruirAresta(int inicio, int fim) {
    MatAdj[inicio][fim] = 0;
    MatAdj[fim][inicio] = 0;
}

// Função para destruir um nó
void destruirNo(int n) {
    // Antes de remover um nó, você precisa remover todas as arestas associadas a ele
    for (int i = 0; i < nNOs; i++) {
        destruirAresta(n, i);
        destruirAresta(i, n);
    }

    // Agora, realoca o VetorNos sem o nó destruído
    // Nota: isso requer o reposicionamento dos nós no vetor, o que pode ser custoso em termos de desempenho
    for (int i = n; i < nNOs - 1; i++) {
        VetorNos[i] = VetorNos[i + 1];
    }
    nNOs--; // Reduz o contador de nós
}

// Função para destruir o grafo
void destruirGrafo() {
    for (int i = 0; i < nNOs; i++) {
        free(MatAdj[i]); // Libera cada linha da matriz de adjacência
    }
    free(MatAdj); // Libera a matriz de adjacência
    free(VetorNos); // Libera o vetor de nós
    nNOs = 0; // Reseta o contador de nós
}

// Estrutura para uma fila
typedef struct {
    int items[100];
    int frente;
    int tras;
} Fila;

// Função para criar uma fila
void iniciaFila(Fila *f) {
    f->frente = -1;
    f->tras = -1;
}

// Função para verificar se a fila está vazia
int filaVazia(Fila *f) {
    return (f->frente == -1);
}

// Função para adicionar elemento na fila
void enfileirar(Fila *f, int valor) {
    if(f->frente == -1) {
        f->frente = 0;
    }
    f->tras++;
    f->items[f->tras] = valor;
}

// Função para remover elemento da fila
int desenfileirar(Fila *f) {
    int item;
    item = f->items[f->frente];
    f->frente++;
    if(f->frente > f->tras) {
        f->frente = f->tras = -1;
    }
    return item;
}

// Função BFS
void bfs(int verticeInicial) {
    Fila f;
    iniciaFila(&f);
    VetorNos[verticeInicial].visitado = 1;
    enfileirar(&f, verticeInicial);

    while(!filaVazia(&f)) {
        int verticeAtual = desenfileirar(&f);
        mostraNo(verticeAtual);

        int adjacente;
        while((adjacente = NoNaoVisitado(verticeAtual)) != -1) {
            VetorNos[adjacente].visitado = 1;
            enfileirar(&f, adjacente);
        }
    }

    // Redefine os nós como não visitados para uso posterior
    for (int i = 0; i < nNOs; i++)
        VetorNos[i].visitado = 0;
}

void main() {
    Inicia_Grafo(10);

    insereNO('A'); // 0
    insereNO('B'); // 1
    insereNO('C'); // 2
    insereNO('D'); // 3
    insereNO('E'); // 4
    insereNO('F'); // 5
    insereNO('G'); // 6
    insereNO('H'); // 7
    insereNO('I'); // 8

    insereAresta(0, 1); // AB
    insereAresta(1, 5); // BF
    insereAresta(5, 7); // FH
    insereAresta(0, 2); // AC
    insereAresta(0, 3); // AD
    insereAresta(3, 6); // DG
    insereAresta(6, 8); // GI
    insereAresta(0, 4); // AE

    printf("Visitada DFS: ");
    dfs(); // Busca em profundidade ou largura
    bfs(0); // Busca em largura
    printf("\n");

}
