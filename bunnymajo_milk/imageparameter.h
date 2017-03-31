#pragma once
#ifndef     _IPROCESSB_H  
#define     _IPROCESSB_H  
#include <atlimage.h>  

struct IMAGEPARAMENT {
	int     nWidth;
	int     nHeight;
	int     nBitCount;
	int     nBytesPerLine;
	int     nBytesPerPixel;
	int     nNumColors;
	int     nSize;
};

void  GetImageParament(CImage *pImg, struct IMAGEPARAMENT *ppImgParam);
int   InImage(CImage *pImg, int x, int y);
//   
//DWORD GetPixelValue(CImage *pImg,int x,int y);   
//void  SetPixelValue(CImage *pImg,int x,int y,DWORD c);   
//void  GetRectValue(CImage *pImg,int x,int y,int Dx,int Dy,BYTE *buf);   
void  SetRectValue(CImage *pImg, int x, int y, int Dx, int Dy, BYTE *buf);
//   
//void  GetPalette(CImage *pImg,int n,int *pr,int *pg,int *pb);   
//void  SetPalette(CImage *pImg,int n,int r,int g,int b);   
void  GetAllPalette(CImage *pImg, RGBQUAD *ColorTab);
void  SetAllPalette(CImage *pImg, RGBQUAD *ColorTab);
//   
//void  RectFillValue(CImage *pImg,int x,int y,int Dx,int Dy,DWORD color);   
//void  DrawBox(CImage *pImg,int x,int y,int Dx,int Dy,int w,DWORD c);   
//void  SetDefaultPalette(CImage *pImg);   
//   
int   PaletteType(RGBQUAD   *ColorTab);
int   ImageType(CImage *pImgm);
//void  ImageCopy(CImage *pImgn,CImage *pImgm);   
//   
//BYTE** Create2DList(CImage *pImgm);   
//void  Release2DList(BYTE** list);   
//   
//COLORREF DwordToColorref(DWORD c);   

void RotateCimage(CImage *Imgm, CImage *Imgn);

#endif  //!_IPROCESSB_H