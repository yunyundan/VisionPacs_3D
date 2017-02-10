#pragma once

//#include "AnnManager.h"//注释体处理

#include "stdafx.h"
#include "AtlImage.h"

extern float SinPiX(const float &x);
extern float Matrix_Bicubic(const float (&A)[4], const float (&B)[4][4], const float (&C)[4]);

template <class T1>
void GetPixValueFromLut(T1 &nPixValue,LutData *pLut)
{
	if(pLut==NULL)
		return ;


	T1 *pData = (T1*)(pLut->pLutData);

	if(nPixValue<=pLut->nMinValue)
		nPixValue = pData[0];
	else if(nPixValue>=pLut->nMaxValue)
		nPixValue = pData[pLut->nLutLen-1];
	else
		nPixValue = pData[nPixValue - pLut->nMinValue];
}
template <class T2>
void FillBits( T2** pDisplayData,BYTE *pBits,BYTE** pDisplayOverlayData,unsigned long nRow,unsigned long nCol,bool bShowOverLay,bool bInverse,const QVector<LutData*> &pLutUsed,long *pLut,RECT* pValidRc)
{
	memset(pBits,0,nRow*nCol);
	int nLut = pLutUsed.size();
	unsigned long i = 0;
	for (long r=nRow-1;r>=0;r--)
	{

		for (long c=0;c<nCol;c++)
		{
			if (pValidRc)
			{
				if(c<=pValidRc->left || c>=pValidRc->right || r<=pValidRc->top || r>=pValidRc->bottom)
				{
					i++;
					continue;
				}
			}

			if (pDisplayOverlayData && bShowOverLay)
			{
				if(pDisplayOverlayData[r][c])
				{
					pBits[i] = 255; 

					i++;
					continue;
				}
			}

			T2 v = pDisplayData[r][c];
			for(int u=0;u<nLut;u++)
				GetPixValueFromLut(v,pLutUsed[u]);

			if(bInverse)
				pBits[i] = 255 - pLut[v];
			else
				pBits[i] = pLut[v];
			//if(v==-127 || v==129)
			//	ATLTRACE("\r\n------pLut[%d] = %d",v,pLut[v]);
			i++;
		}

	}

}

template <class T3>
void ImgSizeOneSampleNormal(T3** pNewData,unsigned long nNewRow,unsigned long nNewCol,T3 **pOriData,unsigned long nOriRow,unsigned long nOriCol, long x0, long y0)
{
	float fZoomX = (nOriCol-x0)*1.00/nNewCol;
	float fZoomY = (nOriRow-y0)*1.00/nNewRow;

	unsigned long tr = 0;
	unsigned long tc = 0;

	for(unsigned long r=0;r<nNewRow;r++)
	{
		tr = y0+(unsigned long)(r*fZoomY);
		for(unsigned long c=0;c<nNewCol;c++)
		{
			tc = x0+(unsigned long)(c*fZoomX);
			pNewData[r][c] = pOriData[tr][tc];					
		}
	}
}
template <class T4>
void ImgSizeOneSampleResample(T4** pNewData,unsigned long nNewRow,unsigned long nNewCol,T4 **pOriData,unsigned long nOriRow,unsigned long nOriCol, long x0, long y0,long x1,long y1,BYTE** pOriOverLayData=NULL)
{
	float fZoomX = (x1-x0)*1.00/nNewCol;
	float fZoomY = (y1-y0)*1.00/nNewRow;


	//这些Row变量,将被计算nNewRow次
	float fr = 0.00;
	int Y = 0;
	float fy = 0.00;

	//这些Col变量若放入二次循环内将被计算 nNewRow*nNewCol次.重复了nNewRow*(nNewCol-1)次,所以要提出来
	float fc =0.00;
	int *X = new int[nNewCol];		memset(X, 0,nNewCol*sizeof(int));
	float *fx = new float[nNewCol];	memset(fx,0,nNewCol*sizeof(float));


	int preC = 0;
	for (int c=0;c<nNewCol;c++)
	{
		fc = c*fZoomX;
		X[c] = int(fc) + x0;
		fx[c] = float(fc- X[c] + x0);


		if(X[c]+2>=nOriCol)//发现要越界
		{
			fc = preC*fZoomX;//.就用最后一次正确的c也就是preC,来计算
			X[c] = int(fc) + x0;
			fx[c] = float(fc- X[c] + x0);
		}
		else
		{
			preC = c;
		}

	}

	int PreR = 0;
	for(int r=0;r<nNewRow;r++)
	{
		fr = r*fZoomY;
		Y = int(fr) + y0;
		fy = float(fr - Y + y0);


		if ( Y>=nOriRow-2 )//发现要越界.
		{
			fr = PreR*fZoomY;//就用最后一次正确的r也就是preR,来计算
			Y = int(fr) + y0;
			fy = float(fr - Y + y0);
		}
		else
		{
			PreR = r;
		}

		for(int c=0;c<nNewCol;c++)
		{
			//T4 v1 = pNewData[r][c];
			//double s1 = (1-fy)*(1-fx[c])*pOriData[Y+1][X[c]+1];
			//double s2 = (1-fy)* fx[c]	*pOriData[Y+1][X[c]+2];
			//double s3 = fy*(1-fx[c])	*pOriData[Y+2][X[c]+1];
			//double s4 = fy*fx[c]		*pOriData[Y+2][X[c]+2];

			if(pOriOverLayData == NULL)
			{
				pNewData[r][c] = (T4)(
					(1-fy)*(1-fx[c])*pOriData[Y+1][X[c]+1] + 
					(1-fy)* fx[c]	*pOriData[Y+1][X[c]+2] + 
					fy*(1-fx[c])	*pOriData[Y+2][X[c]+1] +
					fy*fx[c]		*pOriData[Y+2][X[c]+2]   
				);//提取后
			}
			else//这是显示Overlay的另一种方法。这种方法的OverLay会随着wc的变化，也跟着变
			{
				pNewData[r][c] = (T4)(
					(1-fy)*(1-fx[c])*( (pOriOverLayData[Y+1][X[c]+1] ? 16000 : pOriData[Y+1][X[c]+1]) ) + 
					(1-fy)* fx[c]	*( (pOriOverLayData[Y+1][X[c]+2] ? 16000 : pOriData[Y+1][X[c]+2]) ) + 
					fy*(1-fx[c])	*( (pOriOverLayData[Y+2][X[c]+1] ? 16000 : pOriData[Y+2][X[c]+1]) ) +
					fy*fx[c]		*( (pOriOverLayData[Y+2][X[c]+2] ? 16000 : pOriData[Y+2][X[c]+2]) )
					);
			}

			T4 v = pNewData[r][c];
			if( v == 64619)
			{
				int aaa = 0;//此处加断点
			}

		}

		//for(int c=0;c<nNewCol;c++)//优化前:列变量提取前
		//{
		//	float fc = c*fZoomX;
		//	int X = int(fc) + x0;
		//	float fx = float(fc- X + x0);

		//	if ( X>=nOriCol-2 )
		//		X = nOriCol-3;

		//	pNewData[r][c] = (T4)(   (1-fy)*(1-fx)*pOriData[Y+1][X+1]+(1-fy)*fx*pOriData[Y+1][X+2]+fy*(1-fx)*pOriData[Y+2][X+1]+fy*fx*pOriData[Y+2][X+2]   );
		//}
	}

	delete []X;
	delete []fx;
}
template <class T5>
void ImgSizeOneSampleBicubic(T5** pNewData,unsigned long nNewRow,unsigned long nNewCol,T5 **pOriData,unsigned long nOriRow,unsigned long nOriCol, long x0, long y0,long nMinPixValue,long nMaxPixValue)
{
	float fZoomX = (nOriCol-x0)*1.00/nNewCol;
	float fZoomY = (nOriRow-y0)*1.00/nNewRow;

	float A_Matrix[4]	={1,1,1,0};
	float B_Matrix[4][4]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	float C_Matrix[4]	={0,0,0,0};

	float i = 0.00;
	float j = 0.00;
	float u = 0.00;
	float v = 0.00;

	float tmp = 0.00;

	float fr = 0;
	float fc = 0;
	int tr=0, tc=0;

	for(unsigned long r=0;r<nNewRow;r++)
	{
		fr = y0+r*fZoomY;

		i = ((unsigned long)fr);
		u = fr - i;

		for(unsigned long c=0;c<nNewCol;c++)
		{
			fc = x0 + c*fZoomX;

			j = ((unsigned long)fc);
			v = fc - j;

			for (BYTE t=0; t<4; t++)
			{
				A_Matrix[t] = SinPiX(u-t+1);
				C_Matrix[t] = SinPiX(v-t+1);
				for (BYTE f=0; f<4; f++)
				{
					tr = i+t-1;
					tc = j+f-1;
					if(tc>=nOriCol) tc = nOriCol-1;
					if(tr>=nOriRow) tr = nOriRow-1;
					if(tc<=x0) tc = x0;
					if(tr<=y0) tr = y0;
					B_Matrix[t][f] = pOriData[tr][tc];
				}
			}

			tmp = Matrix_Bicubic(A_Matrix,B_Matrix,C_Matrix);

			if ( tmp < nMinPixValue)
				pNewData[r][c] = nMinPixValue;
			else if ( tmp >= nMaxPixValue)
				pNewData[r][c] = nMaxPixValue - 1; //m_imgInfo.HS_LUT_length-1;
			else
				pNewData[r][c] = (T5)tmp;
		}
	}

}
template <class T6>
int ImageCut(T6** pOldData,RECT CutRc,T6** pNewData)
{
	if(pNewData==NULL || pOldData==NULL)
		return Ret_InvalidPara;

	for (int r=CutRc.top;r<CutRc.bottom;r++)
	{
		for (int c=CutRc.left;c<CutRc.right;c++)
		{
			pNewData[r-CutRc.top][c-CutRc.left] = pOldData[r][c];
		}
	}

	return Ret_Success;
}

template <class T7>
int GetMaxMinPix(T7** pOriData,const RECT& PixRc, long& nMin, long& nMax)
{
	nMax = -100000;
	nMin = 100000;

	for (long r=PixRc.top;  r<PixRc.bottom;  r++)
	{
		for (long c=PixRc.left;  c<PixRc.right; c++)
		{
			nMin = min(nMin,long(pOriData[r][c]));
			nMax = max(nMax,long(pOriData[r][c]));
		}
	}

	if (nMin>nMax)
		return Ret_GetPixValueFailed;

	return Ret_Success;
}

template <class T8>
int TransformPix(T8** pOriData,long nRow,long nCol,int nType,long &lpNewOriData, unsigned long &nNewRow,	unsigned long &nNewCol)
{
	if(nType<1 || nType>4)
		return Ret_InvalidPara;

	nCol = ((nCol*8)+31)/32*4;

	long c = 0;//相对于pOriData
	long r = 0;//相对于pOriData

	T8** pNewOriData = NULL;
	if (nType==1)//顺时针90度,就是把旧矩阵的第一列像素变成新矩阵的第一行像素
	{
		pNewOriData = (T8**)ArrayNew(nCol,nRow,sizeof(T8),&nNewRow, &nNewCol);
		for ( c=0;  c<nCol;  c++)
		{
			for ( r=nRow-1;  r>=0; r--)
				pNewOriData[c][nRow-r-1] = pOriData[r][c];
		}
	}
	else if(nType==2)//逆时针90,最后列变第一行
	{
		pNewOriData = (T8**)ArrayNew(nCol,nRow,sizeof(T8),&nNewRow, &nNewCol);
		for ( c=nCol-1;  c>=0;  c--)
		{
			for ( r=0;  r<nRow; r++)
				pNewOriData[nCol-c-1][r] = pOriData[r][c];
		}
	}
	else if (nType==3)//上下翻转,flip,第一行变最后行
	{
		pNewOriData = (T8**)ArrayNew(nRow,nCol,sizeof(T8),&nNewRow, &nNewCol);

		int nRowLen = sizeof(T8)*nCol;
		for ( r=0;  r<nRow;  r++)
			memcpy(pNewOriData[nRow-r-1],pOriData[r],nRowLen);//每行都是连续内存,直接拷贝
	}
	else if (nType==4)//左右镜像 mirror,第一列变最后列
	{
		pNewOriData = (T8**)ArrayNew(nRow,nCol,sizeof(T8),&nNewRow, &nNewCol);

		for (r=0; r<nRow;  r++)
		{
			for (c=0; c<nCol; c++)
			{
				pNewOriData[r][c] = pOriData[r][nCol-c-1];
			}
		}
	}


	lpNewOriData = long(pNewOriData);

	return Ret_Success;
}

template <class T9>
int ImgSubstract(T9** pOriDataA,T9** pOriDataB,unsigned long nRow,unsigned long nCol,T9 nMinValue,T9 nMaxValue)
{//A=A-B

	//T9 tMin = nMaxValue;
	//T9 tMax = nMinValue;

	INT64 t = 0;
	for (int r=0;r<nRow;r++)
	{
		for (int c=0;c<nCol;c++)
		{
			t = (pOriDataA[r][c] - pOriDataB[r][c]);
			t = (t + nMaxValue)/2;

			if (t<nMinValue) 
				pOriDataA[r][c] = nMinValue;
			else if(t>nMaxValue)
				pOriDataA[r][c] = nMaxValue;
			else
				pOriDataA[r][c] = t;

			//tMin = min(tMin,t);
			//tMax = max(tMax,t);
		}
	}

	//nMaxValue = tMax;
	//nMinValue = tMin;

	return Ret_Success;
}
template <class TA>
int FillPixel(TA**pOriData,TA** pNewOriData, RECT rcInCurImg,int nSelectPixW,int nSelectPixH,int nNewWidth,int nNewHeight,int nBytePerPix,TA  nSmallestPixelValue)
{
	if (nBytePerPix!=3)
	{
		for (int r=0; r<nNewHeight;	r++)
		{
			for (int c=0;  c<nNewWidth;  c++)
				pNewOriData[r][c] = nSmallestPixelValue;
		}

	}

	//开始向里面拷当前图像的rcInCurImg区域内的像素
	//计算在新图像内像素们该呆的位置
	RECT rcNewImg = {0,0,nNewWidth-1,nNewHeight-1};//整个新图的区域
	RECT rcPixel;
	rcPixel.left = (rcNewImg.left + rcNewImg.right)/2 - nSelectPixW/2;
	rcPixel.top = (rcNewImg.top + rcNewImg.bottom)/2 - nSelectPixH/2;
	rcPixel.right = rcPixel.left + nSelectPixW;
	rcPixel.bottom = rcPixel.top + nSelectPixH;

	for (int r=rcPixel.top; r<rcPixel.bottom;	r++)
		memcpy(&pNewOriData[r][rcPixel.left],  &pOriData[rcInCurImg.top + r - rcPixel.top][rcInCurImg.left], nSelectPixW*nBytePerPix);

	return 0;
}

template <class T>
bool ConvertCoordForTransformation(T &x,T &y,long w,long h,bool HorFlip,int nAngle)
{
	if (nAngle < 0)
		nAngle += 360;

	T dfinalW;
	T newX;
	T newY;

	dfinalW = w;
	if (nAngle == 0)
	{
		newX = x; newY = y;
	}
	if (nAngle == 90)
	{
		newX = h - y + 1; newY = x;
		dfinalW = h;
	}
	else if (nAngle == 180)
	{
		newX = w - x + 1; newY = h - y + 1;
	}
	else if (nAngle == 270)
	{
		newX = y ; newY = w - x + 1;
		dfinalW = h;
	}

	if (HorFlip == true)
	{
		x = dfinalW - newX + 1; y = newY;
	}
	else
	{
		x = newX; y = newY;
	}

	return true;
}

template <class S>
S** SharpImage(S** pData,unsigned long nRow,unsigned long nCol,unsigned short nBitStore,INT64 nMinValue,INT64 nMaxValue,int v)
{
	INT64 t11,t12,t13,t21,t22,t23,t31,t32,t33;
	S** tempImgData = (S**)ArrayNew(nRow,nCol,sizeof(S));

	long GrayCount=long(pow(2.00,nBitStore*1.00));

	for (int r=0; r<nRow; r++)
	{
		for (int c=0; c<nCol; c++)
		{
			t22 = pData[r][c];

			if(r>0 && c>0 && r<nRow-1 && c<nCol-1)
			{
				t11 = pData[r-1][c-1];
				t12 = pData[r-1][c  ];
				t13 = pData[r-1][c+1];

				t21 = pData[r  ][c-1];
				t23 = pData[r  ][c+1];

				t31 = pData[r+1][c-1];
				t32 = pData[r+1][c  ];
				t33 = pData[r+1][c+1];

				t22 = t22+v*(t22-(t11+t12+t13+t21+t23+t31+t32+t33)/8);
			}

			//if(t22>GrayCount-1)
			//	t22=GrayCount-1;
			//if(t22<0)
			//	t22=0;

			if(t22 > nMaxValue) t22 = (S)nMaxValue;
			if(t22 < nMinValue) t22 = (S)nMinValue;

			tempImgData[r][c] = (S)t22;
		}
	}

	//for (long r=0; r<nRow;r++)
	//{
	//	for (long column=0; column<nCol; column++)
	//	{
	//		pData[r][column] = tempImgData[r][column];
	//	}
	//}


	return tempImgData;
}

template <class U>
U** SharpImageUSM(U** pData,unsigned long nRow,unsigned long nCol,unsigned short nBitStore,INT64 nMinValue,INT64 nMaxValue,int v,HWND hwd)//nFaZhi,int nAmount,int nRad
{
	int rad = 6;//模糊时的半径

	INT64 nFaZhi = nBitStore * 2;//模糊出的像素 与 原像素 的差值 超过 多少，才动手突出这个像素

	INT64 t22,tMoHu,Value;

	int nPix = 0;
	INT64 nPlusAll= 0;

//	if(hwd)
//		::SendMessage(hwd,WM_ShowSharpPro,0,-1);

	double fStep = 0.00;
	U** tempImgData = (U**)ArrayNew(nRow,nCol,sizeof(U));
	for (int r=0; r<nRow; r++)
	{
		//if(hwd)
		//{
		//	double t = (r+1)*1.00/nRow;
		//	if(t - fStep > 0.01)
		//	{
		//		::SendMessage(hwd,WM_ShowSharpPro,0,t*100);
		//		fStep = t;
		//	}
		//}

		for (int c=0; c<nCol; c++)
		{
			t22 = pData[r][c];

			//模糊
			nPix = 0;
			nPlusAll= 0;
			for (int mr=r-rad;mr<=r+rad;mr++)
			{
				if(mr<0) 
					continue;
				if(mr>=nRow) 
					continue;

				for (int mc=c-rad;mc<=c+rad;mc++)
				{
					if(mc<0) 
						continue;
					if(mc>=nCol) 
						continue;

					if(mr == r && mc == c)
						continue;

					nPlusAll += pData[mr][mc];
					nPix++;
				}
			}			

			tMoHu = nPlusAll*1.00/nPix;

			//看看与模糊值差距
			Value = t22 - tMoHu;

			//差距超过阀值，则突出这个像素
			if (abs(Value) > nFaZhi)
			{
				Value = t22 + v * Value*1.00 / 100;//1232
				t22 = Value;
			}

			if(t22 > nMaxValue) 
				t22 = nMaxValue;

			if(t22 < nMinValue) 
				t22 = nMinValue;

			tempImgData[r][c] = (U)t22;

		}
	}

//	if(hwd)
//		::SendMessage(hwd,WM_ShowSharpPro,0,-100);

	return tempImgData;

}


template <class T4>
void ImgSizeOverlay(T4** pNewData,unsigned long nNewRow,unsigned long nNewCol,T4 **pOriOverlayData,T4 **pOriPixData,unsigned long nOriRow,unsigned long nOriCol, long x0, long y0,long x1,long y1,int nSizeType)
{
	if (nSizeType == HSIZE_RESAMPLE)
	{
		float fZoomX = (x1-x0)*1.00/nNewCol;
		float fZoomY = (y1-y0)*1.00/nNewRow;


		//这些Row变量,将被计算nNewRow次
		float fr = 0.00;
		int Y = 0;
		float fy = 0.00;

		//这些Col变量若放入二次循环内将被计算 nNewRow*nNewCol次.重复了nNewRow*(nNewCol-1)次,所以要提出来
		float fc =0.00;
		int *X = new int[nNewCol];		memset(X, 0,nNewCol*sizeof(int));
		float *fx = new float[nNewCol];	memset(fx,0,nNewCol*sizeof(float));


		int preC = 0;
		for (int c=0;c<nNewCol;c++)
		{
			fc = c*fZoomX;
			X[c] = int(fc) + x0;
			fx[c] = float(fc- X[c] + x0);


			if(X[c]+2>=nOriCol)//发现要越界
			{
				fc = preC*fZoomX;//.就用最后一次正确的c也就是preC,来计算
				X[c] = int(fc) + x0;
				fx[c] = float(fc- X[c] + x0);
			}
			else
			{
				preC = c;
			}

		}

		int PreR = 0;
		for(int r=0;r<nNewRow;r++)
		{
			fr = r*fZoomY;
			Y = int(fr) + y0;
			fy = float(fr - Y + y0);


			if ( Y>=nOriRow-2 )//发现要越界.
			{
				fr = PreR*fZoomY;//就用最后一次正确的r也就是preR,来计算
				Y = int(fr) + y0;
				fy = float(fr - Y + y0);
			}
			else
			{
				PreR = r;
			}

			for(int c=0;c<nNewCol;c++)
			{
				pNewData[r][c] = (T4)(
					(1-fy)*(1-fx[c])*pOriOverlayData[Y+1][X[c]+1]==0 ? pOriPixData[Y+1][X[c]+1] : pOriOverlayData[Y+1][X[c]+1] + 
					(1-fy)* fx[c]	*pOriOverlayData[Y+1][X[c]+2]==0 ? pOriPixData[Y+1][X[c]+2] : pOriOverlayData[Y+1][X[c]+2] + 
					fy*(1-fx[c])	*pOriOverlayData[Y+2][X[c]+1]==0 ? pOriPixData[Y+2][X[c]+1] : pOriOverlayData[Y+2][X[c]+1] +
					fy*fx[c]		*pOriOverlayData[Y+2][X[c]+2]==0 ? pOriPixData[Y+2][X[c]+2] : pOriOverlayData[Y+2][X[c]+2]  
				);

			}
		}

		delete []X;
		delete []fx;
	}

}

class CHsFile;
class InfoInstance;
class CAnnoMprRotateLine;
class CDicomPr;
class CHsBaseImg
{
public:
	CHsBaseImg(void);
	~CHsBaseImg(void);

	// 图像的原始数据
	BYTE ** m_pOriData;

	// m_pOriData经过插值加工之后的像素,据此可生成显示用的m_pBits;
	BYTE ** m_pDisplayData;

	// 覆盖图原始像素
	BYTE **m_pOriOverlayData;

	// 覆盖图显示用像素
	BYTE **m_pDisplayOverlayData;

	//图像原始信息
	ImageInfo m_ImgInfo;

	//当前图像状态,效果等参数
	ImgState m_ImgState;

	//所有Lut.
	QVector <LutData*> m_pLutV;

	//显示在哪个HWND上
	HWND m_hWnd;

	SIZE m_PreSize;//Hs_Size函数接受过的参数,
	short m_nPreSizeType;//Hs_Size函数接受过的插值类型
	RECT *m_pPreSizeRc;//Hs_Size函数接受过的矩形区域

	//HMY
	bool m_bMpr;//标记此pImg是否为用来做mpr;
	QString m_sImgMprMode;////MPR时，效果是否为MIP,false时为average
protected:

	//最终显示出的像素(0-255)
	BYTE *m_pBits;

	//WL LUT
	long *m_pWcLut;

	//更新显示用的m_pBits
	int UpdateBits(unsigned long nRow,unsigned long nCol,BYTE** pDisplayData, BYTE** pDisplayOverlayData,BYTE* &pBits,RECT* pValidRc=NULL);
	int UpdateBitsNormal(unsigned long nRow,unsigned long nCol,BYTE** pDisplayData,BYTE** pDisplayOverlayData,BYTE* pBits,RECT* pValidRc=NULL);
	int UpdateBitsPaletteColor(unsigned long nRow,unsigned long nCol,BYTE** pDisplayData,BYTE** pDisplayOverlayData,BYTE* pBits);

	void ImgSizeThreeSample(MYDATA24** pNewData,unsigned long nNewRow,unsigned long nNewCol,MYDATA24 **pOriData,unsigned long nOriRow,unsigned long nOriCol, long x0, long y0,long x1,long y1,short nSizeType);

	//每个Lut都有名字,根据名字取其指针
	LutData *GetLutByName(QString sLutName);

	//普通图像调窗
	virtual int WinLevelNormal(long w,long c,bool bUpdateBits = true);

	//插值
	int ImgSize(BYTE ** pRetData,BYTE ** pRetOverlayData,RECT *pSrcRc, unsigned long nWidth, unsigned long nHeight, short nType);


	////定位线
	//int CalLocLine(CHsBaseImg *pImgOnMe,geometry::LineCoord&line);


	//像素转换颠倒
	virtual int TransformImg(int nType);

	//锐化---有写函数应该做成通用的，但是如果将其作为本类的对外接口却显得繁琐，于是想插值函数，锐化函数便有了两套
	BYTE** ImgSharp(BYTE** pData,int nRow,int nCol, int v,HWND hwd);

	//锐化时，锐化原始像素还是显示像素
	bool m_bSharpOri;
public:
	//应用dcm文件中名字为sName的Lut或窗宽窗位
	LutData* Hs_ApplyLut(QString sName);

	//获取当前应用的Lut名称
	int Hs_GetCurLutName(QString &sName);

	//调窗,bChangeValue=true意为w和c是变化量.否则w和c是绝对值
	virtual int Hs_WinLevel(long w,long c,bool bChangeValue=false,long *pRetW=NULL,long *pRetC=NULL);

	//绘制m_pDisplayData
	virtual int Hs_DrawImg(HDC dc,const RECT& rc,BYTE *pBits=NULL,unsigned long nRow=0,unsigned long nCol=0);

	//获取当前图像大小.
	SIZE Hs_GetImgSize(bool bDisplaySize=false);

	//当前显示用的m_pDisplayData是m_pOriData被Hs_Size函数加工后的产物,那么m_pDisplayData是用整个m_pOriData加工的吗?还是只用了m_pOriData一部分像素加工的?--这事Hs_Size函数知道
	bool Hs_IsWholeImgSized();

	//释放资源
	int Hs_FreeMem();

	//本图像是否有数据
	bool Hs_IsEmpty();

	//应显示或隐藏OverLay
	int Hs_ApplyOverlay(bool bShow);

	//应用或取消使用斜率截距
	int Hs_ApplySlope(bool bUse);

	//对图像进行插值
	int Hs_Size(RECT *pSrcRc, unsigned long nWidth, unsigned long nHeight, short nType,bool bForce=false);

	//获取某点的像素值取自m_pDisplayData
	double Hs_GetPixelValue(unsigned long r,unsigned long c);

	//复制一个本类的对象
	virtual int Hs_CopyTo(CHsBaseImg &NewHsImg);

	//还原
	virtual int Hs_Restore();

	//旋转
	virtual int Hs_Rotate(int nAngle,bool bSaveFlag = true);

	//上下翻转--这个为何没有SaveFlag参数：因为再次实现它时，此操作会被旋转和镜像代替。
	virtual int Hs_Flip();

	//左右镜像
	virtual int Hs_Mirror(bool bSaveFlag = true);


	//剪切
	virtual int Hs_Cut(long &left,long &top,long &right,long &bottom);


	//对图像的ImgRc区域,制作nBitsRow X nBitsCol大小的pBits---放大镜片--局部放大功能需求
	int Hs_CreateDisplayBits(RECT ImgRc,BYTE* &pRetBits,unsigned long &nBitsRow,unsigned long &nBitsCol);

	//获取ImgPixRc区域内的像素最大值最小值----感兴趣区
	int GetMinMaxValue(const RECT &ImgPixRc,long &nMin,long &nMax,bool bUseSlope);

	//减影(本图 - pImgB)
	int Hs_Subtraction(CHsBaseImg *pImgB);

	//负片
	int Hs_Inverse();

	//填充ImgState
	int Hs_InitImgState();

	//获取最后一次在图像上的插值区域
	RECT Hs_GetSizeRcOnOriImg();

	//补齐像素以当前图像的rcInCurImg所含像素为中心，向四周补像素，补到nNewWidth, nNewHeight这么大
	int Hs_FillPixel(RECT rcInCurImg,int nNewWidth,int nNewHeight);

	////写信息
	//virtual void Hs_DrawInfo(HDC dc,RECT rc,int nInfoType,COLORREF TxtClor,bool bLimitMinFont);

	//获取CT值
	int Hs_GetCtValuePoint(POINT ImgPt,QString &sTxt);

	//获取椭圆范围CT值
	int Hs_GetCtValueEllipse(RECT ImgRc,QString &sTxt);

	//求pt1---pt2线段，经过的点的像素值
	int Hs_GetPixelOnLine(const POINT &pt0,const POINT &pt1,int nPixCount,BYTE* pPixel);

	////存成图片
	//int Hs_SaveAs(char* sFileName,int nType,bool bRuler,bool bPtInfo,bool bAnno);

	//锐化
	int Hs_Sharp(int v,bool bUpdateBits,HWND hwd);

	//CT，MR等图像获取病人位置
	int Hs_GetPatientPos( double x, double y, double z,QString& sLeft, QString& sRight );

private:

	void IsRotateOrHorFlip(char cLft, char cTop, char cRgt, char cBtm,long &nAngle, bool &bFilp);

};
