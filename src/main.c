#define _GNU_SOURCE

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#include<windows.h>
LRESULT CALLBACK window_procedure(HWND window_handle, UINT message, WPARAM wide_parameter, LPARAM long_parameter)
{
	switch(message)
	{
		case WM_SIZE:
			printf("width:%u height%u\n", LOWORD(long_parameter), HIWORD(long_parameter));
		default:
			return DefWindowProc(window_handle, message, wide_parameter, long_parameter);
	}
	return 0;
};

int main(int argc, char **argv)
{
	assert(argc==1);

	assert(AllocConsole());
	assert(freopen("CONOUT$", "wb", stdout));
	assert(freopen("CONIN$", "rb", stdin));

	WNDCLASS window_class={};
	window_class.lpfnWndProc=window_procedure;
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