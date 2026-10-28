#pragma once
#include "stdafx.h"
#include "TypeConversion.h"
#include "HelperFunction.h"
#include <cmath>

#define PI 3.14

#define EMPTY_CHECK(img) (img.data == 0x00)
#define CV_TRY_CATCH_NOEXCEPTION(a)		\
		try {							\
			a;							\
		}								\
		catch (const cv::Exception& e){	\
		}

using namespace cv;

namespace NSE
{
	enum LoadType{
		UNCHANGED = -1,
		GRAY,
		COLOR
	};
	enum AutoThTypes{
		ISODATA = 0,
		HUANG,
		INTERMODES,
		LI,
		MEAN,
		YEN,
		TRIAGLE,
		OTSU,
	};

	enum KernelTypes {
		// Only 3x3 filter
		EMBOSSING,
		LAPLACIAN_4WAY,
		LAPLACIAN_8WAY,
		UNSHARPEN_4WAY,
		UNSHARPEN_8WAY,
		SOVELV,
		SOVELH,
		SCHARRV,
		SCHARRH,
		PREWITTV,
		PREWITTH,
		ROBERTSV,
		ROVERTSH,
		MORPH_TEST1,
		// NxM filter
		BLUR_GAUSSIAN,
		BLUR_MEDIAN,
		EDGE,
		// Only NxN filter
		BLUR_MEAN,
		SHARPEN,		// Only 3x3 Gaussian filter (sigma = 1)	
	};

	struct Label {
		cv::Mat labeledImg;
		cv::Mat stats;
		cv::Mat centroids;
		int numOfLabel;
	};
	
	struct MinMax {
		double	d_min;
		double	d_max;
		int		n_min;
		int		n_max;
	};

	enum MarphShapeExtra {
		MORPH_XCROSS = 3,
		MORPH_SELF
	};

	struct NonZeroData {
		size_t sum;
		size_t num;
		float avg;
		float max;
	};

	struct AveMinMax {
		int avg;
		int min;
		int max;
		int area;
	};

	enum SubtractTypes {
		SUBTRACT_ALL,
		SUBTRACT_NON_ZERO
	};

	struct SeparateDent {
		Mat positiveImg;
		Mat negativeImg;
	};

	void vKernelInit();

	/////////// Log 기록
	void WriteLog(const CString& logMessage);


	/////////// 파일 관련
	bool Load(cv::Mat& src, const CString& cstr, const int& nType = NSE::LoadType::UNCHANGED);
	bool Load(cv::Mat& src, const String& str, const int& nType = NSE::LoadType::UNCHANGED);

	bool Save(const cv::Mat& src, const CString& cstr, const std::vector<int>& compression_params = std::vector<int>());
	bool Save(const cv::Mat& src, const String& str, const std::vector<int>& compression_params = std::vector<int>());

	bool Savetiff(const cv::Mat& src, const CString& cstr);
	bool Savetiff(const cv::Mat& src, const String& str);


	/////////// 이미지 밝기 변환 관련
	cv::Mat GammaCorrection(cv::Mat& src, const float alpha, const float gamma);
	cv::Mat LogTransformation(cv::Mat& src, double scale);

	// operation 2 images - image light transform
	cv::Mat LightenOnly(cv::Mat& src1, cv::Mat& src2);
	cv::Mat Screen(cv::Mat& src1, cv::Mat& src2);
	cv::Mat Dodge(cv::Mat& src1, cv::Mat& src2);
	cv::Mat Addition(cv::Mat& src1, cv::Mat& src2);
	cv::Mat DarkenOnly(cv::Mat& src1, cv::Mat& src2);
	cv::Mat Multiply(cv::Mat& src1, cv::Mat& src2);
	cv::Mat Burn(cv::Mat& src1, cv::Mat& src2);
	cv::Mat Overlay(cv::Mat& src1, cv::Mat& src2);
	cv::Mat SoftLight(cv::Mat& src1, cv::Mat& src2);
	cv::Mat HardLight(cv::Mat& src1, cv::Mat& src2);
	cv::Mat Difference(cv::Mat& src1, cv::Mat& src2);
	cv::Mat Subtraction(cv::Mat& src1, cv::Mat& src2);
	cv::Mat GrainExtract(cv::Mat& src1, cv::Mat& src2);
	cv::Mat GrainMerge(cv::Mat& src1, cv::Mat& src2);
	cv::Mat Divide(cv::Mat& src1, cv::Mat& src2);
	cv::Mat BitOR(cv::Mat& src1, cv::Mat& src2);
	cv::Mat BitAND(cv::Mat& src1, cv::Mat& src2);
	cv::Mat BitXOR(cv::Mat& src1, cv::Mat& src2);
	cv::Mat BitNOT(cv::Mat& src1);

	cv::Mat Addition(cv::Mat& src1, int n);
	cv::Mat Subtraction(cv::Mat& src1, int n);
	cv::Mat Multiply(cv::Mat& src1, int n);
	cv::Mat Divide(cv::Mat& src1, int n);
		
	cv::Mat ConvertColor(cv::Mat& src);
	cv::Mat ConvertGray(cv::Mat& src);

	/////////// 필터링 관련
	cv::Mat BilateralFilter(const cv::Mat& src, int nDiameter = -1, double dbSigma = 10);
	cv::Mat BoxFilterBlur(const cv::Mat& src, int nSize = 3);
	cv::Mat Sharpen2D(const cv::Mat& src, float fValue = 4);
	cv::Mat GaussianFilter(const cv::Mat& src, int nSize = 3, double sigma = 1.0);
	cv::Mat MedianBlur(const cv::Mat& src, int nSize = 3);
	cv::Mat GaborFilter(const cv::Mat& src, int nSize = 21);
	cv::Mat Denoising(const cv::Mat& src, float h = 3, int tempWindow = 7, int searchWindow = 21);

	cv::Mat Morphology(const cv::Mat& src, cv::MorphTypes types, int nSize = 3, int iter = 1);

	cv::Mat Sobel(const cv::Mat& src, int dx, int dy, int kSize);
	cv::Mat SobelAll(const cv::Mat& src, int kSize);
	cv::Mat Scharr(const cv::Mat& src, int dx, int dy, double scale);
	cv::Mat Laplacian(const cv::Mat& src, int kSize);

	cv::Mat Filter_2D(const cv::Mat& src, KernelTypes kType, cv::Size kernel = cv::Size(3, 3), double gaussianKernelParam = 1.0, double sharpKernelParam = 1.0,
		int depth = -1, cv::Point anchor = cv::Point(-1, -1), double delta = 0, cv::BorderTypes bType = cv::BORDER_REFLECT_101);
	cv::Mat GaussianFilterGen(cv::Size& kernel = cv::Size(3, 3), double sigma = 1.0);

	/////////// Histogram, Labeling, Threshold 관련
	// histogram
	cv::Mat HistogramStretching(const cv::Mat &src, double saturated = 0.0, INT32 area = 0);
	MinMax	GetMinAndMax(const cv::Mat &src, double saturated = 0.0);
	MinMax	GetMinAndMaxArea(const cv::Mat &src, double saturated = 0.0, const INT32 area = 0);

	MinMax	GetMinAndMax16(const cv::Mat &src, double saturated = 0.0);
	MinMax	GetMinAndMaxArea16(const cv::Mat &src, double saturated = 0.0, const INT32 area = 0);
	cv::Mat HistogramEqualization(const cv::Mat &src);
	cv::Mat HistogramNormalization(const cv::Mat &src);
	cv::Mat HIstogramClahe(const cv::Mat &src, int nLimit = 4);

	cv::Mat MinMaxStretching(const cv::Mat &src, double min, double max);
	MinMax	GetMinMax(const cv::Mat &src);
		
	//AveMinMax GetAverage(const cv::Mat &src, const int LowCut=0, const int HighCut=255);
	//AveMinMax GetMaxPixelLevel(const cv::Mat &src, const int LowCut, const int HighCut);

	// labeling
	NSE::Label Labelling(const cv::Mat& src, int connectivity = 8, int type = CV_32S);
	cv::Mat RemoveSmallPixel(cv::Mat& src, int iMin);

	// distance transform
	cv::Mat DistanceTransform(const cv::Mat &src, int type, int mask);

	// threshold
	cv::Mat CalcGrayHist(const Mat &img);
	cv::Mat Calc16bitHist(const Mat &img);
	bool bimodal(const Mat& y);

	int IsoData(const Mat& his);
	int Huang(const Mat& his);
	int Intermodes(const Mat& his);
	int Li(const Mat& his);
	int Mean(const Mat& his);
	int Yen(const Mat& his);
	int Triangle(const Mat& his);

	cv::Mat Threshold(const cv::Mat& src, double threshold, double threshMaxVal = 255, cv::ThresholdTypes type = cv::THRESH_BINARY);
	cv::Mat Threshold(const Mat &src, NSE::AutoThTypes autoTh, double threshMaxVal = 255, cv::ThresholdTypes type = cv::THRESH_BINARY);
	cv::Mat AdaptiveThreshold(const Mat& src, double maxVal = 255.0, cv::AdaptiveThresholdTypes adaptType = cv::AdaptiveThresholdTypes::ADAPTIVE_THRESH_MEAN_C, cv::ThresholdTypes type = cv::THRESH_BINARY, int nBlockSize = 3, double c = 0.0f);
	cv::Mat ThresholdMinMax(const cv::Mat& src, int min, int max);
	cv::Mat ThresholdMinMax16(const cv::Mat& src, int min, int max);

	cv::Mat ThresholdMinMax16_2(const cv::Mat& src, int min, int max, int val, double& dbObjMean);

	// NSE Inspect Algorithm
	cv::Mat NSECond1(cv::Mat& src, int nTh);
	cv::Mat NSECond2(cv::Mat& src, int nTh);
	cv::Mat NSECond3(cv::Mat& src, int nTh);
	cv::Mat NSECond4(cv::Mat& src, int nTh);
	cv::Mat NSECond5(cv::Mat& src, int nTh);
	cv::Mat NSECond6(cv::Mat& src, int nTh);

	// ImgBuf <-> Mat 변환
	cv::Mat ImgBuftoMat(unsigned int **Img_Buf, int w, int h);		// 8bit
	cv::Mat ImgBuftoMatchar(unsigned char **Img_Buf, int w, int h);		// 8bit
	cv::Mat ImgBuftoMat16(unsigned int **Img_Buf, int w, int h);		// 16bit
	cv::Mat ImgBuftoMat16_re(unsigned int **Img_Buf, int w, int h, int re);		// 16bit
	unsigned int** MattoImgBuf(const cv::Mat src);		// 8bit
	unsigned char** MattoImgBufchar(const cv::Mat src);		// 8bit
	unsigned int** MattoImgBuf16(const cv::Mat src);		// 16bit

	cv::Mat ConvertToColorPx(const cv::Mat& src, int nVal, Scalar sc);

	// Pixel 정보
	void MinMaxAvg(std::vector<int> vPx, int &min, int &max, double &avg);
	double StandardDevitation(std::vector<int> vPx, double avg);
	double StandardDevitation(std::vector<int> vPx);

	// 16 bit <-> 8 bit 변환
	cv::Mat Normalization(cv::Mat& src, double min, double max);		// Normalization
	cv::Mat Normalization8bit(cv::Mat& src);	// 16bit 이미지 8bit Gray Scale로 normalization (16 bit 내 min/max를 0~255로)
	cv::Mat Normalization16bit(cv::Mat& src);	// 8bit 이미지 16bit Gray Scale로 normalization (8 bit 내 min/max를 0~65535로)
	cv::Mat ConvertTo8bit(cv::Mat& src);			// 16bit 이미지 8bit로 변환 (px / 256)
	cv::Mat ConvertTo8bit_MinMax(cv::Mat& src);			// 16bit 이미지 8bit로 변환 (이미지 내 MinMax 고려)
	cv::Mat ConvertTo16bit(cv::Mat& src);			// 8bit 이미지 16bit로 변환 (px * 256)

	///////////////// 제품 전처리, 특수검사 알고리즘
	// 병제품 바닥 예외처리
	cv::Mat ExceptionBottleBottom(cv::Mat& src, int nTh, int nPoint);
	
	// 제품 외곽 마스킹
	cv::Mat MaskingOutside(const cv::Mat& src, int iter);

	// 알약 검사 labeling
	cv::Mat InspectPill(const cv::Mat& src, int min, int max, int &minPill, int &maxPill, int &numPill);

	// 채반 검사 Masking
	cv::Mat MaskingGridTray(const cv::Mat& src, int block, double c);

	// 듀얼 디텍터
	cv::Mat InspectDual(const cv::Mat& src);		// 닭가슴살
	cv::Mat InspectDualFish(const cv::Mat& src);

	// 누락 검사
	bool InspectOmit(const cv::Mat& src, int low, int high, int OmitNum, int perL, int perH);

	// 과자박스 누락
	//bool InspectBoxOmit(const cv::Mat& src, int low, int high, int size, int wCount, int hCount, int num);
	cv::Mat InspectBoxOmit(const cv::Mat& src, int low, int high, int size, int wCount, int hCount, int num, int masklr, int masktb);
}

