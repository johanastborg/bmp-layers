#ifndef BMP_LAYER_HANDLER_H
#define BMP_LAYER_HANDLER_H

#include <windows.h>
#include <fstream>
#include <math.h>
#include "bmplayer.h"

#define TRANSPARENT 0x104501

class BmpLayerHandler {

public:

	BmpLayerHandler(int width, int height);

	void AssignRendererToWnd(HWND hwnd);
	void Render();
	void NewLayer(COLORREF base_color);
	void NewLayer(int width, int height, COLORREF base_color);
	void ContractLayers();

	void RenderLayerToBmp(int index);
	void FastRenderCurrent(int index);
	void ReadBitmapAndRender(char *filename);

	// Testing
	void Test(int index);
	void Test2(int index);

	DWORD dRGB(DWORD rgb);

	// Drawing
	void DrawPixel(BmpLayer *layer,int x,int y, DWORD color);
	void DrawRectangle(BmpLayer *layer, int x, int y, int width, int height, DWORD fillcolor);
	void DrawLine(BmpLayer *layer, int x, int y, int end_x, int end_y, DWORD color);
	void DrawCircle(BmpLayer *layer, int x, int y, int radius, DWORD fillcolor);
	
	void ClearLayer(BmpLayer *layer, DWORD color);
	void ClearLayer(int index, DWORD color);

	// Testing
	//void AddObject(Rectangle *object);

	// Filters / Effects
	void InvertLayer(int index);
	void InvertBitmapArray(DWORD *bmpArray, int width, int height);

	void BlurLayer(int index);
	void BlurBitmapArray(BmpLayer *layer);

	void SaturationLayer(int index, float value);
	void SaturatonBitmapArray(BmpLayer *layer, float value);

	void LightnessLayer(int index, float value);
	void LightnessBitmapArray(BmpLayer *layer, float value);

	// Layer properties
	void SetLayerOpacity(int index, float opacity);
	float GetLayerOpacity(int index);
	
	// testing
	int temp_left;
	int temp_right;

private:

	COLORREF DwordToColorRef(DWORD data);

	HWND hwndOwner;
	DWORD *bmpMain;
	long bmpMainBitCount;
	int iwidth, iheight;

	BmpLayer *bmpLayers[99];
	int layerCount;

	HBITMAP bmp_rnd;

};

#endif