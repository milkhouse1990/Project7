#include "imageparameter.h"
//#include "IProcessB.h"  
#include <math.h>  

 
void GetImageParament(CImage *pImg, struct IMAGEPARAMENT *ppImgParam)
{
	if (pImg->IsNull()) return;

	ppImgParam->nWidth = pImg->GetWidth();      //图像宽度  
	ppImgParam->nHeight = pImg->GetHeight();        //图像高度  
	ppImgParam->nBitCount = pImg->GetBPP();      //每像素位数  
	ppImgParam->nBytesPerLine = (pImg->GetWidth()*pImg->GetBPP() + 31) / 32 * 4;   //每行字节数  
	ppImgParam->nBytesPerPixel = pImg->GetBPP() / 8;      //每像素字节数  
	if (pImg->GetBPP() <= 8)
		ppImgParam->nNumColors = 1 << pImg->GetBPP();        //调色板单元数  
	else
		ppImgParam->nNumColors = 0;
	ppImgParam->nSize = ppImgParam->nBytesPerLine*ppImgParam->nHeight;        //像素总字节数  
}

int  InImage(CImage *pImg, int x, int y)
{
	struct  IMAGEPARAMENT  P;

	GetImageParament(pImg, &P);
	if ((x<0) || (y<0) || (x >= P.nWidth) || (y >= P.nHeight))  return 0;
	else  return 1;
}



void SetRectValue(CImage *pImg, int x, int y, int Dx, int Dy, BYTE *buf)
{
	struct  IMAGEPARAMENT  P;
	BYTE    *lp;
	int     i, dw, dh, x1, y1, alpha, delta, Dxb, dwb;

	GetImageParament(pImg, &P);
	if (P.nBitCount<8) return;
	x1 = x;
	y1 = y;
	alpha = delta = 0;
	if (x<0) {
		alpha = -x;    x1 = 0;
	}
	if (y<0) {
		delta = -y;    y1 = 0;
	}
	if (!InImage(pImg, x1, y1)) return;
	dw = min(Dx, (int)P.nWidth - x1);
	dh = min(Dy, (int)P.nHeight - y1);
	dw -= alpha;
	dh -= delta;

	Dxb = Dx*P.nBytesPerPixel;
	dwb = dw*P.nBytesPerPixel;
	lp = (BYTE*)pImg->GetPixelAddress(x1, y1);
	buf += (delta*Dx + alpha)*P.nBytesPerPixel;
	for (i = 0; i<dh; i++) {
		memcpy(lp, buf, dwb);
		buf += Dxb;
		lp += P.nBytesPerLine;
	}
}

void GetAllPalette(CImage *pImg, RGBQUAD *ColorTab)
{
	struct  IMAGEPARAMENT P;

	GetImageParament(pImg, &P);
	pImg->GetColorTable(0, P.nNumColors, ColorTab);
}

void SetAllPalette(CImage *pImg, RGBQUAD *ColorTab)
{
	struct  IMAGEPARAMENT P;

	GetImageParament(pImg, &P);
	pImg->SetColorTable(0, P.nNumColors, ColorTab);
}



int   PaletteType(RGBQUAD  *ColorTab)
{
	int     i, k, m, n, r, g, b;

	m = n = 0;
	for (i = 0; i<256; i++)
	{
		r = ColorTab[i].rgbRed;
		g = ColorTab[i].rgbGreen;
		b = ColorTab[i].rgbBlue;
		if ((r != g) || (r != b)) m = 0;
		if ((i>0) && (r>ColorTab[i - 1].rgbRed)) m++;
		if (r + g + b == 0) n++;
	}
	k = 3;
	if (m == 255) k = 2;
	else  if (256 - n == 1) k = 0;
	else  if (256 - n == 15) k = 1;
	return(k);
}

int ImageType(CImage *pImgm)            //判断图像类型   
{
	RGBQUAD ColorTab[256];
	int     k;

	if (pImgm->IsNull()) return(0);

	switch (pImgm->GetBPP())
	{
	case 1:  k = 0;   break;
	case 4:  k = 1;   break;
	case 8:  k = 3;   break;
	default: k = 4;   break;
	}

	if (k == 3)
	{
		pImgm->GetColorTable(0, 256, ColorTab);
		k = PaletteType(ColorTab);
	}
	return(k);
}


/*图像旋转函数
Imgn/Imgm        目标图像/源图像指针
alpha            旋转角度,顺时针方向为正*/
void RotateCimage(CImage *Imgm, CImage *Imgn)
{
	IMAGEPARAMENT P;
	RGBQUAD ColorTab[256];
	int i, j;	
	BYTE **list, *sc;
	//int flag;
	

	//if (ImageType(Imgm) == 2)  flag = 1; //flag为标志位，当取值为1时，表示双线性内插法 ,此时图像类型为灰阶图像  
	//flag = 0; //0表示最近邻点法   
	GetImageParament(Imgm, &P);

	//还有一点要修改，不然当图像高度远大于宽度时会崩溃  
	sc = (BYTE*)malloc(P.nBytesPerLine);    // * P.nBytesPerLine); //申请工作单元   
	//  
	list = (BYTE**)malloc(P.nHeight * sizeof(BYTE*)); //对原位图建立二维数组   
	for (i = 0; i < P.nHeight; i++)
		list[i] = (BYTE*)Imgm->GetPixelAddress(0, i);
	Imgn->Destroy();
	Imgn->Create(P.nWidth, P.nHeight, P.nBitCount,CImage::createAlphaChannel); //建立结果位图   
	if (P.nBitCount == 8)
	{
		GetAllPalette(Imgm, ColorTab);
		//修改一，设置目标调色板  
		SetAllPalette(Imgn, ColorTab); //复制调色板   
	}
	

	for (j = 0; j < P.nHeight; j++)
	{
		if (P.nBitCount == 8)
			memset(sc, 0, 0); //256色位图像素行置背景值   
		else
			memset(sc, 256, P.nBytesPerLine); //真彩色位图像素行置背景值   

		for (i = 0; i < P.nWidth; i++)
		{
			//sc[Xd]  
			memcpy(&sc[i*P.nBytesPerPixel], &list[j][(P.nWidth - i - 1)*P.nBytesPerPixel], P.nBytesPerPixel); //从源位图复制像素数据   
		}
		SetRectValue(Imgn, 0, j, P.nWidth, 1, sc);
	}
	free(list); //释放工作单元   
	free(sc);

}