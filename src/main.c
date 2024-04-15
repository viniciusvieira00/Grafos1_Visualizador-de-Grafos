#define _GNU_SOURCE

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<assert.h>

#include<windows.h>

#include<pthread.h>

struct point
{
	double x, y, z;
	struct point **connection;
	unsigned char connection_count;

	char item;
	int visited;
};
struct point *point(double x, double y, double z)
{
	struct point *self;
	assert(self=calloc(1, sizeof(struct point)));
	self->x=x;
	self->y=y;
	self->z=z;
	return self;
}
void point_connect(struct point *self, struct point *connection_point)
{
	assert(self);
	assert(connection_point);
	assert(++self->connection_count);
	assert(self->connection=realloc(self->connection, self->connection_count*sizeof(struct point*)));
	self->connection[self->connection_count-1]=connection_point;
}

void *pixel_data=NULL;
RECT client_rectangle;
void pixel_data_set(int row, int col, COLORREF rgb)
{
    if(pixel_data)
    {
        COLORREF *pixels=(COLORREF*)pixel_data;
        size_t index=(size_t)((row*client_rectangle.right)+col);
        if(index<(size_t)client_rectangle.right*client_rectangle.bottom)
            pixels[index]=rgb;
    }
}
void pixel_data_bresenham_line(int x0, int y0, int x1, int y1, COLORREF color)
{
    int dx=abs(x1-x0), sx=x0<x1?1:-1;
    int dy=-abs(y1-y0), sy=y0<y1?1:-1; 
    int err=dx+dy, e2;
    for(;;)
    {
        pixel_data_set(x0, y0, color);
        if(x0==x1&&y0==y1)break;
        e2=2*err;
        if(e2>=dy){err+=dy;x0+=sx;}
        if(e2<=dx){err+=dx;y0+=sy;}
    }
}
void draw_point(struct point *self)
{
	assert(self);
	struct point_2d
	{
		double x, y;
	}
	projection_3d(double x, double y, double z)
	{
	    const double z_near = 1.0;
	    const double z_far = 1000.0;
	    const double fov = M_PI / 4;
	    const double aspect_ratio = (double)client_rectangle.right / (double)client_rectangle.bottom;
	    double f = 1.0 / tan(fov / 2.0);
	    double a = z_near + z_far / (z_near - z_far);
	    double b = (2.0 * z_near * z_far) / (z_near - z_far);
	    double x_proj = f / aspect_ratio * x / -z;
	    double y_proj = f * y / -z;
	    struct point_2d result;
	    result.x = (int)((x_proj + 1) * client_rectangle.right / 2);
	    result.y = (int)((1 - y_proj) * client_rectangle.bottom / 2);
	    return result;
	}
	struct point_2d self_projected=projection_3d(self->x, self->y, self->z);
	for(unsigned char connection_index=0; connection_index<self->connection_count; connection_index++)
	{
		struct point *connection_point=self->connection[connection_index];
		struct point_2d connection_projected=projection_3d(connection_point->x, connection_point->y, connection_point->z);
		pixel_data_bresenham_line(self_projected.x, self_projected.y, connection_projected.x, connection_projected.y, RGB(255, 255, 255));
	}
}

void *cli_thread_function(void *arg)
{
	puts("   _____                 _      __      ___                 _ _              ");
	puts("  / ____|               | |     \\ \\    / (_)               | (_)             ");
	puts(" | |  __ _ __ __ _ _ __ | |__    \\ \\  / / _ _____   _  __ _| |_ _______ _ __ ");
	puts(" | | |_ | '__/ _` | '_ \\| '_ \\    \\ \\/ / | |_  / | | |/ _` | | |_  / _ \\ '__|");
	puts(" | |__| | | | (_| | |_) | | | |    \\  /  | |/ /| |_| | (_| | | |/ /  __/ |   ");
	puts("  \\_____|_|  \\__,_| .__/|_| |_|     \\/   |_/___|\\__,_|\\__,_|_|_/___\\___|_|   ");
	puts("                  | |                                                        ");
	puts("                  |_|                                                        ");
	puts("Seja bem vindo ao vizualizador de grafos!");
	puts("Ele utiliza o algoritmo DFS e muito mais!");
	puts("Comandos:");
	puts("\'exit\' - terminar o programa");
	puts("\'cubo\' - exemplo de um grafo em formato de cubo");
	puts("\'tetrahedro\' - exemplo de um grafo em formato de tetrahedro");
	puts("\'create\' - criar um grafo");
	char string_buffer[256];
	while(scanf("%s", &string_buffer))
		if(!strcmp(string_buffer, "exit"))
		{

		}
		else if(!strcmp(string_buffer, "cubo"))
		{

		}
		else if(!strcmp(string_buffer, "tetrahedro"))
		{
			
		}
		else if(!strcmp(string_buffer, "create"))
		{
			int *itens, topo, tam;
			void Inicia_Pilha(int n)
			{
			    assert(itens =malloc(n*sizeof(int)));
			    tam = n;
			    topo = 0;
			}
			void push(int valor)
			{
			    itens [ topo ] = valor;
			    topo++;
			}
			void pop()   { topo--; }
			int top()    { return itens[ topo-1 ]; }
			int empty()  { return ( topo == 0 ); }
			typedef struct
			{
				char item;
				int visitado;
			} NO;
			NO *VetorNos;
			int **MatAdj;
			int nNOs;
			void Inicia_Grafo(int max)
			{
			    int i, j;
			    VetorNos =malloc(max*sizeof(NO));
			    MatAdj = (int **) malloc(max*sizeof(int *));
			    for(i=0; i<max; i++)
			    	MatAdj[i] = (int *) malloc(max*sizeof(int));
			    for (j=0; j<max; j++)
			        for (i=0; i<max; i++)
			            MatAdj[i][j] = 0;
			    nNOs = 0;
			    Inicia_Pilha(max);
			}
			void insereNO(char v)
			{
			    VetorNos[nNOs].item = v;
			    VetorNos[nNOs].visitado = 0;
			    nNOs++;
			}
			void insereAresta(int inicio, int fim)
			{
				if(inicio<0||fim<0||inicio>=nNOs||fim>=nNOs)
					return;
			    MatAdj[inicio][fim] = 1;
			    MatAdj[fim][inicio] = 1;
			}
			void mostraNo(int v)
			{
			    printf("%c ", VetorNos[v].item);
			}
			// Retorna Nó que nao foram visitados e sao adjacentes ao Nó passado como parametro
			int NoNaoVisitado(int i)
			{
				int j;
			    for (j=0; j<nNOs; j++)
			        if (MatAdj[i][j]==1 && VetorNos[j].visitado==0)
			            return j;
			    return -1;
			}

			struct point **points=NULL;
			unsigned int point_count=0;
			  // Algoritmo Busca por profundidade (dfs)
			  // Esse algoritmo possui um laco atd que a pilha esteja vazia, onde sao realizadas 4 tarefas:
			  //  1- Examinar o Nó do topo da pilha, usando metodo top
			  //  2- Tentar encontrar um vizinho nao visitado desse Nó
			  //  3- Se nao encontrar um, desempilha
			  //  4- Se encontrar o Nó, visita ele (marca como visitado) e empilha-o
			void dfs()
			{
			     VetorNos[0].visitado = 1; // Comeca visitando o Nó zero
			     //mostraNo(0); // Exibe o Nó zero
			     //draw_point(points[0]);
			     push(0); // Empilha o Nó zero
			     while(!empty()) { // enquanto a pilha NAO esta vazia
			        int v = NoNaoVisitado(top());
			        if ( v == -1) // Se nao houver Nó NAO visitado
			           pop(); // desempilha
			        else {
			           VetorNos[v].visitado = 1; // Visita o Nó
			           //mostraNo(v); // Exibe o Nó
			           push(v); // Empilha o Nó
			           //draw_point(points[v]);
			        }
			     } // fim while
			     // Aqui a pilha esta vazia, chegou no final
				 int i;
			     for (i=0; i<nNOs; i++) // redefine flags para uso posterior se necessario
			         VetorNos[i].visitado = 0;
			}
			// Funcao para destruir uma aresta
			void destruirAresta(int inicio, int fim)
			{
			    MatAdj[inicio][fim] = 0;
			    MatAdj[fim][inicio] = 0;
			}
			// Funcao para destruir um nó
			void destruirNo(int n)
			{
			    // Antes de remover um nó, você precisa remover todas as arestas associadas a ele
			    for (int i = 0; i < nNOs; i++)
			    {
			        destruirAresta(n, i);
			        destruirAresta(i, n);
			    }

			    // Agora, realoca o VetorNos sem o nó destruído
			    // Nota: isso requer o reposicionamento dos nós no vetor, o que pode ser custoso em termos de desempenho
			    for (int i = n; i < nNOs - 1; i++)
				    VetorNos[i] = VetorNos[i + 1];
			    nNOs--; // Reduz o contador de nós
			}
			// Funcao para destruir o grafo
			void destruirGrafo()
			{
			    for (int i = 0; i < nNOs; i++) {
			        free(MatAdj[i]); // Libera cada linha da matriz de adjacência
			    }
			    free(MatAdj); // Libera a matriz de adjacência
			    free(VetorNos); // Libera o vetor de nós
			    nNOs = 0; // Reseta o contador de nós
			}

			// Estrutura para uma fila
			typedef struct
			{
			    int items[100];
			    int frente;
			    int tras;
			} Fila;
			// Funcao para criar uma fila
			void iniciaFila(Fila *f)
			{
			    f->frente = -1;
			    f->tras = -1;
			}
			// Funcao para verificar se a fila esta vazia
			int filaVazia(Fila *f)
			{
			    return (f->frente == -1);
			}
			// Funcao para adicionar elemento na fila
			void enfileirar(Fila *f, int valor)
			{
			    if(f->frente == -1) {
			        f->frente = 0;
			    }
			    f->tras++;
			    f->items[f->tras] = valor;
			}
			// Funcao para remover elemento da fila
			int desenfileirar(Fila *f)
			{
			    int item;
			    item = f->items[f->frente];
			    f->frente++;
			    if(f->frente > f->tras) {
			        f->frente = f->tras = -1;
			    }
			    return item;
			}
			puts("Digite o maximo numero de vertices desejado:");
			int maximum_vertice_count;
			if (!fgets(string_buffer, sizeof(string_buffer), stdin) || sscanf(string_buffer, "%d", &maximum_vertice_count) != 1) {
		        puts("Entrada inválida.");
		        return 1; // Handle error appropriately
		    }
		    Inicia_Grafo(maximum_vertice_count);

		    puts("Digite três números flutuantes por linha, interrupção com entrada não-numérica:");
		    double x, y, z;
		    while (1) {
		        if (!fgets(string_buffer, sizeof(string_buffer), stdin)) break;
		        if (sscanf(string_buffer, "%lf %lf %lf", &x, &y, &z) != 3) break;
		        points = realloc(points, (point_count + 1) * sizeof(struct point *));
		        points[point_count] = point(x, y, z);
		        insereNO(point_count);  // Assumes vertices are numbered from 0 upwards
		        printf("Vértice %u criado\n", point_count);
		        point_count++;
		    }

		    puts("Digite dois números positivos inteiros por linha, interrupção com entrada não-numérica:");
		    unsigned int v0, v1;
		    while (fgets(string_buffer, sizeof(string_buffer), stdin)) {
		        if (sscanf(string_buffer, "%u %u", &v0, &v1) != 2) break;
		        if (v0 < maximum_vertice_count && v1 < maximum_vertice_count) {
		            insereAresta(v0, v1);
		        } else {
		            puts("Número de vértice inválido.");
		        }
		    }
			//dfs();
			puts("Entre algum comando");
		}
		else
			puts("Comando nao encontrado");
	return NULL;
}
int main(int argc, char **argv)
{
	assert(argc==1);
	assert(AllocConsole());
	assert(freopen("CONOUT$", "wb", stdout));
	assert(freopen("CONIN$", "rb", stdin));
	pthread_t cli_thread;
	assert(!pthread_create(&cli_thread, NULL, cli_thread_function, NULL));
    HDC hdcMem;
	HBITMAP hBitmap;
	WNDCLASS window_class={};
	window_class.lpfnWndProc=({
		LRESULT CALLBACK window_procedure(HWND window_handle, UINT message, WPARAM wide_parameter, LPARAM long_parameter)
		{
			switch(message)
			{
				case WM_CREATE:
					GetClientRect(window_handle, &client_rectangle);
				    assert(pixel_data=realloc(pixel_data, client_rectangle.right*client_rectangle.bottom*sizeof(COLORREF)));
				    memset(pixel_data, 0, client_rectangle.right*client_rectangle.bottom*sizeof(COLORREF));
				    HDC device_handle=GetDC(window_handle);
				    hdcMem=CreateCompatibleDC(device_handle);
				    hBitmap=CreateCompatibleBitmap(device_handle, client_rectangle.right, client_rectangle.bottom);
				    SelectObject(hdcMem, hBitmap);
				    ReleaseDC(window_handle, device_handle);
			        break;
				case WM_PAINT:
					PAINTSTRUCT paint_struct;
				    HDC hdc=BeginPaint(window_handle, &paint_struct);
				    SetDIBits(hdcMem, hBitmap, 0, client_rectangle.bottom, pixel_data,
				              &(BITMAPINFO){.bmiHeader={
				                  .biSize=sizeof(BITMAPINFOHEADER),
				                  .biWidth=client_rectangle.right,
				                  .biHeight=-client_rectangle.bottom,
				                  .biPlanes=1,
				                  .biBitCount=32,
				                  .biCompression=BI_RGB
				              }}, DIB_RGB_COLORS);
				    BitBlt(hdc, 0, 0, client_rectangle.right, client_rectangle.bottom, hdcMem, 0, 0, SRCCOPY);
				    EndPaint(window_handle, &paint_struct);
		        	break;
				case WM_SIZE:
					assert(GetClientRect(window_handle, &client_rectangle));
    				assert(pixel_data=realloc(pixel_data, client_rectangle.right*client_rectangle.bottom*sizeof(COLORREF)));
					break;
				case WM_CLOSE:
					assert(DestroyWindow(window_handle));
					break;
				case WM_DESTROY:
					if(hdcMem)assert(DeleteDC(hdcMem));
		            if(hBitmap)assert(DeleteObject(hBitmap));
		            if(pixel_data)free(pixel_data);
					PostQuitMessage(0);
					break;
				default:
					return DefWindowProc(window_handle, message, wide_parameter, long_parameter);
			}
			return 0;
		};
		window_procedure;
	});
	assert(window_class.hInstance=GetModuleHandle(NULL));
	window_class.lpszClassName="window_name";
	assert(RegisterClass(&window_class));
	HWND window_handle;
	assert(window_handle=CreateWindow(
    window_class.lpszClassName,
    window_class.lpszClassName,
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT,
    NULL,   
    NULL,
    window_class.hInstance,
    NULL
    ));
    assert(!ShowWindow(window_handle, SW_SHOW));
    assert(GetClientRect(window_handle, &client_rectangle));
    assert(MoveWindow(window_handle, 500, 250, client_rectangle.right, client_rectangle.bottom, FALSE));
   	MSG message;
   	BOOL GetMessage_return;
	while(GetMessage_return=GetMessage(&message, NULL, 0, 0))
	{
		assert(GetMessage_return!=-1);
	    TranslateMessage(&message);
	    DispatchMessage(&message);
	}
	assert(!pthread_join(cli_thread, NULL));
	assert(FreeConsole());
	return 0;
}