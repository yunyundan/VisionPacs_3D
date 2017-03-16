#pragma once

typedef QMap<unsigned long, QString > MAP_TAGNAME;

//image process return code
#define Ret_FileExist				-8	//添加文件AddFile时,此文件已添加过了
#define Ret_UnKnownCase				-7
#define Ret_FindOddLen				-6	//遇到值长度为奇数的情况,且修正成功
#define Ret_NoValue					-5
#define Ret_ReachPixelData			-4
#define Ret_ReachBufferEnd			-3	//当前Buffer不够,发生了某tag被切断的事情,需要再去读一截buffer
#define Ret_ReachFileEnd			-2
#define Ret_ReachParentEleEnd		-1

//Error Code define
#define Ret_Success					0 

#define Ret_InvalidPara				1 
#define Ret_LoadFileError			2
#define Ret_GetTagPosError			3
#define Ret_InvalidDicomFile		4
#define Ret_TagNotFound				5
#define Ret_NoPixelFound			6
#define Ret_ErrorInSaveAs			7
#define Ret_InvalidDicomVR			8
#define Ret_GetElementError			9
#define Ret_NoTransferSyntaxes		10
#define Ret_GetValueFailed			11
#define Ret_OutOfValueCount			12
#define Ret_InvalidBuf				13
#define Ret_GetImgError				14
#define Ret_AllocateMemFailed		15
#define Ret_GetLutDataError			16
#define Ret_LedtolsGetImgError		17
#define Ret_EmptyImage				18
#define Ret_FileNotFound			19
#define Ret_GetPixValueFailed		20
#define Ret_InvalidPixFormat		21
#define Ret_UnSupportPara			22//合法,但是还没完成的参数
#define Ret_InvalidImgForSubtract	23//减影之非法图像

#define Ret_CreateFileFail	        24	
#define Ret_InvalidElement			25
#define Ret_RepeatElement			26
#define Ret_NoObjectSelected		27
#define Ret_InvalidImgInfo			28
#define Ret_CreateAviErr			29


#define Ret_InvalidFuncID		101
#define Ret_InvalidScreenID		102
#define Ret_NoScreenWnd			103

#define Ret_AddPrintImgErr		104
#define Ret_InsertPrintGpErr	105
#define Ret_CreateLBitmapErr	106
#define Ret_CreateMemDcErr		107
#define Ret_CreateBitmapErr		108
#define Ret_SelectObjectErr		109
#define Ret_CreateAtlImageErr	110
#define Ret_GetAtlImageDcErr	111

#define Ret_AbstractUnsupported	300
#define Ret_AssociateRejected	301
#define Ret_PrintingNow			302
#define Ret_GetImgBoxUidEmpty	303

typedef struct _DcmTag
{
	unsigned long  nCode;     // Tag Code
	unsigned long  nMask;     // Mask (for multiple-elements specifies the same entry in the table)
	char		   pszName[100];  // Name
	unsigned long  nVR;       // Value Representation
	unsigned long  nMinVM;    // Minimum Value Multiplicity
	unsigned long   nMaxVM;    // Maximum Value Multiplicity
	unsigned long  nDivideVM; // Value that should divide the Value Multiplicity

	_DcmTag()
	{
		nCode = 0;
		nMask = 0;
		strcpy_s(pszName, 100, "UnKnown");
		nVR = 0;
		nMinVM = 0;
		nMaxVM = 0;
		nDivideVM = 0;
	}

} DcmTag, *pDcmTag;

typedef struct _DcmVR
{
	unsigned long  nCode;		// Code (VR_AE, VR_AS, ...)
	char pszName[256];			// Name ("Application Entity", "Age String", ...)
	char pszShortName[10];		//Name (VR_OF,VR_UI)

	unsigned long  nValueLeng;	// Data Value  Length
	unsigned long  nRestrict;	// Restriction applied to the length
	unsigned long  nUnitSize;	// The size for the smallest item

	unsigned long  nLenOfLenDes;//"值长度描述"本身占用长度,这个描述的长度.一个Tag里有一部分是说明后面值的长度的,这个说明的长度也不是固定的
	unsigned long nOffset;		//一个Tag的值的起始位置(相对于Tag Number而言,也就是一个Tag前面三部分的总长度).某些类型而言有时候这个值是固定的.
	int nVrType;				//不知道,但很重要.待查
	unsigned long nLenOfVrDes;	//VR域本身长度

} DcmVR, *pDcmVR;

typedef struct _HsElement
{
	_HsElement* pParentEle;			//爹
	_HsElement* pPreEle;			//兄
	_HsElement* pNextEle;			//弟
	QVector<_HsElement*> pChildEleV;	//子

	unsigned long	nTag;	    	// Tag
	unsigned long	nVR;			// 类型
	unsigned long	nLenDesc;		// 值长度的描述占多少字节

	unsigned long	nValueCount;	//一个数据元素中包含的数据个数，值的多样性
	unsigned long	nLen;			//数据占用字节数

	unsigned long   nTagPos;		//一个数据元素的起始位置,也就是其TagNumber域的第一个字节
	unsigned long   nOffset;		//TagNumber第一个字节向后多少字节为值域第一个字节(值域在元素内部的位置)

	bool			bBigEndian;		//这个Tag是否是大端语法.

	bool			bVirtualItem;	//有的SQ下只有一个Item,于是就不写了,所以这个Item我要虚拟一个

	bool			bNewTag;       //是否为修改过的或新建的TAG

	BYTE			*pValue;		//若为修改过的或新建的TAG这便是它的值

	bool			bSquence;		//是否为Squence;

	_HsElement()//初始化
	{
		Reset();
	}

	void Reset()
	{
		pParentEle = NULL;
		pPreEle = NULL;
		pNextEle = NULL;

		nTag = 0;
		nVR = 0;
		nLenDesc = 0;

		nValueCount = 0;
		nLen = 0;

		nOffset = 0;
		nTagPos = 0;
		bBigEndian = false;

		bVirtualItem = false;

		bNewTag = false;

		pValue = NULL;

		bSquence = false;

	}

	_HsElement &operator=(const _HsElement &info)
	{
		pParentEle = NULL;			//爹
		pPreEle = NULL;			    //兄
		pNextEle = NULL;			//弟
		//vector<_HsElement*> pChildEleV;	//子

		nTag = info.nTag;
		nVR = info.nVR;
		nLenDesc = info.nLenDesc;

		nValueCount = info.nValueCount;
		nLen = info.nLen;

		nTagPos = info.nTagPos;
		nOffset = info.nOffset;
		bBigEndian = info.bBigEndian;

		bVirtualItem = info.bVirtualItem;

		bNewTag = info.bNewTag;

		if (info.pValue && info.nLen)
		{
			pValue = new BYTE[nLen];
			memcpy(pValue, info.pValue, nLen);
		}
		else
		{
			pValue = NULL;
		}


		bSquence = info.bSquence;

		return *this;
	}

	~_HsElement()
	{
		size_t nChild = pChildEleV.size();

		for (size_t i = 0; i<nChild; i++)
			delete pChildEleV[i];

		pChildEleV.clear();

		if (pValue)
			delete[]pValue;

		pValue = NULL;
	}

}HsElement, *pHsElement;

typedef struct _HsBaseFileInfo
{
	int					nTsType;		//传输语法Transfer Syntrax
	unsigned long		nPixPos;		//像素元素开始的位置
	unsigned long		nStartPos;		//标签搜索的起始位置
	unsigned long       nEndPos;		//标签搜索的结束位置
	unsigned long		nFullSize;		//文件总字节长度
	unsigned long		nCurStart;		//当文件长度大于32000000时，要考虑分段读取文件，此为当前读取段的起点
	unsigned long		nCurEnd;		//此为当前读取段的终点
	int					nSpecialTs;		// 因传输语法导致错误，进行校正

	QString             sFileName;		// 文件路径名 
	long                nBitsAllocated;	//像素分配位
	int					nPixelTagNumber;// 一个文件中，有几个pixel-data的标签
	int					nDataType;		//数据类型，1为普通，2为Palette Color, 3：RGB	

	unsigned long		nReadLen;		// 读取段的长度
	long				nLoadLen;		// 第一次读取的长度
	long				nOffsetWidth;	// 窗宽的预设偏移量
	long				hnOffsetCenter;	// 窗位的预设偏移量

	HsElement			pElement;		//获得当前图像的element

	int					nJPEG;			// 是否解压缩图片
	int					nNewlength;		// 解压后数据部分的字节长度
	unsigned char *     pJPEGbuf;		// 解压数据流的指针

	int					nSpecial;		// 为特殊需要设定，1为三门峡的dsa，图像tag值表述不对，文件中有几个图像计算不对。

	unsigned long		nCurPos;		//当前分析Buffer分析道何处了

	bool				bCpuBigEndia;	//CPU大端吗?

	int					nFrame;//	有图像的话是几帧

	_HsBaseFileInfo()//初始化
	{
		Reset();
	}

	void Reset()
	{
		nTsType = -1;
		nPixPos = 0;
		nStartPos = 0;
		nEndPos = 0;
		nFullSize = 0;
		nCurStart = 0;
		nCurEnd = 0;
		nSpecialTs = 0;

		sFileName = "";
		nBitsAllocated = 0;
		nPixelTagNumber = 0;
		nDataType = 0;

		nReadLen = 0;
		nLoadLen = 0;
		nOffsetWidth = 0;
		hnOffsetCenter = 0;

		pElement.Reset();

		nJPEG = 0;
		nNewlength = 0;
		pJPEGbuf = 0;
		nSpecial = 0;

		nCurPos = 0;
		bCpuBigEndia = false;
		nFrame = 0;

	}

}HsBaseFileInfo; //比较重要的常量


typedef struct _HSPOINT
{
	double x;
	double y;

	bool operator == (const _HSPOINT& other) const
	{
		if (fabs(x - other.x) < 0.000001 && fabs(y - other.y) < 0.000001)
			return true;

		return false;
	}

	bool operator != (const _HSPOINT& other) const
	{
		if (fabs(x - other.x) < 0.000001 && fabs(y - other.y) < 0.000001)
			return false;

		return true;
	}

}HSPOINT;

//做定位线需要的材料数据都在这里
typedef struct _ImageLoc
{
	//是否有效.
	bool bValide;

	//From Tag:0x00280010
	long nRow;		//图像高度(单位像素)
	double fRowmm;	//图像高度(单位mm)

	//From Tag:0x00280011
	long nCol;		//图像宽度(单位像素)
	double fColmm;	//图像宽度(单位mm)

	////From Tag:0x00280030
	double fPixSpacingX;//横向像素间距
	double fPixSpacingY;//纵向像素间距

	//关于定位线坐标:以人视角为基准.从脚向头=Z轴正向,从右向左=X轴正向,从胸前向背后=Y轴正向
	//From Tag:0x00200032 TAG_IMAGE_POSITION_PATIENT
	double fOriLeftTopPixX;//图像左上角第一个像素点,在X轴的坐标
	double fOriLeftTopPixY;//图像左上角第一个像素点,在Y轴的坐标
	double fOriLeftTopPixZ;//图像左上角第一个像素点,在Z轴的坐标

	//From Tag:0x00200037 TAG_IMAGE_ORIENTATION_PATIENT
	double fOriFirstRowCosX;//第一行像素与X轴的cos值
	double fOriFirstRowCosY;//第一行像素与Y轴的cos值
	double fOriFirstRowCosZ;//第一行像素与Z轴的cos值

	double fOriFirstColCosX;//第一列像素与X轴的cos值
	double fOriFirstColCosY;//第一列像素与Y轴的cos值
	double fOriFirstColCosZ;//第一列像素与Z轴的cos值

	//
	double fLeftTopPixX;//图像左上角第一个像素点,在X轴的坐标
	double fLeftTopPixY;//图像左上角第一个像素点,在Y轴的坐标
	double fLeftTopPixZ;//图像左上角第一个像素点,在Z轴的坐标

	//From Tag:0x00200037 TAG_IMAGE_ORIENTATION_PATIENT
	double fFirstRowCosX;//第一行像素与X轴的cos值
	double fFirstRowCosY;//第一行像素与Y轴的cos值
	double fFirstRowCosZ;//第一行像素与Z轴的cos值

	double fFirstColCosX;//第一列像素与X轴的cos值
	double fFirstColCosY;//第一列像素与Y轴的cos值
	double fFirstColCosZ;//第一列像素与Z轴的cos值

	//From Tag:0x00180050 TAG_SLICE_THICKNESS
	double fSliceThickness;//层厚

	//From Tag:0x00201041 TAG_SLICE_LOCATION
	double fSliceLoction;//层位置

	_ImageLoc()
	{
		bValide = false;
		nRow = 0;
		nCol = 0;
		fPixSpacingX = 0.00;
		fPixSpacingY = 0.00;

		fOriLeftTopPixX = 0.00;
		fOriLeftTopPixY = 0.00;
		fOriLeftTopPixZ = 0.00;

		fOriFirstRowCosX = 0.00;
		fOriFirstRowCosY = 0.00;
		fOriFirstRowCosZ = 0.00;

		fOriFirstColCosX = 0.00;
		fOriFirstColCosY = 0.00;
		fOriFirstColCosZ = 0.00;

		fLeftTopPixX = 0.00;
		fLeftTopPixY = 0.00;
		fLeftTopPixZ = 0.00;

		//From Tag:0x00200037 TAG_IMAGE_ORIENTATION_PATIENT
		fFirstRowCosX = 0.00;
		fFirstRowCosY = 0.00;
		fFirstRowCosZ = 0.00;

		fFirstColCosX = 0.00;
		fFirstColCosY = 0.00;
		fFirstColCosZ = 0.00;

		fSliceThickness = 0.00;
		fSliceLoction = 0.00;
	}

	_ImageLoc & operator = (const _ImageLoc &info)//运算符重载
	{
		this->bValide = info.bValide;

		this->nRow = info.nRow;
		this->fRowmm = info.fRowmm;

		this->nCol = info.nCol;
		this->fColmm = info.fColmm;

		this->fPixSpacingX = info.fPixSpacingX;
		this->fPixSpacingY = info.fPixSpacingY;

		this->fOriLeftTopPixX = info.fOriLeftTopPixX;
		this->fOriLeftTopPixY = info.fOriLeftTopPixY;
		this->fOriLeftTopPixZ = info.fOriLeftTopPixZ;

		this->fOriFirstRowCosX = info.fOriFirstRowCosX;
		this->fOriFirstRowCosY = info.fOriFirstRowCosY;
		this->fOriFirstRowCosZ = info.fOriFirstRowCosZ;

		this->fOriFirstColCosX = info.fOriFirstColCosX;
		this->fOriFirstColCosY = info.fOriFirstColCosY;
		this->fOriFirstColCosZ = info.fOriFirstColCosZ;

		this->fLeftTopPixX = info.fLeftTopPixX;
		this->fLeftTopPixY = info.fLeftTopPixY;
		this->fLeftTopPixZ = info.fLeftTopPixZ;

		this->fFirstRowCosX = info.fFirstRowCosX;
		this->fFirstRowCosY = info.fFirstRowCosY;
		this->fFirstRowCosZ = info.fFirstRowCosZ;

		this->fFirstColCosX = info.fFirstColCosX;
		this->fFirstColCosY = info.fFirstColCosY;
		this->fFirstColCosZ = info.fFirstColCosZ;

		this->fSliceThickness = info.fSliceThickness;
		this->fSliceLoction = info.fSliceLoction;

		return *this;
	}

}ImageLoc;

typedef struct _ImageInfo
{
	QString		 sFileName;							//所属文件名

	double       fWinCenter;						//窗位
	double       fWinWidth;							//窗宽

	double       fRescaleSlope;						//重新调整的斜率
	double       fRescaleIntercept;					//重新调整的截距

	long		 nLutDescriptor1;					//TAG_LUT_DESCRIPTOR
	long		 nLutDescriptor2;					//TAG_LUT_DESCRIPTOR
	long	     nUltrasound_Color_Data_Present;	//超声中是否有彩色	
	long		 nWcLutLen;							//窗宽窗位Lut的长度
	long		 iLutStart;							//我的Lut数组有负下标.此处记下第一个下标
	long		 iLutEnd;							//iLutStart + nWcLutLen;

	int			 nOverLayType;						//0:无,1：独立,2：嵌入
	long         nOverlayBitsAllocated;             // 覆盖图位分配
	long         iOverlayBitPosition;               // 覆盖图位位置
	long         nOverlayRows;                      // 覆盖图行
	long         nOverlayCols;						// 覆盖图列
	QString		 sOverlayType;						// 覆盖图类型
	long		 nOverlayOrigin1;					// 覆盖图原点1
	long		 nOverlayOrigin2;					// 覆盖图原点2
	QString		 sOverlayMagnificationType;			// 放大类型
	QString		 sOverlaySmoothingType;				// 平滑类型
	QString		 sOverlayOrImageMagnification;		// 放大率
	long		 nMagnifyToNumberOfColumns;			// 列数量放大
	QString		 sModality;
	pHsElement	 pEle;
	int          iFrame;							//当前图像的ID(第几帧)
	QString		 sFrameLabelVector;					//帧标签向量，值的数量应该等于帧的数量,Lh没用过
	double		 fImagePosition[3];					// 图片位置第三个参数lh没用过
	//////////////////////////////////////////////////////////////////////////
	int iCompress;			//0:无压缩.3-9.代表传输语法中提到的压缩方法
	long nSamplePerPixel;	//一个像素分成几段(段:Sample或称作样本)表示?
	long nBitsAllocated;	//每段分配多少了位?
	long nBitStored;		//每段实际使用多少位?
	long iHighBit;			//每像素段中最高位是第几位?
	long nBitsPerPixel;		//每个像素总共占多少位? (nSamplePerPixel*nBitsAllocated)
	long iPlanarConfig;		//决定RGB图像是rgb,还是rrrgggbbb的Tag(一般为0:rgb,若为1则是r1r2r3g1g2g3b1b2b3) TAG_PLANAR_CONFIGURATION

	QString sPhotometric;	//"MONOCHROME1","MONOCHROME2","PALETTE COLOR","RGB","YBR_FULL_422","YBR FULL","FFFFFFFF","-1"

	long nRows;				//像素行数(高度),这个值在旋转时是会被改变的
	long nCols;				//像素列数(宽度),这个值在旋转时是会被改变的

	long nOriRows;				//像素行数(高度),这个值永远不会被改变,只负责记录最初的高
	long nOriCols;				//像素列数(宽度),这个值永远不会被改变,只负责记录最初的宽

	double fPixelSpaceX;	//横向像素间距(单位英寸)
	double fPixelSpaceY;	//纵向像素间距(单位英寸)

	QString sImageType;		//图像类型
	long nPixelRepresentation;//Pixel_Representation，是否高位补码

	long nSmallestPixelValue;//像素最小值
	long nLargestPixelValue; //像素最大值

	long nFrame;			//帧数

	//推断的结果
	bool bInverse;			 //if(sPhotometric.find("MONOCHROME1") )>=0)	bInverse = true;
	bool bGrayImg;			//MONOCHROME1 MONOCHROME2=Gray

	double fWinLevelStep;	//wc的放大率.有的图像LutLen很大,调节效果不明显.用此值放大

	ImageLoc ImgLocPara;	//定位线相关材料

	bool bBigEndia;//大端吗？

	bool bValid;			//信息是否有效


	//HMY
	long iAcquisitionNum;  //一次序列中的获得序号，多用于分辨多期
	QString sScanOptions;	   //扫描方式，辅助辨别多期

	double fDifusionBvalue;//弥散成像中的B值


	_ImageInfo()
	{
		Reset();
		memset(&ImgLocPara, 0, sizeof(ImageLoc));
	}

	void Reset()
	{
		sFileName = "";
		fWinCenter = 0.0;
		fWinWidth = 0.0;
		fRescaleSlope = 1.00;
		fRescaleIntercept = 0.00;
		nLutDescriptor1 = 0;
		nLutDescriptor2 = 0;
		nUltrasound_Color_Data_Present = -1;
		nWcLutLen = 0;
		iLutStart = 0;
		iLutEnd = 0;

		nOverlayBitsAllocated = 0;        // 覆盖图位分配
		iOverlayBitPosition = 0;        // 覆盖图位位置
		nOverlayRows = 0;        // 覆盖图行
		nOverlayCols = 0;        // 覆盖图列
		sOverlayType = "";
		nOverlayOrigin1 = 0;
		nOverlayOrigin2 = 0;
		sOverlayMagnificationType = "";		// 放大类型
		sOverlaySmoothingType = "";		// 平滑类型
		sOverlayOrImageMagnification = "";		// 放大率
		nMagnifyToNumberOfColumns = 0;		// 列数量放大
		sModality = "";
		pEle = NULL;
		iFrame = -1;
		fImagePosition[0] = -10000.00;
		fImagePosition[1] = -10000.00;
		fImagePosition[2] = -10000.00;
		sFrameLabelVector = "";

		iCompress = 0;
		nSamplePerPixel = 0;
		nBitsAllocated = 0;
		nBitStored = 0;
		iHighBit = 0;
		nBitsPerPixel = 0;
		iPlanarConfig = 0;
		sPhotometric = "";
		nRows = 0;
		nCols = 0;
		nOriRows = 0;
		nOriCols = 0;
		fPixelSpaceX = 0.00;
		fPixelSpaceY = 0.00;
		sImageType = "";
		nPixelRepresentation = 0;
		nSmallestPixelValue = 0;
		nLargestPixelValue = 0;
		bInverse = false;
		nFrame = 0;
		bGrayImg = true;

		fWinLevelStep = 1.00;
		bBigEndia = false;
		bValid = false;

		iAcquisitionNum = 0;
		sScanOptions = "";

		fDifusionBvalue = 0.00;
	}

}ImageInfo;

typedef struct _ImgState
{//经过处理的图像再次从文件中载入数据时需要显示之前处理过的样子,此处为影响图像效果的参数

	bool bImgStateFilled;	//m_ImgState被赋过值了

    RECT *pOriRect;//当前的原始数据,在最初的原始数据中的位置.---经过剪切后,这个值负责记录
	long nCurOriPixRow;//m_pOriData经过剪切后当前剩余尺寸
	long nCurOriPixCol;//m_pOriData经过剪切后当前剩余尺寸
    POINT CurWc;	//当前窗宽窗位
	unsigned long nCurBitsRow;//需要显示的图像行数
	unsigned long nCurBitsCol;//需要显示的图像列数
	bool bShowOverLay;
	bool bUseSlope;

	//当前显示用得到m_pDisplayData的行列数
	unsigned long nDispalyRow;
	unsigned long nDispalyCol;

	//位置效果
	HSPOINT fCenterPt;//这代表两个比例:x=屏幕中心点到图像显示rc的left的距离/图像显示宽度.y=屏幕中心点到图像显示rc的top/图像显示高度,x=-100表示靠左侧显示，100表示靠右侧显示

	//放大效果
	double fZoomX;//如果图像居中充满显示的CenterRc,实际显示时占用得到DisplayRc,这俩值的比例便是此值DisplayRc宽/CenterRc宽,高度不计了,与宽度的比例一样,没意义了

	bool bWholeImgSized;//当前显示用的m_pDisplayData是m_pOriData被Hs_Size函数加工的产物,那么m_pDisplayData是用整个m_pOriData加工的吗?

	bool bToSubstract;//是否需要减影
	bool bSubstracted;//是否减过影

	bool bInversed;	//是否负片

	QString sLeftPatientPos;//图像左侧是病人的什么位置(AH Hp......),会根据图像的翻转而变化
	QString sTopPatientPos;
	QString sRightPatientPos;
	QString sBottomPatientPos;

	long nAngleRotated;//旋转了多少度？
	bool bMirror;//是否左右镜像了

	char cLft;//当前的Left是原始图像的哪个边？取值:LTRB
	char cTop;//当前的Top是原始图像的哪个边？取值:LTRB
	char cRgt;//当前的Rgiht是原始图像的哪个边？取值:LTRB
	char cBtm;//当前的Btm是原始图像的哪个边？取值:LTRB

	QString sCurLutName;//当前应用的Lut名称，Reload时，要重现该效果的

	bool IsUserSubstractWc;//--减影时用户调的wc吗？
	QVector<int> nSharpValueV;//锐化次数及每次的锐化度

	_ImgState()
	{
		pOriRect = NULL;
		Reset();
	}

	_ImgState &operator = (const _ImgState &info)
	{
		this->bImgStateFilled = info.bImgStateFilled;

		this->nCurOriPixRow = info.nCurOriPixRow;
		this->nCurOriPixCol = info.nCurOriPixCol;

		if (info.pOriRect)
		{
            this->pOriRect = new RECT;
			*pOriRect = *(info.pOriRect);
		}
		else
		{
			if (this->pOriRect)
				delete this->pOriRect;

			this->pOriRect = NULL;
		}

		this->CurWc = info.CurWc;
		this->nCurBitsRow = info.nCurBitsRow;
		this->nCurBitsCol = info.nCurBitsCol;
		this->nDispalyRow = info.nDispalyRow;
		this->nDispalyCol = info.nDispalyCol;
		this->bShowOverLay = info.bShowOverLay;
		this->bUseSlope = info.bUseSlope;

		this->fCenterPt = info.fCenterPt;

		this->fZoomX = info.fZoomX;
		this->bWholeImgSized = info.bWholeImgSized;
		this->bToSubstract = info.bToSubstract;
		this->bSubstracted = info.bSubstracted;
		this->bInversed = info.bInversed;

		this->sLeftPatientPos = info.sLeftPatientPos;
		this->sTopPatientPos = info.sTopPatientPos;
		this->sRightPatientPos = info.sRightPatientPos;
		this->sBottomPatientPos = info.sBottomPatientPos;

		this->sCurLutName = info.sCurLutName;
		this->nAngleRotated = info.nAngleRotated;
		this->bMirror = info.bMirror;

		this->cBtm = info.cBtm;
		this->cLft = info.cLft;
		this->cRgt = info.cRgt;
		this->cTop = info.cTop;

		this->IsUserSubstractWc = info.IsUserSubstractWc;
		this->nSharpValueV = info.nSharpValueV;

		return *this;
	}

	void Reset()
	{
		if (pOriRect)
			delete pOriRect;
		pOriRect = NULL;

		bImgStateFilled = false;

        CurWc.x=0;
        CurWc.y=0;
		nCurBitsRow = nCurBitsCol = 0;
		bShowOverLay = bUseSlope = true;
		nDispalyRow = nDispalyCol = 0;

		fCenterPt.x = 0.5;
		fCenterPt.y = 0.5;
		fZoomX = 1.00;

		nCurOriPixRow = 0;
		nCurOriPixCol = 0;

		bWholeImgSized = true;
		bToSubstract = false;
		bSubstracted = false;
		bInversed = false;

		sLeftPatientPos = "";
		sTopPatientPos = "";
		sRightPatientPos = "";
		sBottomPatientPos = "";

		sCurLutName = "";
		bMirror = false;
		nAngleRotated = 0;

		cLft = 'L';
		cTop = 'T';
		cRgt = 'R';
		cBtm = 'B';

		IsUserSubstractWc = false;
		nSharpValueV.clear();

	}


	~_ImgState()
	{
		if (pOriRect)
			delete pOriRect;
		pOriRect = NULL;
	}

}ImgState;


typedef struct _HsDateTime
{
	unsigned long nYear; /* year */
	unsigned long nMonth; /* month */
	unsigned long nDay; /* day */
	unsigned long nHours; /* hours */
	unsigned long nMinutes; /* minutes */
	unsigned long nSeconds; /* seconds */
	unsigned long nFractions; /* fraction of a second */
	long nOffset; /* suffix */
} HsDateTime;

typedef struct _LutData
{
	bool	bModality;		//是Modality的还是VOI的?Modality有强制性而且Modality Lut下不会有窗宽窗位
	QString	sName;			//名称
	QString sLutType;		//C.11.1.1.2 (OD、HU、US)

	bool	bWc;			//是窗宽窗位吗?

	//若是窗宽窗位,
	double	nW;				//这就是窗宽
	double	nC;				//这就是窗位

	//否则这就是LutData
	long	nLutLen;		//LutData长度(个数)
	long	nMinValue;		//最小值
	long	nMaxValue;		//最大值
	long	nBitsPerData;	//每个LutData占几位?

	BYTE*	pLutData;		//LutData
	unsigned short nBytePerData;//每个Data占几字节?

	bool	bUse;			//是否启用

	int iLutID;//有时候Lut没有名称，那就依据他在这个文件内的Lut顺序起个名字

	bool bFromPr;//这个Lut是否来自PR文件

	void Reset()
	{
		bModality = false;
		sName = "";
		sLutType = "";
		bWc = false;
		nW = 0.00;
		nC = 0.00;

		nLutLen = 0;
		nMinValue = 0;
		nMaxValue = 0;
		nBitsPerData = 0;
		pLutData = NULL;
		nBytePerData = 0;
		bUse = false;
		iLutID = 0;
		bFromPr = false;
		if (pLutData)
		{
			delete[]pLutData;
			pLutData = NULL;
		}
	}
	_LutData()
	{
		bModality = false;
		bWc = false;
		nW = 0.00;
		nC = 0.00;
		sName = "";
		sLutType = "";

		nLutLen = 0;
		nMinValue = 0;
		nMaxValue = 0;
		nBitsPerData = 0;
		pLutData = NULL;
		nBytePerData = 0;
		bUse = false;
		iLutID = 0;
		bFromPr = false;
	}

	_LutData &operator = (_LutData const &info)
	{
		if (info.pLutData)
		{
			long nByte = info.nLutLen*info.nBytePerData;
			this->pLutData = new BYTE[nByte];
			memcpy(this->pLutData, info.pLutData, nByte);
		}

		this->bModality = info.bModality;
		this->bWc = info.bWc;
		this->nW = info.nW;
		this->nC = info.nC;
		this->sName = info.sName;
		this->sLutType = info.sLutType;

		this->nLutLen = info.nLutLen;
		this->nMinValue = info.nMinValue;
		this->nMaxValue = info.nMaxValue;
		this->nBitsPerData = info.nBitsPerData;
		this->nBytePerData = info.nBytePerData;
		this->bUse = info.bUse;
		this->iLutID = info.iLutID;
		this->bFromPr = info.bFromPr;

		return *this;
	}

	~_LutData()
	{
		if (pLutData)
		{
			delete[]pLutData;
			pLutData = NULL;
		}
	}
}LutData;

typedef QMap<unsigned long, DcmTag> MAP_TAGPROPERTY;
typedef QMap<unsigned long, DcmVR> MAP_VRPROPERTY;
typedef QMap<QString, int> MAP_DCMFILE;

typedef enum ENUM_HS_TRANSFER_SYNTAXES
{
	TS_IMPLICIT_VR_LITTLE_ENDIAN,//1.2.840.10008.1.2
	TS_EXPLICIT_VR_LITTLE_ENDIAN,//1.2.840.10008.1.2.1
	TS_EXPLICIT_VR_BIG_ENDIAN,//1.2.840.10008.1.2.2
	TS_RLE_LOSSLESS,//1.2.840.10008.1.2.5
	TS_JPEG_BASELINE_1,//1.2.840.10008.1.2.4.50
	TS_JPEG_EXTENDED_2_4,//1.2.840.10008.1.2.4.51
	TS_JPEG_LOSSLESS_NONHIER_14,//1.2.840.10008.1.2.4.57
	TS_JPEG_LOSSLESS_NONHIER_14B,//1.2.840.10008.1.2.4.70
	TS_JPEG2000_LOSSLESS_ONLY,//1.2.840.10008.1.2.4.90
	TS_JPEG2000                   //1.2.840.10008.1.2.4.91  
}HS_TRANSFER_SYNTAXES;

typedef struct _MYDATA24
{
	BYTE pData[3];

	_MYDATA24()
	{
		memset(pData, 0, 3);
	}

	_MYDATA24 &operator = (_MYDATA24 const &info)
	{
		memcpy(this->pData, info.pData, 3);

		return *this;
	}
}MYDATA24;

#define OverLay_None	0	//没overlay
#define OverLay_Bits	1	//所有数据转成二进制后,每一位代表一个overlay数据的有无.
#define OverLay_Pixel	2	//嵌入到像素空白位的Overlay
#define OverLay_Byte	3	//一个字节代表一个overlay--还没遇到

#define OverlayValue	9


//------------------------插值方式---------------------------------------------
#define HSIZE_NONE		0//无插值
#define HSIZE_NORMAL	1//最近邻域法
#define HSIZE_RESAMPLE  2//双线性插值法
#define HSIZE_BICUBIC	3//三次卷积

#define Len_AfterPixelData	300 //大部分文件主像素后就结束了,而有的文件还有其它数据,这便是主像素后其余数据的最大值

typedef struct _HSRECT
{
	double left;
	double top;
	double right;
	double bottom;
}HSRECT;

//Img左右键交互
typedef enum Enum_ImgInteractionStyleIds {
	LOCTION_INTERACTION,
	WINDOW_LEVEL_INTERACTION,
	PAN_INTERACTION,
	ZOOM_INTERACTION,
	BROWSER_INTERACTIOM
}ImgInteractionStyleIds;

//交互标记
#define IMGSTSTEM_LOCTION			   0
#define IMGSTSTEM_PICK				   5
#define IMGSTSTEM_PAN				   1
#define IMGSTSTEM_ZOOM				   2
#define IMGSTSTEM_WL				   3
#define IMGSTSTEM_BROWSER			   4


//MPR 图像方位
#define  ORIIMG_AXIAL					1
#define  ORIIMG_CORONAL					2
#define  ORIIMG_SAGITTAL				3

//#define  ORIIMG_AXIAL_R					0		//横断位，X方向是由右到左
//#define  ORIIMG_AXIAL_L					1		//横断位，X方向是由左到右
//#define  ORIIMG_CORONAL_R				2		//冠状位，X方向是由右到左
//#define  ORIIMG_CORONAL_R				3		//冠状位，X方向是由左到右
//#define  ORIIMG_SAGGITAL_A				4		//矢状位，X方向是由前到后
//#define  ORIIMG_SAGGITAL_P				4		//矢状位，X方向是由后到前


typedef struct _INFOITEM
{
	QString sItemName;//项目名称	(1:0x.....为DicomTag,2:[Txt]为常量,3:{字段名}为数据字段)
	QString sTag;//Tag
	QString sFormat;//“%s%d”之类的格式化字符串
	int iRow;//在第几行
	int iOrder;//行中顺序号

	QString sValue;//填充值之后
	_INFOITEM()
	{
		sItemName = "";
		sTag = "";
		sFormat = "";
		iRow = 1;
		iOrder = 1;
		sValue = "";
	}
}INFOITEM;

typedef struct _CORNORINFO
{
	QString sPos;					//位置号
	vector <INFOITEM> infoV;

	_CORNORINFO  &operator = (_CORNORINFO const &info)
	{
		sPos = info.sPos;

		this->infoV = info.infoV;

		return *this;
	}

}CORNORINFO;

typedef struct _MODINFO
{
	QString sModality;

	QString sFaceName;	//字体名称
	int nSize;		//字体大小
	QColor clor;	//字体颜色;

	vector<CORNORINFO> coInfoV;

	_MODINFO()
	{
		sModality = "";
		sFaceName = "黑体";
		nSize = 10;

		clor = qRgb(255, 255, 255);
	}

	void AddCornerInfo(QString sPos, INFOITEM InfoItem)
	{
		int n = int(coInfoV.size());
		for (int i = 0; i < n; i++)
		{
			if (coInfoV[i].sPos == sPos)
			{
				coInfoV[i].infoV.push_back(InfoItem);
				return;
			}
		}

		CORNORINFO coInfo;
		coInfo.sPos = sPos;
		coInfo.infoV.push_back(InfoItem);

		coInfoV.push_back(coInfo);
	}

	_MODINFO  &operator = (_MODINFO const &info)
	{
		sModality = info.sModality;

		sFaceName = info.sFaceName;
		nSize = info.nSize;
		clor = info.clor;

		this->coInfoV = info.coInfoV;

		return *this;
	}

}MODINFO;
