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
    	printf("%d %d, %d %d\n", row, col, client_rectangle.right, client_rectangle.bottom);
        COLORREF *pixels=(COLORREF*)pixel_data;
        unsigned int index=(row*client_rectangle.right)+col;
        printf("%d/%d\n", index, client_rectangle.right*client_rectangle.bottom);
        if(col >= 0 && col < client_rectangle.right && row >= 0 && row < client_rectangle.bottom)
	       pixels[index]=rgb;
    }
}
void pixel_data_bresenham_line(int x0, int y0, int x1, int y1, COLORREF color)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1; 
    int err = dx + dy, e2;
    for (;;) {
        pixel_data_set(y0, x0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}
void point_draw(struct point *self)
{
	assert(self);
	struct point_2d
	{
		double x, y;
	}
	projection_3d(double x, double y, double z)
	{
		printf("==== %lf %lf %lf\n", x, y, z);
	    const double z_near = 0.1;  // Minimum view distance
    const double fov = M_PI / 4;  // Field of view (45 degrees)
    const double aspect_ratio = (double)client_rectangle.right / client_rectangle.bottom;
    
    // Ensure z is above a minimal distance to avoid extreme projections
    if (z < z_near) {
        z = z_near;
    }

    // Calculate projection
    double f = 1.0 / tan(fov / 2.0);  // Focal length based on field of view
    double x_proj = (f / aspect_ratio) * x / z;
    double y_proj = f * y / z;

    // Convert from normalized device coordinates (NDC) to screen coordinates
    struct point_2d result;
    result.x = (x_proj + 1) * client_rectangle.right / 2;
    result.y = (1 - y_proj) * client_rectangle.bottom / 2;

    // Ensure projected points are within the screen bounds (optional depending on your need)
    result.x = max(0, min(result.x, client_rectangle.right - 1));
    result.y = max(0, min(result.y, client_rectangle.bottom - 1));
		printf("==== %lf %lf\n", result.x, result.y);
	    return result;
	}
	printf("**** %lf %lf %lf\n", self->x, self->y, self->z);
	struct point_2d self_projected=projection_3d(self->x, self->y, self->z);
	pixel_data_set(self_projected.y, self_projected.x, RGB(255, 255, 255));
	for(unsigned char connection_index=0; connection_index<self->connection_count; connection_index++)
	{
		struct point *connection_point=self->connection[connection_index];
		struct point_2d connection_projected=projection_3d(connection_point->x, connection_point->y, connection_point->z);
		printf("%lf %lf %lf %lf\n", self_projected.x, self_projected.y, connection_projected.x, connection_projected.y);
		pixel_data_bresenham_line(self_projected.x, self_projected.y, connection_projected.x, connection_projected.y, RGB(255, 255, 255));
	}
}

void *cli_thread_function(void *arg)
{
	assert(arg);
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
	puts("\'criar\' - criar um grafo");
	char string_buffer[256];
	while(scanf("%s", &string_buffer))
		if(!strcmp(string_buffer, "exit"))
		{
			DestroyWindow(*(HWND*)arg);
		}
		else if(!strcmp(string_buffer, "cubo"))
		{
			int *itens, topo, tam;
			void Inicia_Pilha(int n)
			{
			    assert(itens =malloc(n*sizeof(int)));
			    tam=n;
			    topo=0;
			}
			void push(int valor)
			{
			    itens [ topo ]=valor;
			    topo++;
			}
			void pop(){topo--;}
			int top(){return itens[topo-1];}
			int empty(){ return ( topo == 0 );}
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
			    MatAdj=malloc(max*sizeof(int *));
			    for(i=0; i<max; i++)
			    	MatAdj[i]=malloc(max*sizeof(int));
			    for (j=0; j<max; j++)
			        for (i=0; i<max; i++)
			            MatAdj[i][j]=0;
			    nNOs=0;
			    Inicia_Pilha(max);
			}
			void insereNO(char v)
			{
			    VetorNos[nNOs].item=v;
			    VetorNos[nNOs].visitado=0;
			    nNOs++;
			}
			void insereAresta(int inicio, int fim)
			{
				if(inicio<0||fim<0||inicio>=nNOs||fim>=nNOs)
					return;
				printf("inicio:%d fim:%d\n", inicio, fim);
			    MatAdj[inicio][fim]=1;
			    MatAdj[fim][inicio]=1;
			}
			void mostraNo(int v)
			{
			    printf("%c ", VetorNos[v].item);
			}
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
			struct point **drawing_points=NULL;
			unsigned int drawing_point_count=0;
			void dfs()
			{
				
			    VetorNos[0].visitado=1;
			    push(0);
			    assert(drawing_points=realloc(drawing_points, (++drawing_point_count)*sizeof(struct point*)));
			    drawing_points[drawing_point_count-1]=points[0];
			    while(!empty())
			    {
			    	int v=NoNaoVisitado(top());
			    	if(v==-1)
			    		pop();
			    	else
			    	{
			    		VetorNos[v].visitado=1;
			    		push(v);
			    		puts("naaa");
			    		assert(drawing_points=realloc(drawing_points, (++drawing_point_count)*sizeof(struct point*)));
			    		drawing_points[drawing_point_count-1]=points[v];
			    	}
			    }
				int i;
			    for (i=0; i<nNOs; i++)
			        VetorNos[i].visitado=0;
			}
			void destruirAresta(int inicio, int fim)
			{
			    MatAdj[inicio][fim]=0;
			    MatAdj[fim][inicio]=0;
			}
			void destruirNo(int n)
			{
			    for (int i=0; i < nNOs; i++)
			    {
			        destruirAresta(n, i);
			        destruirAresta(i, n);
			    }
			    for (int i=n; i < nNOs - 1; i++)
				    VetorNos[i]=VetorNos[i + 1];
			    nNOs--;
			}
			void destruirGrafo()
			{
			    for (int i=0; i < nNOs; i++)
			        free(MatAdj[i]);
			    free(MatAdj);
			    free(VetorNos);
			    nNOs=0;
			}
			typedef struct
			{
			    int items[100];
			    int frente;
			    int tras;
			} Fila;
			void iniciaFila(Fila *f)
			{
			    f->frente=-1;
			    f->tras=-1;
			}
			int filaVazia(Fila *f)
			{
			    return (f->frente == -1);
			}
			void enfileirar(Fila *f, int valor)
			{
			    if(f->frente == -1)
			        f->frente=0;
			    f->items[++f->tras]=valor;
			}
			int desenfileirar(Fila *f)
			{
			    int item;
			    item=f->items[f->frente++];
			    if(f->frente > f->tras)
			        f->frente=f->tras=-1;
			    return item;
			}
			double x, y, z;
		    unsigned int v0, v1;
			double coords[8][3] = {
			    {100.0, 100.0, 100.0},
			    {-100.0, 100.0, 100.0},
			    {100.0, -100.0, 100.0},
			    {-100.0, -100.0, 100.0},
			    {100.0, 100.0, -100.0},
			    {-100.0, 100.0, -100.0},
			    {100.0, -100.0, -100.0},
			    {-100.0, -100.0, -100.0}
			};
			puts("Digite o máximo número de vértices desejado:\n8");
		    Inicia_Grafo(8);
		    puts("Digite três números flutuantes por linha, interrupção com entrada não-numérica:");
		    for (int i = 0; i < 8; i++)
		    {
		    	insereNO(i);
		    	assert(points=realloc(points, (i+1)*sizeof(struct point*)));
			    points[i] = point(coords[i][0], coords[i][1], coords[i][2] + 500.0);
			    printf("Vértice %u criado: (%lf, %lf, %lf)\n", i, coords[i][0], coords[i][1], coords[i][2] + 500.0);
			}
		    puts("Digite dois números positivos inteiros por linha, interrupção com entrada não-numérica:");
			int connections[12][2]={
			    {0, 1}, {1, 3}, {3, 2}, {2, 0},
			    {4, 5}, {5, 7}, {7, 6}, {6, 4},
			    {0, 4}, {1, 5}, {2, 6}, {3, 7}
			};
			for (int i = 0; i < 12; i++)
			{
			    point_connect(points[connections[i][0]], points[connections[i][1]]);
			    point_connect(points[connections[i][1]], points[connections[i][0]]);
			    insereAresta(connections[i][0], connections[i][1]);
			    printf("Aresta criada entre %u e %u.\n", connections[i][0], connections[i][1]);
			}
		    dfs();
		    printf("drawing_point_count:%d\n", drawing_point_count);
		    for(unsigned int point_index=0; point_index<drawing_point_count; point_index++)
		    	point_draw(points[point_index]);
			puts("Entre algum comando");
		}
		else if(!strcmp(string_buffer, "criar"))
		{
			int *itens, topo, tam;
			void Inicia_Pilha(int n)
			{
			    assert(itens =malloc(n*sizeof(int)));
			    tam=n;
			    topo=0;
			}
			void push(int valor)
			{
			    itens [ topo ]=valor;
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
			    MatAdj=malloc(max*sizeof(int *));
			    for(i=0; i<max; i++)
			    	MatAdj[i]=malloc(max*sizeof(int));
			    for (j=0; j<max; j++)
			        for (i=0; i<max; i++)
			            MatAdj[i][j]=0;
			    nNOs=0;
			    Inicia_Pilha(max);
			}
			void insereNO(char v)
			{
			    VetorNos[nNOs].item=v;
			    VetorNos[nNOs].visitado=0;
			    nNOs++;
			}
			void insereAresta(int inicio, int fim)
			{
				if(inicio<0||fim<0||inicio>=nNOs||fim>=nNOs)
					return;
			    MatAdj[inicio][fim]=1;
			    MatAdj[fim][inicio]=1;
			}
			void mostraNo(int v)
			{
			    printf("%c ", VetorNos[v].item);
			}
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
			void dfs()
			{
			     VetorNos[0].visitado=1;
			     push(0);
			     point_draw(points[0]);
			     while(!empty())
			     {
			        int v=NoNaoVisitado(top());
			        if ( v == -1)
			           pop();
			        else
			        {
			           VetorNos[v].visitado=1;
			           push(v);
			           point_draw(points[v]);
			        }
			     }
				 int i;
			     for (i=0; i<nNOs; i++)
			         VetorNos[i].visitado=0;
			}
			void destruirAresta(int inicio, int fim)
			{
			    MatAdj[inicio][fim]=0;
			    MatAdj[fim][inicio]=0;
			}
			void destruirNo(int n)
			{
			    for (int i=0; i < nNOs; i++)
			    {
			        destruirAresta(n, i);
			        destruirAresta(i, n);
			    }
			    for (int i=n; i < nNOs - 1; i++)
				    VetorNos[i]=VetorNos[i + 1];
			    nNOs--;
			}
			void destruirGrafo()
			{
			    for (int i=0; i < nNOs; i++)
			        free(MatAdj[i]);
			    free(MatAdj);
			    free(VetorNos);
			    nNOs=0;
			}
			typedef struct
			{
			    int items[100];
			    int frente;
			    int tras;
			} Fila;
			void iniciaFila(Fila *f)
			{
			    f->frente=-1;
			    f->tras=-1;
			}
			int filaVazia(Fila *f)
			{
			    return (f->frente == -1);
			}
			void enfileirar(Fila *f, int valor)
			{
			    if(f->frente == -1)
			        f->frente=0;
			    f->items[++f->tras]=valor;
			}
			int desenfileirar(Fila *f)
			{
			    int item;
			    item=f->items[f->frente++];
			    if(f->frente > f->tras)
			        f->frente=f->tras=-1;
			    return item;
			}

			double x, y, z;
		    unsigned int v0, v1;
		    int maximum_vertice_count;

			puts("Digite o máximo número de vértices desejado:");
		    while(1)
		    {
		    	int c;
   				while((c = getchar()) != '\n' && c != EOF){}
		        if (!fgets(string_buffer, sizeof(string_buffer), stdin))
		        {
		            puts("Failed to read input.");
		            continue;
		        }
		        if (sscanf(string_buffer, "%d", &maximum_vertice_count) == 1)
		            break;
		        puts("Entrada inválida. Tente novamente:");
		    }
		    Inicia_Grafo(maximum_vertice_count);
		    puts("Digite três números flutuantes por linha, interrupção com entrada não-numérica:");
		    while (fgets(string_buffer, sizeof(string_buffer), stdin)) {
		        if (sscanf(string_buffer, "%lf %lf %lf", &x, &y, &z) != 3) break;
		        printf("Vértice %u criado: (%lf, %lf, %lf)\n", point_count, x, y, z);
		        assert(points=realloc(points, (++point_count)*sizeof(struct point*)));
		        points[point_count-1]=point(x, y, z);
		        insereNO(point_count);
		    }
		    puts("Digite dois números positivos inteiros por linha, interrupção com entrada não-numérica:");
		    while (fgets(string_buffer, sizeof(string_buffer), stdin)) {
		        if (sscanf(string_buffer, "%u %u", &v0, &v1) != 2) break;
		        if (v0 < point_count && v1 < point_count) {
		            point_connect(points[v0], points[v1]);
		            point_connect(points[v1], points[v0]);
		            insereAresta(v0, v1);
		            printf("Aresta criada entre %u e %u.\n", v0, v1);

		            dfs();
		        } else
		            puts("Número de vértice inválido.");
		    }

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
				case WM_TIMER:
					switch(wide_parameter)
					{
					case 0:
						InvalidateRect(window_handle, NULL, TRUE);
					}
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
    pthread_t cli_thread;
	assert(!pthread_create(&cli_thread, NULL, cli_thread_function, &window_handle));
    SetTimer(window_handle, 0, 1000, NULL);
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