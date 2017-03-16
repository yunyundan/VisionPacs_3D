#pragma once

class CHsImage;

class CHsNormalMprMaker
{
public:
	CHsNormalMprMaker();
	~CHsNormalMprMaker();

private:
	vector<CHsImage*> m_vOriImage;
	void ***m_p3DImgArray;
	vtkSmartPointer<vtkImageData> m_p3DImgData;
	int m_nImgWndType;
	int m_nOriImgType;

public:
	int m_nMprMode;//0:MIP; 1£ºMAP

	void InitParaAndData(vector<CHsImage*> vImg, void ***pImgArray, vtkSmartPointer<vtkImageData> p3Ddata, int nWndType, int nOriType);
	void GetShowImage(CHsImage *pShowImg, int iIndex, int iSliceNum);
	void GetStartAndEndSlice(int iIndex, int &iSliceNum,int &iStartS, int &iEndS);
	void DoInterpolationToImage(CHsImage *pImg, double dSpacingX, double dSpacingY,int nRows,int nCols);
	void CalImagePositon(CHsImage *pImg,int nIndex);
};

