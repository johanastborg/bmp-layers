#include "bmplayer.h"

BmpLayer::BmpLayer(int width, int height, DWORD bg_color) {

	layerBits = new DWORD[width*height];

	for(int x=0;x<width*height;x++) {

		layerBits[x] = bg_color;
		
	}
	iwidth = width;
	iheight = height;

	fopacity = 1.00;
}

DWORD BmpLayer::GetLayerBit(int fast_index) {

	return layerBits[fast_index];

}

long BmpLayer::GetLayerBitCount() {

	return iwidth*iheight;
}

int BmpLayer::GetHeight() {

	return iheight;
}

int BmpLayer::GetWidth() {

	return iwidth;
}

void BmpLayer::AssignBitsToLayer(DWORD *bmpBits) {

	for(int i=0;i<iwidth*iheight;i++) {

		layerBits[i] = bmpBits[i];
	}
}

DWORD *BmpLayer::GetLayerBits() {

	return layerBits;
}

void BmpLayer::SetLayerOpacity(float opacity) {

	fopacity = opacity;
}

float BmpLayer::GetLayerOpacity() {

	return fopacity;
}