#include "imageparameter.h"
//#include "IProcessB.h"  
#include <math.h>  

 
void GetImageParament(CImage *pImg, struct IMAGEPARAMENT *ppImgParam)
{
	if (pImg->IsNull()) return;

	ppImgParam->nWidth = pImg->GetWidth();      //ͼ����  
	ppImgParam->nHeight = pImg->GetHeight();        //ͼ��߶�  
	ppImgParam->nBitCount = pImg->GetBPP();      //ÿ����λ��  
	ppImgParam->nBytesPerLine = (pImg->GetWidth()*pImg->GetBPP() + 31) / 32 * 4;   //ÿ���ֽ���  
	ppImgParam->nBytesPerPixel = pImg->GetBPP() / 8;      //ÿ�����ֽ���  
	if (pImg->GetBPP() <= 8)
		ppImgParam->nNumColors = 1 << pImg->GetBPP();        //��ɫ�嵥Ԫ��  
	else
		ppImgParam->nNumColors = 0;
	ppImgParam->nSize = ppImgParam->nBytesPerLine*ppImgParam->nHeight;        //�������ֽ���  
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

int ImageType(CImage *pImgm)            //�ж�ͼ������   
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


/*ͼ����ת����
Imgn/Imgm        Ŀ��ͼ��/Դͼ��ָ��
alpha            ��ת�Ƕ�,˳ʱ�뷽��Ϊ��*/
void RotateCimage(CImage *Imgm, CImage *Imgn)
{
	IMAGEPARAMENT P;
	RGBQUAD ColorTab[256];
	int i, j;	
	BYTE **list, *sc;
	//int flag;
	

	//if (ImageType(Imgm) == 2)  flag = 1; //flagΪ��־λ����ȡֵΪ1ʱ����ʾ˫�����ڲ巨 ,��ʱͼ������Ϊ�ҽ�ͼ��  
	//flag = 0; //0��ʾ����ڵ㷨   
	GetImageParament(Imgm, &P);

	//����һ��Ҫ�޸ģ���Ȼ��ͼ��߶�Զ���ڿ��ʱ�����  
	sc = (BYTE*)malloc(P.nBytesPerLine);    // * P.nBytesPerLine); //���빤����Ԫ   
	//  
	list = (BYTE**)malloc(P.nHeight * sizeof(BYTE*)); //��ԭλͼ������ά����   
	for (i = 0; i < P.nHeight; i++)
		list[i] = (BYTE*)Imgm->GetPixelAddress(0, i);
	Imgn->Destroy();
	Imgn->Create(P.nWidth, P.nHeight, P.nBitCount,CImage::createAlphaChannel); //�������λͼ   
	if (P.nBitCount == 8)
	{
		GetAllPalette(Imgm, ColorTab);
		//�޸�һ������Ŀ���ɫ��  
		SetAllPalette(Imgn, ColorTab); //���Ƶ�ɫ��   
	}
	

	for (j = 0; j < P.nHeight; j++)
	{
		if (P.nBitCount == 8)
			memset(sc, 0, 0); //256ɫλͼ�������ñ���ֵ   
		else
			memset(sc, 256, P.nBytesPerLine); //���ɫλͼ�������ñ���ֵ   

		for (i = 0; i < P.nWidth; i++)
		{
			//sc[Xd]  
			memcpy(&sc[i*P.nBytesPerPixel], &list[j][(P.nWidth - i - 1)*P.nBytesPerPixel], P.nBytesPerPixel); //��Դλͼ������������   
		}
		SetRectValue(Imgn, 0, j, P.nWidth, 1, sc);
	}
	free(list); //�ͷŹ�����Ԫ   
	free(sc);

}