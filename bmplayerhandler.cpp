#include "bmplayerhandler.h"

BmpLayerHandler::BmpLayerHandler(int width, int height) {

	layerCount=0;
	bmpMain = new DWORD[width*height];
	iwidth = width;
	iheight = height;	

	temp_left=0;
}

void BmpLayerHandler::AssignRendererToWnd(HWND hwnd) {

	hwndOwner = hwnd;
}

void BmpLayerHandler::ContractLayers() {

	DWORD *tempBits = new DWORD[iwidth*iheight];
	
	int ra,ga,ba;
	int rb,gb,bb;

	COLORREF aclr, bclr;

	DWORD *layerOne = bmpLayers[0]->GetLayerBits();
	DWORD *layerTwo = bmpLayers[1]->GetLayerBits();

	// just for testing
	for(int x=0;x<iwidth*iheight;x++) {	

		aclr = layerOne[x];
		bclr = layerTwo[x];

		rb = GetRValue(bclr); 
		gb = GetGValue(bclr);
		bb = GetBValue(bclr);

		if(bclr != dRGB(RGB(0,0,255))) {

			ra = GetRValue(aclr);
			ga = GetGValue(aclr);
			ba = GetBValue(aclr);	

			bmpMain[x] = ((int) (((ra << 7 ) + rb *(256-128)) >> 8)) | ((int) (((ga << 7 ) + gb * (256-128)) >> 8) << 8) | (int) (((ba << 7)+ bb *(256-128)) >> 8) << 16;	
		}
		else {		
				bmpMain[x] = ((int)255) | (((int) 255) << 8) | ((int)(255) << 16);	
		}	

		//bmpMain[x] = ((int) ((ra+rb)/2)) | ((int) ((ga+gb)/2) << 8) | (int) ((ba+bb)/2) << 16;		
	}

	delete [] tempBits;
}

void BmpLayerHandler::NewLayer(COLORREF base_color) {

	DWORD dColor = dRGB(base_color);

	bmpLayers[layerCount] = new BmpLayer(iwidth,iheight, dColor);
	layerCount++;
}

void BmpLayerHandler::NewLayer(int width, int height, COLORREF base_color) {
	
	DWORD dColor = dRGB(base_color);

	bmpLayers[layerCount] = new BmpLayer(width,height, dColor);
	layerCount++;
}

COLORREF BmpLayerHandler::DwordToColorRef(DWORD data) {

	return RGB(GetRValue(data),GetGValue(data),GetBValue(data));
}

void BmpLayerHandler::Render() {
/*
	HDC dc = GetDC(hwndOwner);
	HDC mem = CreateCompatibleDC(dc);
	HBITMAP bmp = CreateCompatibleBitmap(dc,iwidth,iheight);

	SelectObject(mem,bmp);

	for(int x=0;x<iwidth;x++) {

		for(int y=0;y<iheight;y++) {

			SetPixel(mem,x,y,bmpMain[x*y]);
		}
	}

	BitBlt(dc,0,0,iwidth,iheight,mem,0,0,SRCCOPY);

	DeleteDC(mem);
	DeleteDC(dc);
	DeleteObject(bmp);*/

	HDC dc = GetDC(hwndOwner);
	HDC mem = CreateCompatibleDC(dc);		

	DWORD *bmpArray = bmpMain;
	DWORD *pBackBits;	

	HBITMAP bmp_dib;

	BITMAPINFO bmi;
	ZeroMemory(&bmi.bmiHeader, sizeof(BITMAPINFOHEADER));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = iwidth;
	bmi.bmiHeader.biHeight = iheight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	
	bmp_dib = CreateDIBSection(dc,  &bmi, DIB_RGB_COLORS,(void**)&pBackBits, 0,0);
	if (!bmp_dib)
		MessageBox(hwndOwner,"Dib Error","Error",0);

	SelectObject(mem,bmp_dib);		


	for(int i=0;i<iwidth*iheight;i++) {			

		pBackBits[i] = bmpArray[i];
	}

	SelectObject(mem,bmp_dib);		
	BitBlt(dc, 0,0, iwidth, iheight, mem, 0, 0, SRCCOPY);

	DeleteDC(mem);
	DeleteDC(dc);
	DeleteObject(bmp_dib);
}

void BmpLayerHandler::RenderLayerToBmp(int index) {

	if(index < layerCount) {
		
		HDC dc = GetDC(hwndOwner);
		HDC mem = CreateCompatibleDC(dc);		
		bmp_rnd = CreateCompatibleBitmap(dc,bmpLayers[index]->GetWidth(),bmpLayers[index]->GetHeight());

		SelectObject(mem,bmp_rnd);		

		int current_y=0;
		int current_x=0;	

		// Need optimization...
		/*
		for(int i=0;i<=bmpLayers[index]->GetLayerBitCount();i++) {
				
				if(current_x == bmpLayers[index]->GetWidth()) {

					current_x = 0;
					current_y++;
				}
				else {
					
					current_x++;
				}

				SetPixel(mem,current_x,bmpLayers[index]->GetHeight()-current_y,bmpLayers[index]->GetLayerBit(i));
		}*/	

		DWORD *bmpArray = bmpLayers[index]->GetLayerBits();
		DWORD *pBackBits;	

		HBITMAP bmp_dib;

		BITMAPINFO bmi;
		ZeroMemory(&bmi.bmiHeader, sizeof(BITMAPINFOHEADER));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = bmpLayers[index]->GetWidth();
		bmi.bmiHeader.biHeight = bmpLayers[index]->GetHeight();
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		
		bmp_dib = CreateDIBSection(dc,  &bmi, DIB_RGB_COLORS,(void**)&pBackBits, 0,0);
		if (!bmp_dib)
			MessageBox(hwndOwner,"Dib Error","Error",0);

		SelectObject(mem,bmp_dib);		
	
	
		for(int i=0;i<bmpLayers[index]->GetLayerBitCount();i++) {

			if(current_x == bmpLayers[index]->GetWidth()) {

				current_x = 0;
				current_y++;
			}
			else {
			
				current_x++;
			}

			pBackBits[i] = bmpArray[i];
		}

		SelectObject(mem,bmp_dib);		
		BitBlt(dc, 0,0, bmpLayers[index]->GetWidth(), bmpLayers[index]->GetHeight(), mem, 0, 0, SRCCOPY);

		DeleteDC(mem);
		DeleteDC(dc);
		DeleteObject(bmp_dib);
		
		//DeleteObject(bmp);
	
	}
}

void BmpLayerHandler::FastRenderCurrent(int index) {
/*
	if(index < layerCount) {

		HDC dc = GetDC(hwndOwner);
		HDC mem = CreateCompatibleDC(dc);
		SelectObject(mem,bmp_rnd);		

		BitBlt(dc,0,0,bmpLayers[index]->GetWidth(),bmpLayers[index]->GetHeight(),mem,0,0,SRCCOPY);

		DeleteDC(dc);
		DeleteDC(mem);
	}*/
}


void BmpLayerHandler::InvertBitmapArray(DWORD *bmpArray, int width, int height) {

	DWORD *tempArray = new DWORD[width*height];

	int size=width*height;
	DWORD temp=0;
	char r,g,b;

	for(int i=0;i<size;i++) {

		r = GetRValue(bmpArray[i]);
		g = GetGValue(bmpArray[i]);
		b = GetBValue(bmpArray[i]);

		temp = RGB(255-r,255-g,255-b);
		tempArray[i] = temp;
	}

	for(int a=0;a<size;a++) {

		bmpArray[a] = tempArray[a];
	}

	delete [] tempArray;
}

void BmpLayerHandler::InvertLayer(int index) {

	if(index < layerCount) {
    
		InvertBitmapArray(bmpLayers[index]->GetLayerBits(),bmpLayers[index]->GetWidth(),bmpLayers[index]->GetHeight());	
		RenderLayerToBmp(index);
		FastRenderCurrent(index);
	}
}

void BmpLayerHandler::BlurLayer(int index) {

	if(index < layerCount) {

		BlurBitmapArray(bmpLayers[index]);
		RenderLayerToBmp(index);
		FastRenderCurrent(index);
	}
}

void BmpLayerHandler::BlurBitmapArray(BmpLayer *layer) {

	DWORD *tempArray = new DWORD[layer->GetLayerBitCount()];
	DWORD *bmpArray = layer->GetLayerBits();

	int xpos[9] = {-1,0,1,-1,0,1,-1,0,1};
	int ypos[9] = {-1,-1,-1,0,0,0,1,1,1};

	unsigned char r[10], g[10], b[10];
	unsigned char ar=0,ag=0,ab=0;	
	int current_y=0;
	int current_x=0;	

	for(int y=0;y<layer->GetLayerBitCount();y++)
	{
		if(current_x == layer->GetWidth()-1) {

			current_x = 0;
			current_y++;
		}
		else {
			
			current_x++;
		}

		if((current_x >= 2 && current_x <= layer->GetWidth() - 2) && (current_y > 2 && current_y < layer->GetHeight() - 2)) {
		
			for(int a=1;a<10;a++) {
				
				r[a] = GetRValue(bmpArray[(layer->GetHeight()-(current_y+ypos[a-1]))*layer->GetWidth()+(current_x+xpos[a-1])]);
				g[a] = GetGValue(bmpArray[(layer->GetHeight()-(current_y+ypos[a-1]))*layer->GetWidth()+(current_x+xpos[a-1])]);
				b[a] = GetBValue(bmpArray[(layer->GetHeight()-(current_y+ypos[a-1]))*layer->GetWidth()+(current_x+xpos[a-1])]);
			}
			
			ar = (r[1] + r[2] + r[3] + r[4] + r[5] + r[6] + r[7] + r[8] + r[9])/9;
			ag = (g[1] + g[2] + g[3] + g[4] + g[5] + g[6] + g[7] + g[8] + g[9])/9;
			ab = (b[1] + b[2] + b[3] + b[4] + b[5] + b[6] + b[7] + b[8] + b[9])/9;	

			tempArray[(layer->GetHeight()-(current_y))*layer->GetWidth()+(current_x)] = RGB(ar,ag,ab);
	
		}
		else {
			
			if(current_y == 0 && current_x > 0) {

				for(int a=4;a<10;a++) {

					r[a] = GetRValue(bmpArray[(layer->GetHeight()-(current_y+ypos[a-1]))*layer->GetWidth()+(current_x+xpos[a-1])]);
					g[a] = GetGValue(bmpArray[(layer->GetHeight()-(current_y+ypos[a-1]))*layer->GetWidth()+(current_x+xpos[a-1])]);
					b[a] = GetBValue(bmpArray[(layer->GetHeight()-(current_y+ypos[a-1]))*layer->GetWidth()+(current_x+xpos[a-1])]);
				}
			
				ar = (r[4] + r[5] + r[6] + r[7] + r[8] + r[9])/9;
				ag = (g[4] + g[5] + g[6] + g[7] + g[8] + g[9])/9;
				ab = (b[4] + b[5] + b[6] + b[7] + b[8] + b[9])/9;
			}
			else {

				ar = 0;
				ag = 0;
				ab = 0;
			}

			tempArray[y] = RGB(ar,ag,ab);
		}		
		
	}
	
	for(int a=0;a<layer->GetWidth()*layer->GetHeight();a++) {

		bmpArray[a] = tempArray[a];
	}

	delete [] tempArray;
}

void BmpLayerHandler::SaturationLayer(int index, float value) {
	
	if(index < layerCount && value > 0.0 && value < 1.0) {

		SaturatonBitmapArray(bmpLayers[index], value);
		RenderLayerToBmp(index);
		FastRenderCurrent(index);
	}
}

void BmpLayerHandler::SaturatonBitmapArray(BmpLayer *layer, float value) {
	
	DWORD *tempArray = new DWORD[layer->GetLayerBitCount()];
	DWORD *bmpArray = layer->GetLayerBits();

	unsigned char ar=0,ag=0,ab=0;	
	unsigned char avg_color=0;

	/* SATURATION ALGORITHM */
	for(int y=0;y<layer->GetLayerBitCount();y++)
	{
		ar = GetRValue((bmpArray[y]));
		ag = GetGValue((bmpArray[y]));
		ab = GetBValue((bmpArray[y]));		
		
		avg_color = (ar+ag+ab)/3;

		tempArray[y] = RGB(ar*(1.0-value) + avg_color*value,ag*(1.0-value) + avg_color*value,ab*(1.0-value) + avg_color*value);
	
	}

	for(int a=0;a<layer->GetWidth()*layer->GetHeight();a++) {

		bmpArray[a] = tempArray[a];
	}

	delete [] tempArray;
}

void BmpLayerHandler::LightnessLayer(int index, float value) {

	if(index < layerCount) {

		if(value >= -1.0 && value <= 1.0) {

			LightnessBitmapArray(bmpLayers[index], value);
			RenderLayerToBmp(index);
			FastRenderCurrent(index);

		}
	}
}

void BmpLayerHandler::LightnessBitmapArray(BmpLayer *layer, float value) {

	DWORD *tempArray = new DWORD[layer->GetLayerBitCount()];
	DWORD *bmpArray = layer->GetLayerBits();

	unsigned char ar=0,ag=0,ab=0;	

	/* LIGHTNESS ALGORITHM */
	for(int y=0;y<layer->GetLayerBitCount();y++)
	{
		ar = GetRValue((bmpArray[y]));
		ag = GetGValue((bmpArray[y]));
		ab = GetBValue((bmpArray[y]));		
		

		if(value < 0) {

			for(int i=0;i<(255*value*-1);i++) {

				if(ar > 0)
					ar--;

				if(ag > 0)
					ag--;

				if(ab > 0)
					ab--;
			}
		}
		else {

			for(int i=0;i<(255*value);i++) {

				if(ar < 255)
					ar++;

				if(ag < 255)
					ag++;

				if(ab < 255)
					ab++;
			}
		}

		tempArray[y] = RGB(ar,ag,ab);	
	}

	for(int a=0;a<layer->GetLayerBitCount();a++) {

		bmpArray[a] = tempArray[a];
	}

	delete [] tempArray;
}

void BmpLayerHandler::Test(int index) {

	DrawPixel(bmpLayers[index],10,10,RGB(100,200,100));
	DrawRectangle(bmpLayers[index],30,30,100,90,RGB(40,0,255));
	DrawLine(bmpLayers[index],100,120,200,20,RGB(20,100,200));	
		
	DrawCircle(bmpLayers[index],120,120,30,RGB(255,0,90));

	DrawPixel(bmpLayers[index],40,200,RGB(100,200,100));
	

	DrawRectangle(bmpLayers[index],80,19,200,230,RGB(0,255,255));

	//RenderLayerToBmp(index);
}

void BmpLayerHandler::Test2(int index) {
/*
	DrawPixel(bmpLayers[index],15,15,RGB(100,200,100));
	DrawRectangle(bmpLayers[index],130,130,200,290,RGB(255,0,255));
			
	DrawCircle(bmpLayers[index],220,220,30,RGB(255,0,0));*/

	DrawRectangle(bmpLayers[index],10+temp_left,10+temp_right,60+temp_left,60+temp_right,RGB(30,70,150));
	

	//RenderLayerToBmp(index);
}

void BmpLayerHandler::SetLayerOpacity(int index, float opacity) {

	if(index < layerCount) {

		bmpLayers[index]->SetLayerOpacity(opacity);
	}
}

DWORD BmpLayerHandler::dRGB(DWORD rgb) {

	char r,g,b;

	r = (char)rgb;
	g = (char)(rgb >> 8);
	b = (char)(rgb >> 16);

	return RGB(b,g,r);
}

void BmpLayerHandler::ClearLayer(BmpLayer *layer, DWORD color) {

	DWORD dColor = dRGB(color);
	DWORD *bmpArray = layer->GetLayerBits();
	
	int count = layer->GetLayerBitCount();

	for(int i=0;i<count;i++) {
				
			bmpArray[i] = dColor;
	}
}

void BmpLayerHandler::ClearLayer(int index, DWORD color) {

	DWORD dColor = dRGB(color);
	DWORD *bmpArray = bmpLayers[index]->GetLayerBits();
	
	int count = bmpLayers[index]->GetLayerBitCount();

	for(int i=0;i<count;i++) {
				
			bmpArray[i] = dColor;
	}
}

void BmpLayerHandler::DrawPixel(BmpLayer *layer, int x, int y, DWORD color) {

	DWORD dColor = dRGB(color);

	if(x >= 0 && y >= 0 && x <= layer->GetWidth() && y <= layer->GetHeight()) {
	
		DWORD *bmpArray = layer->GetLayerBits();
		bmpArray[(layer->GetHeight()-y)*layer->GetWidth()+x] = dColor;
	}
}

void BmpLayerHandler::DrawRectangle(BmpLayer *layer, int x, int y, int width, int height, DWORD fillcolor) {

	DWORD dColor = dRGB(fillcolor);

	if(x >= 0 && y >= 0 && x <= layer->GetWidth() && y <= layer->GetHeight()) {

		if(width < layer->GetWidth() && height < layer->GetHeight()) {

			DWORD *bmpArray = layer->GetLayerBits();

			for(int iy=y;iy<height;iy++)
			for(int ix=x;ix<width;ix++) {
			
				bmpArray[(layer->GetHeight()-iy)*layer->GetWidth()+ix] = dColor;
			}

		}
	}
}

void BmpLayerHandler::DrawCircle(BmpLayer *layer, int x, int y, int radius, DWORD fillcolor) {

	if(x >= 0 && y >= 0 && x <= layer->GetWidth() && y <= layer->GetHeight()) {

		int ix=0,iy=0;
		int ixa=0,iya=0;

		for(float i=0;i<=(3.141592654/2);i+=0.01) {
		
			ix = cos(i)*radius;
			iy = sin(i)*radius;

			ixa = cos(i)*radius;
		
			if(ixa < radius) {

				for(int a=(x-ixa);a<(x+ix);a++)
				DrawPixel(layer,x+a,y+iy,fillcolor);
			}			
		}
		
		for(float i=3.141592654;i>=-3.141592654;i-=0.01) {
			
			ix = cos(i)*radius;
			iy = sin(i)*radius;

			ixa = cos(i)*radius;

			for(int a=(x-ixa);a<(x+ix);a++)
			DrawPixel(layer,x+a,y+iy,fillcolor);
		}
	}

}

void BmpLayerHandler::DrawLine(BmpLayer *layer, int x, int y, int end_x, int end_y, DWORD color) {

	if(x >= 0 && y >= 0 && x <= layer->GetWidth() && y <= layer->GetHeight()) {

		// Line equation y = kx + m

		float k=0;

		// Calculate coefficient
		k = (float)(y-end_y)/(float)(x-end_x);

		int length=0;

		// Calculate length...
		length = sqrt(pow((end_x-x),2));

		for(int i=0;i<length;i++) {

			DrawPixel(layer,x+i,y+i*k,color);
		}
	}
}

void BmpLayerHandler::ReadBitmapAndRender(char *filename) {

	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;

	using namespace std;

    ifstream input_bitmap(filename);

	int size=bmih.biWidth*bmih.biHeight;
	DWORD *bmpArray = new DWORD[size];

	char r,g,b;		
			
	int current_y=0;
	int current_x=0;	
	
	HDC dc;
	HDC mem;
	HBITMAP bmp;

	SelectObject(mem,bmp);
	
	// Read the BITMAPFILEHEADER
	input_bitmap.read((char*)&bmfh,sizeof(bmfh));

	// Read the BITMAPINFOHEADER
	input_bitmap.read((char*)&bmih,sizeof(bmih));	
				
	dc = GetDC(hwndOwner);
	mem = CreateCompatibleDC(dc);
	bmp = CreateCompatibleBitmap(dc,bmih.biWidth,bmih.biHeight);

	SelectObject(mem,bmp);	

	if(input_bitmap.is_open()) {

		char temp=0;

		// Move read_cursor to bitmap bits
		for(int offset=0;offset<bmfh.bfOffBits;offset++)
			input_bitmap.read((char*)&temp,sizeof(char));

		//if(layerCount < 2)
		//NewLayer(bmih.biWidth, bmih.biHeight+1, RGB(0,0,0));

		// Read bmp-file for image data
		if(bmih.biCompression == BI_RGB && bmih.biBitCount == 24) {

			for(int i=0;i<bmih.biSizeImage;i++)	{

				if(!input_bitmap.eof()) {

					input_bitmap.read((char*)&r,sizeof(char));
					input_bitmap.read((char*)&g,sizeof(char));
					input_bitmap.read((char*)&b,sizeof(char));
					
					if(current_x == bmih.biWidth) {

						current_x = 0;
						current_y++;
					}
					else current_x++;

					//SetPixel(mem,current_x,bmih.biHeight-current_y,RGB(b,g,r));
					bmpArray[i] = RGB(rand()%255,rand()%255,rand()%255);
				}
			}
		}
		else MessageBox(0,"dsadsa","hdasds",0);

		//InvertBitmapArray(bmpArray,bmih.biWidth,bmih.biHeight);

		bmpLayers[layerCount-1]->AssignBitsToLayer(bmpArray);

		// Render shit
		BitBlt(dc,0,0,bmih.biWidth,bmih.biHeight,mem,0,0,SRCCOPY);

		DeleteDC(mem);
		DeleteDC(dc);
		DeleteObject(bmp);

		input_bitmap.close();
	}

	// Free memory
	delete [] bmpArray;
}
