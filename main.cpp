// Avantime BitmapLayerHandler 1.2 Test
// Johan Astborg, 2003

#include <windows.h>
#include "bmplayerhandler.h"

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

BmpLayerHandler *bmpLayerHandler;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = "HelloWin";
	MSG			msg ;
	WNDCLASS	wndclass;
	HWND		hwnd;
	
	wndclass.style			= CS_DBLCLKS;
	wndclass.lpfnWndProc	= WndProc ;
	wndclass.cbClsExtra		= 0 ;
	wndclass.cbWndExtra		= 0 ;
	wndclass.hInstance		= hInstance ;
	wndclass.hIcon			= LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor		= LoadCursor (NULL, MAKEINTRESOURCE(IDC_ARROW)) ;
	wndclass.hbrBackground	= (HBRUSH) GetStockObject (BLACK_BRUSH) ;
	wndclass.lpszMenuName	= NULL ;
	wndclass.lpszClassName	= szAppName ;
	 
	if (!RegisterClass (&wndclass))
	{
		MessageBox (NULL, TEXT ("This program requires Windows NT!"), 
				  szAppName, MB_ICONERROR);
		return 0 ;
	}
	
	hwnd = CreateWindow (szAppName, "Avantime BmpLayer 1.0",WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
					 CW_USEDEFAULT, 300, 240, NULL, NULL, hInstance, NULL);	  
 
	bmpLayerHandler = new BmpLayerHandler(280,230);
	bmpLayerHandler->AssignRendererToWnd(hwnd);
	bmpLayerHandler->NewLayer(RGB(255,255,255));
	bmpLayerHandler->NewLayer(RGB(255,255,255));
	bmpLayerHandler->NewLayer(RGB(255,0,0));

	//bmpLayerHandler->ReadBitmapAndRender("C:\\test.bmp");
	//bmpLayerHandler
	bmpLayerHandler->Test(0);
	bmpLayerHandler->BlurLayer(0);
	//bmpLayerHandler->BlurLayer(0);
	bmpLayerHandler->Test2(1);	
	//bmpLayerHandler->Test(2);

	bmpLayerHandler->ContractLayers();



	ShowWindow (hwnd, iCmdShow) ;
	UpdateWindow (hwnd) ;
    	 
	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}

	
	return msg.wParam ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	HBITMAP hBitmap;
    PAINTSTRUCT ps;

	switch (message)
	{

		case WM_CREATE:			
			
			
			//bmpLayerHandler->NewLayer(RGB(255,0,0));
			return 0;

		case WM_KEYDOWN:

			//bmpLayerHandler->ContractLayers();
			//bmpLayerHandler->Render();
			//bmpLayerHandler->InvertLayer(1);
				
			//bmpLayerHandler->RenderLayerToBmp(1);
			
			//bmpLayerHandler->BlurLayer(0);
			//bmpLayerHandler->SaturationLayer(1,0.4);
			//bmpLayerHandler->LightnessLayer(1,0.01);			
			//bmpLayerHandler->BlurLayer(1);

			return 0;

		case WM_MOUSEMOVE:

			bmpLayerHandler->temp_left = LOWORD(lParam);
			bmpLayerHandler->temp_right = HIWORD(lParam);
			bmpLayerHandler->ClearLayer(1,RGB(255,255,255));
			bmpLayerHandler->Test2(1);
			bmpLayerHandler->ContractLayers();
			bmpLayerHandler->Render();

			return 0;

		case WM_TIMER:


			return 0;

		case WM_SIZE:

		
			return 0;

		case WM_EXITSIZEMOVE:

		
			return 0;

		case WM_PAINT:
		
			hdc = BeginPaint(hwnd,&ps);
			
			
			//bmpLayerHandler->RenderLayerToBmp(1);
			//bmpLayerHandler->Render();

			EndPaint(hwnd,&ps);
	
			return 0;

		case WM_DESTROY:

			delete bmpLayerHandler;
			PostQuitMessage (0) ;
			return 0 ;		
	
	}

	return DefWindowProc (hwnd, message, wParam, lParam) ;
}
