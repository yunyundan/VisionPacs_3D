#pragma once

class CHsImage;
class Hmy3DVector;
class HmyPlane3D;
class HmyLine3D;
class HmyImageData3D;
class HmyMatrix4x4;

class CHmyMprMaker
{
public:
	CHmyMprMaker();
	~CHmyMprMaker();

private:
	vector<CHsImage*> m_vOriImage;
	void ***m_p3DImgArray;
	vtkSmartPointer<vtkImageData> m_p3DImgData;
	int m_nImgWndType;

	FPOINT m_pStart;
	FPOINT m_pEnd;

	HmyLine3D *m_pLine1;
	HmyLine3D *m_pLine2;
	HmyImageData3D *m_pHmyImageData;

	HmyMatrix4x4 *m_pCutMatrix;
	HmyPlane3D *m_pCutPlane;

public:
	int m_nSlabMode;
	int m_nSlabNumberOfSlices;
	
	void InitParaAndData(vector<CHsImage*> vImg, void ***pImgArray, vtkSmartPointer<vtkImageData> p3Ddata, int nWndType, HmyImageData3D *pHmyImgData);
	
	void CalculateCutPlane();
	void CalculateCutImageInfo(CHsImage *pCutImg);
	bool IsOrthogonalMatrix(HmyMatrix4x4 *matrix);

	void GetCutImage(CHsImage *pCutImg, HmyLine3D *pLine1, HmyLine3D *pLine2, int nSliceNum);

	void GetOrthogonalImage(CHsImage *pCutImage);
	//nOritation: 0 normal方向为z; 1 normal方向为y; 2 normal方向为x;
	void CalculateOrthogonalImage(CHsImage *pShowImg, int nIndex, int nSliceNum, int nOritation);


	bool IsInterLineInRect(int zIndex, Hmy3DVector origin, Hmy3DVector direction, HmyLine3D &interSetLine);
	//bool GetIntersetLineImage(int zIndex, HmyLine3D intersetLine);

	void GetStartAndEndSlice(int iIndex, int &nSliceNum,int &nStartS, int &nEndS);
	void DoInterpolationToImage(CHsImage *pImg, double dSpacingX, double dSpacingY,int nRows,int nCols);

	void CalMprMode(BYTE **pRetData, BYTE **pAllRows, int r,int nCols, int nSliceNum);
	void CalMprMode(short **pRetData, short **pAllRows, int r, int nCols, int nSliceNum);

	//计算产生图像的方位信息
	void CalImagePositon(CHsImage *pImg);
};

