#define _GNU_SOURCE

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<assert.h>

#include<windows.h>

#include<pthread.h>

struct point_3d
{
	double x, y, z;
	struct point_3d **connection;
	unsigned char connection_count;
};
struct point_3d *point(double x, double y, double z)
{
	struct point_3d *self;
	assert(self=calloc(1, sizeof(struct point_3d)));
	self->x=x;
	self->y=y;
	self->z=z;
	return self;
}
void point_connect(struct point_3d *self, struct point_3d *connection_point)
{
	assert(self);
	assert(connection_point);
	assert(++self->connection_count);
	assert(self->connection=realloc(self->connection, self->connection_count*sizeof(struct point_3d*)));
	self->connection[self->connection_count-1]=connection_point;
}

void *pixel_data=NULL;
RECT client_rectangle;
void pixel_data_set(int row, int col, COLORREF rgb)
{
    assert(pixel_data);
	COLORREF *pixels=(COLORREF*)pixel_data;
	unsigned int index=(row*client_rectangle.right)+col;
	if(col>=0&&col<client_rectangle.right&&row>=0&&row<client_rectangle.bottom)
   		pixels[index]=rgb;
}
void pixel_data_bresenham_line(int x0, int y0, int x1, int y1, COLORREF color)
{
    int dx=abs(x1-x0), sx=x0<x1?1:-1;
    int dy=-abs(y1-y0), sy=y0<y1?1:-1; 
    int err=dx+dy, e2;
    for(;;)
    {
        pixel_data_set(y0, x0, color);
        if(x0==x1&&y0==y1)break;
        e2=2*err;
        if(e2>=dy)
        {
        	err+=dy;
        	x0+=sx;
        }
        if(e2<=dx)
        {
        	err+=dx;
        	y0+=sy;
        }
    }
}
void point_draw(struct point_3d *self)
{
	assert(self);
	struct point_3d_2d
	{
		double x, y;
	}
	projection_3d(double x, double y, double z)
	{
	    const double z_near=0.1;
	    const double fov=M_PI/4;
	    const double aspect_ratio=(double)client_rectangle.right/client_rectangle.bottom;
	    if(z<z_near)
	        z=z_near;
	    double f=1.0/tan(fov/2.0);
	    double x_proj=(f/aspect_ratio)*x/z;
	    double y_proj=f*y/z;
	    struct point_3d_2d result;
	    result.x=(x_proj+1)*client_rectangle.right / 2;
	    result.y=(1-y_proj)*client_rectangle.bottom / 2;
	    result.x=max(0, min(result.x, client_rectangle.right-1));
	    result.y=max(0, min(result.y, client_rectangle.bottom-1));
	    return result;
	}
	struct point_3d_2d self_projected=projection_3d(self->x, self->y, self->z);
	for(unsigned char connection_index=0; connection_index<self->connection_count; connection_index++)
	{
		struct point_3d *connection_point=self->connection[connection_index];
		struct point_3d_2d connection_projected=projection_3d(connection_point->x, connection_point->y, connection_point->z);
		pixel_data_bresenham_line(self_projected.x, self_projected.y, connection_projected.x, connection_projected.y, RGB(255, 255, 255));
	}
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




struct stack
{
	int *items, top, size;
};
struct stack *stack(int n)
{
	struct stack *self;
	assert(self=calloc(1, sizeof(struct stack)));
    assert(self->items=malloc((self->size=n)*sizeof(int)));
    self->top=0;
    return self;
}
void stack_push(struct stack *self, int valor)
{
	assert(self);
    self->items[self->top++]=valor;
}
void stack_pop(struct stack *self)
{
	assert(self);
	self->top--;
}
int stack_top(struct stack *self)
{
	assert(self);
	return self->items[self->top-1];
}
int stack_empty(struct stack *self)
{
	assert(self);
	return (self->top==0);
}

struct graph_node
{
	char item;
	int visited;
};
struct graph
{
	struct stack *visiting_stack;
	struct graph_node *nodes;
	int **adjacy_matrix;
	int node_count;
};
struct graph *graph(int max)
{
	struct graph *self;
	assert(self=calloc(1, sizeof(struct graph)));
    int i, j;
    assert(self->nodes=malloc(max*sizeof(struct graph_node)));
    assert(self->adjacy_matrix=malloc(max*sizeof(int *)));
    for(i=0; i<max; i++)
    	assert(self->adjacy_matrix[i]=malloc(max*sizeof(int)));
    for(j=0; j<max; j++)
        for(i=0; i<max; i++)
            self->adjacy_matrix[i][j]=0;
    self->node_count=0;
    self->visiting_stack=stack(max);
    return self;
}
void graph_vertex_insert(struct graph *self, char v)
{
	assert(self);
    self->nodes[self->node_count].item=v;
    self->nodes[self->node_count].visited=0;
    self->node_count++;
}
void graph_edge_insert(struct graph *self, int inicio, int fim)
{
	assert(self);
	if(inicio<0||fim<0||inicio>=self->node_count||fim>=self->node_count)
		return;
    self->adjacy_matrix[inicio][fim]=1;
    self->adjacy_matrix[fim][inicio]=1;
}
int graph_if_node_not_visited(struct graph *self, int i)
{
	assert(self);
	int j;
    for(j=0; j<self->node_count; j++)
        if(self->adjacy_matrix[i][j]==1&&self->nodes[j].visited==0)
            return j;
    return -1;
}
void graph_point_3d_depth_first_search(struct graph *self, struct point_3d **points)
{
	assert(self);
	assert(points);
	struct point_3d **drawing_points=NULL;
    unsigned int drawing_point_count=0;
    self->nodes[0].visited=1;
    stack_push(self->visiting_stack, 0);
    assert(drawing_points=realloc(drawing_points, (++drawing_point_count)*sizeof(struct point_3d*)));
    drawing_points[drawing_point_count-1]=points[0];
    while(self->visiting_stack->top)
    {
    	int v=graph_if_node_not_visited(self, stack_top(self->visiting_stack));
    	if(v==-1)
    		stack_pop(self->visiting_stack);
    	else
    	{
    		self->nodes[v].visited=1;
    		stack_push(self->visiting_stack, v);
    		assert(drawing_points=realloc(drawing_points, (++drawing_point_count)*sizeof(struct point_3d*)));
    		drawing_points[drawing_point_count-1]=points[v];
    	}
    }
	int i;
    for(i=0; i<self->node_count; i++)
        self->nodes[i].visited=0;
}
void graph_edge_destroy(struct graph *self, int inicio, int fim)
{
	assert(self);
    self->adjacy_matrix[inicio][fim]=0;
    self->adjacy_matrix[fim][inicio]=0;
}
void graph_node_detroy(struct graph *self, int n)
{
	assert(self);
    for(int i=0; i<self->node_count; i++)
    {
        graph_edge_destroy(self, n, i);
        graph_edge_destroy(self, i, n);
    }
    for(int i=n; i<self->node_count-1; i++)
	    self->nodes[i]=self->nodes[i+1];
    self->node_count--;
}
void graph_destroy(struct graph *self)
{
	assert(self);
    for(int i=0; i<self->node_count; i++)
        free(self->adjacy_matrix[i]);
    free(self->adjacy_matrix);
    free(self->nodes);
    free(self);
}
struct heap
{
    int items[100];
    int head;
    int tail;
};
struct heap *heap()
{
	struct heap *self;
	assert(self=calloc(1, sizeof(struct heap)));
    self->head=-1;
    self->tail=-1;
    return self;
}
int heap_if_empty(struct heap *self)
{
	assert(self);
    return (self->head==-1);
}
void heapify(struct heap *self, int value)
{
	assert(self);
    if(self->head==-1)
        self->head=0;
    self->items[++self->tail]=value;
}
int unheapify(struct heap *self)
{
	assert(self);
    int item;
    item=self->items[self->head++];
    if(self->head>self->tail)
        self->head=self->tail=-1;
    return item;
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void *cli_thread_function(void *arg)
{
	void ajuda(void)
	{
		puts("\'sair\' - terminar o programa, pressione Ctrl-C para desbloquear o scnaf do terminal");
		puts("\'ajuda\' - printa esta mesma lista de comandos");
		puts("\'cubo\' - exemplo de um grafo em formato de cubo");
		puts("\'tetrahedro\' - exemplo de um grafo em formato de tetrahedro");
		puts("\'aleatorio\' - criar um grafo aleatorio de n vertices");
		puts("\'criar\' - criar um grafo");
		puts("\'limpar\' - limpar a tela");
	}
	assert(arg);
	puts("   _____                 _      __      ___                 _ _              ");
	puts("  / ____|               | |     \\ \\    / (_)               | (_)             ");
	puts(" | |  __ _ __ __ _ _ __ | |__    \\ \\  / / _ _____   _  __ _| |_ _______ _ __ ");
	puts(" | | |_ | '__/ _` | '_ \\| '_ \\    \\ \\/ / | |_  / | | |/ _` | | |_  / _ \\ '__|");
	puts(" | |__| | | | (_| | |_) | | | |    \\  /  | |/ /| |_| | (_| | | |/ /  __/ |   ");
	puts("  \\_____|_|  \\__,_| .__/|_| |_|     \\/   |_/___|\\__,_|\\__,_|_|_/___\\___|_|   ");
	puts("                  | |                                         		    1.0");
	puts("                  |_|                                         Luciano Machado");
	puts("                                                              Vinicius Vieira");
	puts("Seja bem vindo ao vizualizador de grafos!");
	puts("Ele utiliza o algoritmo DFS e muito mais!");
	puts("Comandos:");
	ajuda();
	char string_buffer[256];
	while(scanf("%s", &string_buffer))
		if(!strcmp(string_buffer, "sair"))
		{
			CloseWindow(*(HWND*)arg);
			return NULL;
		}
		else if(!strcmp(string_buffer, "limpar"))
		{
			memset(pixel_data, 0, client_rectangle.right*client_rectangle.bottom*sizeof(COLORREF));
		}
		else if(!strcmp(string_buffer, "ajuda"))
		{
			ajuda();
		}
		else if(!strcmp(string_buffer, "aleatorio"))
		{
			double randfrom(double min, double max) 
			{
			    double range=max-min; 
			    double div=RAND_MAX/range;
			    return min+(rand()/div);
			}

			double x, y, z;
		    unsigned int v0, v1;
		    int maximum_vertice_count;
		    struct point_3d **points=NULL;
		    unsigned int point_count=0;
			puts("Digite o maximo numero de vertices desejado:");
		    while(1)
		    {
		    	int c;
   				while((c=getchar())!='\n'&&c!=EOF){}
		        if(!fgets(string_buffer, sizeof(string_buffer), stdin))
			        continue;
		        if(sscanf(string_buffer, "%d", &maximum_vertice_count)==1)
		            break;
		        puts("Entrada invalida. Tente novamente:");
		    }
		    struct graph *graph_buffer=graph(maximum_vertice_count);
		    puts("Digite tres numeros flutuantes por linha, interrupcao com entrada nao-numerica:");
		    double coords[maximum_vertice_count][3];
		    for(unsigned int coord_index=0; coord_index<maximum_vertice_count; coord_index++)
		    {
			    coords[coord_index][0]=randfrom(-500.0, 500.0);
			    coords[coord_index][1]=randfrom(-500.0, 500.0);
			    coords[coord_index][2]=randfrom(10.0, 500.0);
			};
		    for(int i=0; i<maximum_vertice_count; i++)
		    {
		    	graph_vertex_insert(graph_buffer, i);
		    	assert(points=realloc(points, (i+1)*sizeof(struct point_3d*)));
			    points[i]=point(coords[i][0], coords[i][1], coords[i][2]+500.0);
			    printf("Vertice %u criado: (%lf, %lf, %lf)\n", i, coords[i][0], coords[i][1], coords[i][2]+500.0);
			}
		    puts("Digite dois numeros positivos inteiros por linha, interrupcao com entrada nao-numerica:");
			unsigned long long int sum=0;
			for(unsigned int index=0; index<maximum_vertice_count; index++)
				sum+=index;
			unsigned int connection_count;
			int connections[connection_count=(unsigned int)((sum*3)/20)][2];
			for(unsigned int connections_index=0; connections_index<connection_count; connections_index++)
			{
				connections[connections_index][0]=rand()%maximum_vertice_count;
				connections[connections_index][1]=rand()%maximum_vertice_count;
			}
			for(int i=0; i<connection_count; i++)
			{
			    point_connect(points[connections[i][0]], points[connections[i][1]]);
			    point_connect(points[connections[i][1]], points[connections[i][0]]);
			    graph_edge_insert(graph_buffer, connections[i][0], connections[i][1]);
			    printf("Aresta criada entre %u e %u.\n", connections[i][0], connections[i][1]);
			}
		    graph_point_3d_depth_first_search(graph_buffer, points);
		    for(unsigned int point_index=0; point_index<maximum_vertice_count; point_index++)
		    	point_draw(points[point_index]);
			puts("\nEntre com algum comando novamente");
		}
		else if(!strcmp(string_buffer, "tetrahedro"))
		{
			struct point_3d **points=NULL;
			puts("Digite o maximo numero de vertices desejado:\n8");
		    struct graph *cube_graph=graph(4);
		    puts("Digite tres numeros flutuantes por linha, interrupcao com entrada nao-numerica:");
		    double coords[4][3]={
			    {100.0, 100.0, 100.0},
			    {100.0, -100.0, -100.0},
			    {-100.0, 100.0, 100.0},
			    {-100.0, -100.0, 100.0}
			};
		    for(int i=0; i<4; i++)
		    {
		    	graph_vertex_insert(cube_graph, i);
		    	assert(points=realloc(points, (i+1)*sizeof(struct point_3d*)));
			    points[i]=point(coords[i][0], coords[i][1], coords[i][2]+500.0);
			    printf("Vertice %u criado: (%lf, %lf, %lf)\n", i, coords[i][0], coords[i][1], coords[i][2]+500.0);
			}
		    puts("Digite dois numeros positivos inteiros por linha, interrupcao com entrada nao-numerica:");
			int connections[6][2]={
			    {0, 1}, {0, 2},
			    {0, 3}, {1, 2},
			    {1, 3}, {2, 3}
			};
			for(int i=0; i<6; i++)
			{
			    point_connect(points[connections[i][0]], points[connections[i][1]]);
			    point_connect(points[connections[i][1]], points[connections[i][0]]);
			    graph_edge_insert(cube_graph, connections[i][0], connections[i][1]);
			    printf("Aresta criada entre %u e %u.\n", connections[i][0], connections[i][1]);
			}
		    graph_point_3d_depth_first_search(cube_graph, points);
		    for(unsigned int point_index=0; point_index<4; point_index++)
		    	point_draw(points[point_index]);
			puts("\nEntre com algum comando novamente");
		}
		else if(!strcmp(string_buffer, "cubo"))
		{
			struct point_3d **points=NULL;
			puts("Digite o maximo numero de vertices desejado:\n8");
		    struct graph *cube_graph=graph(8);
		    puts("Digite tres numeros flutuantes por linha, interrupcao com entrada nao-numerica:");
		    double coords[8][3]={
			    {100.0, 100.0, 100.0},
			    {-100.0, 100.0, 100.0},
			    {100.0, -100.0, 100.0},
			    {-100.0, -100.0, 100.0},
			    {100.0, 100.0, -100.0},
			    {-100.0, 100.0, -100.0},
			    {100.0, -100.0, -100.0},
			    {-100.0, -100.0, -100.0}
			};
		    for(int i=0; i<8; i++)
		    {
		    	graph_vertex_insert(cube_graph, i);
		    	assert(points=realloc(points, (i+1)*sizeof(struct point_3d*)));
			    points[i]=point(coords[i][0], coords[i][1], coords[i][2]+500.0);
			    printf("Vertice %u criado: (%lf, %lf, %lf)\n", i, coords[i][0], coords[i][1], coords[i][2]+500.0);
			}
		    puts("Digite dois numeros positivos inteiros por linha, interrupcao com entrada nao-numerica:");
			int connections[12][2]={
			    {0, 1}, {1, 3}, {3, 2}, {2, 0},
			    {4, 5}, {5, 7}, {7, 6}, {6, 4},
			    {0, 4}, {1, 5}, {2, 6}, {3, 7}
			};
			for(int i=0; i<12; i++)
			{
			    point_connect(points[connections[i][0]], points[connections[i][1]]);
			    point_connect(points[connections[i][1]], points[connections[i][0]]);
			    graph_edge_insert(cube_graph, connections[i][0], connections[i][1]);
			    printf("Aresta criada entre %u e %u.\n", connections[i][0], connections[i][1]);
			}
		    graph_point_3d_depth_first_search(cube_graph, points);
		    for(unsigned int point_index=0; point_index<8; point_index++)
		    	point_draw(points[point_index]);
			puts("\nEntre com algum comando novamente");
		}
		else if(!strcmp(string_buffer, "criar"))
		{
			double x, y, z;
		    unsigned int v0, v1;
		    int maximum_vertice_count;
		    struct point_3d **points=NULL;
		    unsigned int point_count=0;
			puts("Digite o maximo numero de vertices desejado:");
		    while(1)
		    {
		    	int c;
   				while((c=getchar())!='\n'&&c!=EOF){}
		        if(!fgets(string_buffer, sizeof(string_buffer), stdin))
			        continue;
		        if(sscanf(string_buffer, "%d", &maximum_vertice_count)==1)
		            break;
		        puts("Entrada invalida. Tente novamente:");
		    }
		    struct graph *graph_buffer=graph(maximum_vertice_count);
		    puts("Digite tres numeros flutuantes por linha, interrupcao com entrada nao-numerica:");
		    while(fgets(string_buffer, sizeof(string_buffer), stdin))
		    {
		        if(sscanf(string_buffer, "%lf %lf %lf", &x, &y, &z)!=3)
		        	break;
		        printf("Vertice %u criado: (%lf, %lf, %lf)\n", point_count, x, y, z);
		        assert(points=realloc(points, (++point_count)*sizeof(struct point_3d*)));
		        points[point_count-1]=point(x, y, z);
		        graph_vertex_insert(graph_buffer, point_count-1);
		    }
		    puts("Digite dois numeros positivos inteiros por linha, interrupcao com entrada nao-numerica:");
		    while(fgets(string_buffer, sizeof(string_buffer), stdin))
		    {
		        if(sscanf(string_buffer, "%u %u", &v0, &v1)!=2)
		        	break;
		        if(v0<point_count&&v1<point_count)
		        {
		            point_connect(points[v0], points[v1]);
		            point_connect(points[v1], points[v0]);
		            graph_edge_insert(graph_buffer, v0, v1);
		            printf("Aresta criada entre %u e %u.\n", v0, v1);

		            graph_point_3d_depth_first_search(graph_buffer, points);
		        }
		        else
		            puts("Numero de vertice invalido.");
		    }
			puts("\nEntre com algum comando novamente");
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
				              &(BITMAPINFO)
				              {.bmiHeader={
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
    				pixel_data=realloc(pixel_data, client_rectangle.right*client_rectangle.bottom*sizeof(COLORREF));
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
	window_class.lpszClassName="graph_vizualizer";
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