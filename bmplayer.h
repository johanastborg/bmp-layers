#ifndef BMP_LAYER_H
#define BMP_LAYER_H

#include <windows.h>

class BmpLayer {

public:

	BmpLayer(int width, int height, DWORD bg_color);
	DWORD GetLayerBit(int fast_index);
	void AssignBitsToLayer(DWORD *bmpBits);
	long GetLayerBitCount();
	int GetWidth();
	int GetHeight();
	DWORD *GetLayerBits();

	void SetLayerOpacity(float opacity);
	float GetLayerOpacity();
	//Filters (testing)
	

private:

	DWORD *layerBits;
	long layerBitCount;
	int iwidth;
	int iheight;

	float fopacity;
};

#endif