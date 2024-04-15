#define _GNU_SOURCE

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<assert.h>

#include<windows.h>


int main(int argc, char **argv)
{
	assert(argc==1);

	assert(AllocConsole());
	assert(freopen("CONOUT$", "wb", stdout));
	assert(freopen("CONIN$", "rb", stdin));

	int point0_x, point0_y, point1_x=0, point1_y=0;
	unsigned char state=0;

	RECT client_rectangle;
    void *pixel_data=NULL;
    HDC hdcMem;
	HBITMAP hBitmap;
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
						memset(pixel_data, 0, client_rectangle.right*client_rectangle.bottom*sizeof(COLORREF));
						point0_x=client_rectangle.right/2;
						point0_y=client_rectangle.bottom/2;
						switch(state)
						{
						case 0:
							if(++point1_x>=client_rectangle.right)
							{
								point1_x--;
								state=1;
							}
							break;
						case 1:
							if(++point1_y>=client_rectangle.bottom)
							{
								point1_y--;
								state=2;
							}
							break;
						case 2:
							if(--point1_x)
								state=3;
							break;
						case 3:
							if(--point1_y)
								state=0;
						}
						pixel_data_bresenham_line(point0_y, point0_x, point1_y, point1_x, RGB(255, 255, 255));
						InvalidateRect(window_handle, NULL, FALSE); 
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

    SetTimer(window_handle, 0, 5, NULL);

   	MSG message;
   	BOOL GetMessage_return;
	while(GetMessage_return=GetMessage(&message, NULL, 0, 0))
	{
		assert(GetMessage_return!=-1);
	    TranslateMessage(&message);
	    DispatchMessage(&message);
	}

	assert(FreeConsole());
	return 0;
}