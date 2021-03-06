////////////////////////////////////////
// Bitmap.h
////////////////////////////////////////

#ifndef CSE168_BITMAP_H
#define CSE168_BITMAP_H

#include "Core.h"

////////////////////////////////////////////////////////////////////////////////

class Bitmap {
public:
	Bitmap() {}
	Bitmap(int x,int y)						{XRes=x; YRes=y; Pixel=new int[XRes*YRes];}
	~Bitmap()								{delete []Pixel;}

	bool SaveBMP(const char *filename);
	//bool LoadBMP(char* filename, BitmapHeader *bitmapInfoHeader);

	int GetXRes() const						{return XRes;}
	int GetYRes() const						{return YRes;}
	int &GetPixel(int x,int y)				{return Pixel[y*XRes+x];}
	void SetPixel(int x,int y,int pix)		{Pixel[y*XRes+x]=pix;}
	void SetXRes(int x)						{XRes = x;}
	void SetYRes(int y)						{YRes = y;}

private:
	int XRes;
	int YRes;
	int *Pixel;
};

////////////////////////////////////////////////////////////////////////////////

#endif
