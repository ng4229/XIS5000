#include "stdafx.h"
#include "ImageProcessing.h"
#include <stdio.h>
#include <vector>
#include <cmath>
#include <numeric>

using namespace std;
using namespace cv;

vector<pair<int, Mat>> kVecKernelPair;

void NSE::vKernelInit()
{
	pair<int, Mat> kernel;

	int data1[] = {-1, -1, 0, -1, 0, 1, 0, 1, 1 };
	kernel.first = NSE::EMBOSSING;
	kernel.second = cv::Mat(3, 3, CV_8U, data1);
	kVecKernelPair.push_back(kernel	);

	int data2[] = {0, 1, 0, 1, -4, 1, 0, 1, 0};
	kernel.first = NSE::LAPLACIAN_4WAY;
	kernel.second = cv::Mat(3, 3, CV_8U, data2);
	kVecKernelPair.push_back(kernel	);

	int data3[] = {1, 1, 1, 1, -8, 1, 1, 1, 1};
	kernel.first = NSE::LAPLACIAN_8WAY;
	kernel.second = cv::Mat(3, 3, CV_8U, data3);
	kVecKernelPair.push_back(kernel	);

	int data4[] = {0, -1, 0, -1, 5, -1, 0, -1, 0};
	kernel.first = NSE::UNSHARPEN_4WAY;
	kernel.second = cv::Mat(3, 3, CV_8U, data4);
	kVecKernelPair.push_back(kernel	);

	int data5[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
	kernel.first = NSE::SOVELV;
	kernel.second = cv::Mat(3, 3, CV_8U, data5);
	kVecKernelPair.push_back(kernel	);

	int data6[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
	kernel.first = NSE::SOVELH;
	kernel.second = cv::Mat(3, 3, CV_8U, data6);
	kVecKernelPair.push_back(kernel	);

	int data7[] = {-3, 0, 3, -10, 0, 10, -3, 0, 3};
	kernel.first = NSE::SCHARRV;
	kernel.second = cv::Mat(3, 3, CV_8U, data7);
	kVecKernelPair.push_back(kernel	);

	int data8[] = {-3, -10, 3, 0, 0, 0, -3, 10, 3};
	kernel.first = NSE::SCHARRH;
	kernel.second = cv::Mat(3, 3, CV_8U, data8);
	kVecKernelPair.push_back(kernel	);

	int data9[] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
	kernel.first = NSE::PREWITTV;
	kernel.second = cv::Mat(3, 3, CV_8U, data9);
	kVecKernelPair.push_back(kernel	);

	int data10[] = {1, 1, 1, 0, 0, 0, -1, -1, -1};
	kernel.first = NSE::PREWITTH;
	kernel.second = cv::Mat(3, 3, CV_8U, data10);
	kVecKernelPair.push_back(kernel	);

	int data11[] = {-1, 0, 0, 0, 1, 0, 0, 0, 0};
	kernel.first = NSE::ROBERTSV;
	kernel.second = cv::Mat(3, 3, CV_8U, data11);
	kVecKernelPair.push_back(kernel	);

	int data12[] = {0, 0, 1, 0, 1, 0, 0, 0, 0};
	kernel.first = NSE::ROVERTSH;
	kernel.second = cv::Mat(3, 3, CV_8U, data12);
	kVecKernelPair.push_back(kernel	);
}

void NSE::WriteLog(const CString& logMessage)
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	CString timeStr;
	timeStr.Format(_T("[%04d%02d%02d-%02d:%02d:%02d:%03d] "),
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	CString finalLog = timeStr + logMessage + _T("\r\n");

	CStdioFile file;
	if (file.Open(_T("D:\\cvErrorLog.log"),
		CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
	{
		file.SeekToEnd(); // 파일 끝으로 이동
		file.WriteString(finalLog);
		file.Close();
	}
}

/////////// 파일 관련
bool NSE::Load(cv::Mat& src, const CString& cstr, const int& nType)
{
	if(Helper::CheckFileExists(cstr))																															
	{
		src = imread(cstos(cstr), nType);
		return true;
	}
	else
		return false;
}

bool NSE::Load(cv::Mat& src, const String& str, const int& nType)
{
	if(Helper::CheckFileExists(stocs(str)))																															
	{
		src = imread(str, nType);
		return true;
	}
	else
		return false;
}

// 저장하는 버튼 옆에 compression option 선택할 수 있는 list와 option 값을 입력할 수 있는 Editbox 추가
// ?? 버튼 만들어서 저장 옵션 설명 창 뜨도록 구성
bool NSE::Save(const cv::Mat& src, const CString& cstr, const std::vector<int>& compression_params)
{
	CString path = cstr;

	try
	{
		if (src.empty())
		{
			WriteLog(_T("Save failed: src.empty()"));
			return false;
		}

		CString log;
		log.Format(_T("Save enter. path=%s, rows=%d, cols=%d, type=%d"),
			path.GetString(), src.rows, src.cols, src.type());
		WriteLog(log);

		std::string filePath = CT2A(path);

		bool ok = cv::imwrite(filePath, src, compression_params);

		log.Format(_T("Save leave. ok=%d, path=%s"), ok ? 1 : 0, path.GetString());
		WriteLog(log);

		return ok;
	}
	catch (const cv::Exception& ex)
	{
		CString log;
		log.Format(_T("cv::Exception in Save. path=%s, reason=%S"),
			path.GetString(), ex.what());
		WriteLog(log);
		return false;
	}
	catch (const std::exception& ex)
	{
		CString log;
		log.Format(_T("std::exception in Save. path=%s, reason=%S"),
			path.GetString(), ex.what());
		WriteLog(log);
		return false;
	}
	catch (...)
	{
		CString log;
		log.Format(_T("Unknown exception in Save. path=%s"), path.GetString());
		WriteLog(log);
		return false;
	}
}

bool NSE::Save(const cv::Mat& src, const String& str, const std::vector<int>& compression_params)
{
	bool result = false;
	
	try
    {
        result = imwrite(str, src, compression_params);
    }
    catch (const cv::Exception& ex)
    {
		CString cstr;
		cstr.Format(_T("Exception Save image to : %S"), ex.what());
		WriteLog(cstr);
    }

	return result;
}

bool NSE::Savetiff(const cv::Mat& src, const CString& cstr)
{
	std::vector<int> tags;
	tags.push_back(cv::TiffTag::TIFF_TAG_COMPRESSION);
	tags.push_back(cv::TiffCompression::TIFF_UNCOMP);

	try
    {
		imwrite(cstos(cstr), src, tags);
    }
    catch (const cv::Exception& ex)
    {
		CString cstr;
		cstr.Format(_T("Exception Save image to : %S"), ex.what());
		WriteLog(cstr);
		return 1;
    }

	return 0;
}

bool NSE::Savetiff(const cv::Mat& src, const String& str)
{
	std::vector<int> tags;
	tags.push_back(cv::TiffTag::TIFF_TAG_COMPRESSION);
	tags.push_back(cv::TiffCompression::TIFF_UNCOMP);

	try
    {
        imwrite(str, src, tags);
    }
    catch (const cv::Exception& ex)
    {
		CString cstr;
		cstr.Format(_T("Exception Save image to : %S"), ex.what());
		WriteLog(cstr);
		return 1;
    }

	return 0;
}

/////////// 이미지 밝기 변환 관련
// parameter를 Scroll형태로 하여 실시간으로 볼 수 있도록..??? best
cv::Mat NSE::GammaCorrection(cv::Mat& src, const float alpha, const float gamma)
{
	Mat lookUpTable(1, 256, CV_8U);
	uchar* p = lookUpTable.ptr();
	for (int i = 0; i < 256; i++) {
		if (i < alpha)
			p[i] = saturate_cast<uchar>(i);
		else 
			p[i] = saturate_cast<uchar>(powf((i - alpha) / 255.0, 1 / gamma) * 255.0 + alpha);
	}

	Mat dst = Mat();
	LUT(src, lookUpTable, dst);
	return dst;
}

cv::Mat NSE::LogTransformation(cv::Mat& src, double scale)
{
	Mat m_floatImg, m_logImg;
	Mat m_Origin = src.clone();

	m_Origin.convertTo(m_floatImg, CV_32F);
	cv::add(abs(m_floatImg), 1, m_floatImg);
	log(m_floatImg, m_floatImg);
	normalize(m_floatImg, m_floatImg, 0, 255, NORM_MINMAX);
	convertScaleAbs(m_floatImg, m_logImg, scale);

	return m_logImg;
}

cv::Mat NSE::LightenOnly(cv::Mat& src1, cv::Mat& src2)
{
	Mat m_LightOnly(src1.size(), CV_8UC1);

	uchar* pt1 = src1.data;
	uchar* pt2 = src2.data;
	uchar* ptLight = m_LightOnly.data;

	for (auto i = 0; i < src1.cols; i++)
		for (auto j = 0; j < src1.rows; j++)
		{
			int pxSrc1 = pt1[i * src1.rows + j];
			int pxSrc2 = pt2[i * src1.rows + j];

			int px;
			pxSrc1 >= pxSrc2 ? px = pxSrc1 : px = pxSrc2;
				ptLight[i * src1.rows + j] = saturate_cast<uchar>(px);
		}

	return m_LightOnly;
}
cv::Mat NSE::Screen(cv::Mat& src1, cv::Mat& src2)
{
	Mat m_fScreen(src1.size(), CV_32FC1);

	uchar* pt1 = src1.data;
	uchar* pt2 = src2.data;
	float* ptScreen = (float*)m_fScreen.data;

	for(auto i= 0 ; i< src1.cols; i++)
		for (auto j = 0; j < src1.rows; j++)
		{
			float pxSrc1 = pt1[i * src1.rows + j];
			float pxSrc2 = pt2[i * src1.rows + j];

			float px = 255 - ((255 - pxSrc1) * (255 - pxSrc2) / 255);
			ptScreen[i * src1.rows + j] = px;
		}

	normalize(m_fScreen, m_fScreen, 0, 255, NORM_MINMAX);
	Mat m_Screen;
	m_fScreen.convertTo(m_Screen, CV_8UC1);

	return m_Screen;
}
cv::Mat NSE::Dodge(cv::Mat& src1, cv::Mat& src2)
{
	Mat m_Dodge(src1.size(), CV_8UC1);

	uchar* pt1 = src1.data;
	uchar* pt2 = src2.data;
	uchar* ptDogde = m_Dodge.data;

	for (auto i = 0; i < src1.cols; i++)
		for (auto j = 0; j < src1.rows; j++)
		{
			int pxSrc1 = pt1[i * src1.rows + j];
			int pxSrc2 = pt2[i * src1.rows + j];

			int px = (256 * pxSrc2) / ((255 - pxSrc1) + 1);
				ptDogde[i * src1.rows + j] = saturate_cast<uchar>(px);
		}

	return m_Dodge;
}
cv::Mat NSE::Addition(cv::Mat& src1, cv::Mat& src2)
{
	Mat m_Addition;
	cv::add(src1, src2, m_Addition);

	return m_Addition;
}
cv::Mat NSE::DarkenOnly(cv::Mat& src1, cv::Mat& src2)
{
	Mat m_DarkOnly(src1.size(), CV_8UC1);

	uchar* pt1 = src1.data;
	uchar* pt2 = src2.data;
	uchar* ptDark = m_DarkOnly.data;

	for (auto i = 0; i < src1.cols; i++)
		for (auto j = 0; j < src1.rows; j++)
		{
			int pxSrc1 = pt1[i * src1.rows + j];
			int pxSrc2 = pt2[i * src1.rows + j];

			int px;
			pxSrc1 <= pxSrc2 ? px = pxSrc1 : px = pxSrc2;
				ptDark[i * src1.rows + j] = saturate_cast<uchar>(px);
		}

	return m_DarkOnly;
}
cv::Mat NSE::Multiply(cv::Mat& src1, cv::Mat& src2)
{
	Mat m_Multiply;
	cv::multiply(src1, src2, m_Multiply);

	return m_Multiply;
}
cv::Mat NSE::Burn(cv::Mat& src1, cv::Mat& src2)
{
	Mat m_fBurn(src1.size(), CV_32FC1);

	uchar* pt1 = src1.data;
	uchar* pt2 = src2.data;
	float* ptBurn = (float*)m_fBurn.data;

	for(auto i= 0 ; i< src1.cols; i++)
		for (auto j = 0; j < src1.rows; j++)
		{
			float pxSrc1 = pt1[i * src1.rows + j];
			float pxSrc2 = pt2[i * src1.rows + j];

			float px = 255 - ((256 * (255 - pxSrc2)) / (pxSrc1 + 1));
			ptBurn[i * src1.rows + j] = px;
		}

	normalize(m_fBurn, m_fBurn, 0, 255, NORM_MINMAX);
	Mat m_Burn;
	m_fBurn.convertTo(m_Burn, CV_8UC1);

	return m_Burn;
}
cv::Mat NSE::Overlay(cv::Mat& src1, cv::Mat& src2)
{
	Mat m_fOverlay(src1.size(), CV_32FC1);

	uchar* pt1 = src1.data;
	uchar* pt2 = src2.data;
	float* ptOver = (float*)m_fOverlay.data;

	for(auto i= 0 ; i< src1.cols; i++)
		for (auto j = 0; j < src1.rows; j++)
		{
			float pxSrc1 = pt1[i * src1.rows + j];
			float pxSrc2 = pt2[i * src1.rows + j];

			float px = (pxSrc2 / 255) * (pxSrc2 + ((2 * pxSrc1) / 255) * (255 - pxSrc2));
			ptOver[i * src1.rows + j] = px;
		}

	normalize(m_fOverlay, m_fOverlay, 0, 255, NORM_MINMAX);
	Mat m_Overlay;
	m_fOverlay.convertTo(m_Overlay, CV_8UC1);

	return m_Overlay;
}
cv::Mat NSE::SoftLight(cv::Mat& src1, cv::Mat& src2)
{
	Mat m_Screen = NSE::Screen(src1, src2);

	Mat m_fSoftLight(src1.size(), CV_32FC1);

	uchar* pt1 = src1.data;
	uchar* pt2 = src2.data;
	uchar* ptS = m_Screen.data;

	float* ptSoft = (float*)m_fSoftLight.data;

	for(auto i= 0 ; i< src1.cols; i++)
		for (auto j = 0; j < src1.rows; j++)
		{
			float pxSrc1 = pt1[i * src1.rows + j];
			float pxSrc2 = pt2[i * src1.rows + j];
			float pxScreen = ptS[i * src1.rows + j];

			float px = ((255 - pxSrc2) * pxSrc1 + pxScreen) / 255 * pxSrc1;
			ptSoft[i * src1.rows + j] = px;
		}

	normalize(m_fSoftLight, m_fSoftLight, 0, 255, NORM_MINMAX);
	Mat m_SoftLight;
	m_fSoftLight.convertTo(m_SoftLight, CV_8UC1);

	return m_SoftLight;
}
cv::Mat NSE::HardLight(cv::Mat& src1, cv::Mat& src2)
{
	Mat m_fHardLight(src1.size(), CV_32FC1);

	uchar* pt1 = src1.data;
	uchar* pt2 = src2.data;
	float* ptHard = (float*)m_fHardLight.data;

	for(auto i= 0 ; i< src1.cols; i++)
		for (auto j = 0; j < src1.rows; j++)
		{
			float pxSrc1 = pt1[i * src1.rows + j];
			float pxSrc2 = pt2[i * src1.rows + j];

			float px;
			if(pxSrc1 > 128)
				px = 255 - (((255 - 2) * (pxSrc1 - 128)) * (255 - pxSrc2)) / 256;
			else
				px = (2 * pxSrc1 * pxSrc2) / 256;

			ptHard[i * src1.rows + j] = px;
		}

	normalize(m_fHardLight, m_fHardLight, 0, 255, NORM_MINMAX);
	Mat m_HardLight;
	m_fHardLight.convertTo(m_HardLight, CV_8UC1);

	return m_HardLight;
}
cv::Mat NSE::Difference(cv::Mat& src1, cv::Mat& src2)
{
	Mat m_Difference;
	cv::absdiff(src1, src2, m_Difference);

	return m_Difference;
}
cv::Mat NSE::Subtraction(cv::Mat& src1, cv::Mat& src2)
{
	Mat m_Subtraction;
	cv::subtract(src1, src2, m_Subtraction);

	return m_Subtraction;
}
cv::Mat NSE::GrainExtract(cv::Mat& src1, cv::Mat& src2)
{
	Mat m_GrainExtract(src1.size(), CV_8UC1);

	uchar* pt1 = src1.data;
	uchar* pt2 = src2.data;
	uchar* ptGrain = m_GrainExtract.data;

	for (auto i = 0; i < src1.cols; i++)
		for (auto j = 0; j < src1.rows; j++)
		{
			int pxSrc1 = pt1[i * src1.rows + j];
			int pxSrc2 = pt2[i * src1.rows + j];

			int px = pxSrc2 - pxSrc1 + 128;
				ptGrain[i * src1.rows + j] = saturate_cast<uchar>(px);
		}

	return m_GrainExtract;
}
cv::Mat NSE::GrainMerge(cv::Mat& src1, cv::Mat& src2)
{
	Mat m_GrainMerge(src1.size(), CV_8UC1);

	uchar* pt1 = src1.data;
	uchar* pt2 = src2.data;
	uchar* ptGrain = m_GrainMerge.data;

	for (auto i = 0; i < src1.cols; i++)
		for (auto j = 0; j < src1.rows; j++)
		{
			int pxSrc1 = pt1[i * src1.rows + j];
			int pxSrc2 = pt2[i * src1.rows + j];

			int px = pxSrc2 + pxSrc1 - 128;
				ptGrain[i * src1.rows + j] = saturate_cast<uchar>(px);
		}

	return m_GrainMerge;
}
cv::Mat NSE::Divide(cv::Mat& src1, cv::Mat& src2)
{
	Mat m_Divide;
	cv::divide(src1, src2, m_Divide);

	return m_Divide;
}

cv::Mat NSE::BitOR(cv::Mat& src1, cv::Mat& src2)
{
	Mat m_OR;
	cv::bitwise_or(src1, src2, m_OR);

	return m_OR;
}

cv::Mat NSE::BitAND(cv::Mat& src1, cv::Mat& src2)
{
	Mat m_AND;
	cv::bitwise_and(src1, src2, m_AND);

	return m_AND;
}

cv::Mat NSE::BitXOR(cv::Mat& src1, cv::Mat& src2)
{
	Mat m_XOR;
	cv::bitwise_xor(src1, src2, m_XOR);

	return m_XOR;
}

cv::Mat NSE::BitNOT(cv::Mat& src1)
{
	Mat m_NOT;
	cv::bitwise_not(src1, m_NOT);

	return m_NOT;
}

cv::Mat NSE::Addition(cv::Mat& src1, int n)
{
	Mat m_img1 = src1.clone();
	Mat m_img2(m_img1.size(), m_img1.type(), n);
	Mat m_add;

	cv::add(m_img1, m_img2, m_add);

	return m_add;
}

cv::Mat NSE::Subtraction(cv::Mat& src1, int n)
{
	return src1 - n;
}

cv::Mat NSE::Multiply(cv::Mat& src1, int n)
{
	return src1 * n;
}

cv::Mat NSE::Divide(cv::Mat& src1, int n)
{
	return src1 / n;
}

cv::Mat NSE::ConvertColor(cv::Mat& src)
{
	Mat m_Conv;
	if(src.channels() == 1){
		cv::cvtColor(src, m_Conv, cv::COLOR_GRAY2BGR);
		return m_Conv;
	}
	else
		return src;
}

cv::Mat NSE::ConvertGray(cv::Mat& src)
{
	Mat m_Conv;
	if(src.channels() == 3){
		cv::cvtColor(src, m_Conv, cv::COLOR_BGR2GRAY);
		return m_Conv;
	}
	else
		return src;
}

/////////// 필터링 관련
cv::Mat NSE::BilateralFilter(const cv::Mat& src, int nDiameter, double dbSigma)
{
	Mat dst;
	
	nDiameter < -1 ? nDiameter = -1 : nDiameter = nDiameter;
	dbSigma < 0 ? dbSigma = 0 : dbSigma = dbSigma;

	cv::bilateralFilter(src, dst, nDiameter, dbSigma, dbSigma);

	return dst;
}

cv::Mat NSE::BoxFilterBlur(const cv::Mat& src, int nSize)
{
	Mat dst;

	nSize % 2 == 0 ? nSize++ : nSize = nSize;	nSize % 2 == 0 ? nSize++ : nSize = nSize;

	cv::blur(src, dst, cv::Size(nSize, nSize));

	return dst;
}

cv::Mat NSE::Sharpen2D(const cv::Mat& src, float fValue)
{
	Mat dst;
	fValue < 0 ? fValue*=(-1) : fValue = fValue;
	// Construct kernel
	cv::Mat kernel(3, 3, CV_32F, cv::Scalar(0));

	// assigns kernel values
	kernel.at<float>(1, 1) = fValue;
	kernel.at<float>(0, 1) = -1.0;
	kernel.at<float>(2, 1) = -1.0;
	kernel.at<float>(1, 0) = -1.0;
	kernel.at<float>(1, 2) = -1.0;

	cv::filter2D(src, dst, src.depth(), kernel);

	return dst;
}

cv::Mat NSE::GaussianFilter(const cv::Mat& src, int nSize, double sigma)
{
	Mat dst;
	
	Mat Filter = NSE::GaussianFilterGen(Size(nSize, nSize), sigma);
	cv::filter2D(src, dst, src.depth(), Filter);

	return dst;
}

cv::Mat NSE::MedianBlur(const cv::Mat& src, int nSize)
{
	Mat dst;

	nSize % 2 == 0 ? nSize++ : nSize = nSize;

	cv::medianBlur(src, dst, nSize);

	return dst;
}

cv::Mat NSE::GaborFilter(const cv::Mat& src, int nSize)
{
	Mat dst;

	nSize % 2 == 0 ? nSize++ : nSize = nSize;

	Mat kernel = cv::getGaborKernel(Size(nSize, nSize), 5, 1, 10, 1);
	cv::filter2D(src, dst, src.depth(), kernel);

	return dst;
}

cv::Mat NSE::Denoising(const cv::Mat& src, float h, int tempWindow, int searchWindow)
{
	Mat dst;

	tempWindow % 2 == 0 ? tempWindow++ : tempWindow = tempWindow;
	searchWindow % 2 == 0 ? searchWindow++ : searchWindow = searchWindow;

	cv::fastNlMeansDenoising(src, dst, h, tempWindow, searchWindow);

	return dst;
}

cv::Mat NSE::Morphology(const cv::Mat& src, cv::MorphTypes types, int nSize, int iter)
{
	Mat dst;

	nSize % 2 == 0 ? nSize++ : nSize = nSize;

  //	  MORPH_ERODE    = 0, //!< see cv::erode
  //  MORPH_DILATE   = 1, //!< see cv::dilate
  //  MORPH_OPEN     = 2, //!< an opening operation
  //                      //!< \f[\texttt{dst} = \mathrm{open} ( \texttt{src} , \texttt{element} )= \mathrm{dilate} ( \mathrm{erode} ( \texttt{src} , \texttt{element} ))\f]
  //  MORPH_CLOSE    = 3, //!< a closing operation
  //                      //!< \f[\texttt{dst} = \mathrm{close} ( \texttt{src} , \texttt{element} )= \mathrm{erode} ( \mathrm{dilate} ( \texttt{src} , \texttt{element} ))\f]
  //  MORPH_GRADIENT = 4, //!< a morphological gradient
  //                      //!< \f[\texttt{dst} = \mathrm{morph\_grad} ( \texttt{src} , \texttt{element} )= \mathrm{dilate} ( \texttt{src} , \texttt{element} )- \mathrm{erode} ( \texttt{src} , \texttt{element} )\f]
  //  MORPH_TOPHAT   = 5, //!< "top hat"
  //                      //!< \f[\texttt{dst} = \mathrm{tophat} ( \texttt{src} , \texttt{element} )= \texttt{src} - \mathrm{open} ( \texttt{src} , \texttt{element} )\f]
  //  MORPH_BLACKHAT = 6  //!< "black hat"
  //                      //!< \f[\texttt{dst} = \mathrm{blackhat} ( \texttt{src} , \texttt{element} )= \mathrm{close} ( \texttt{src} , \texttt{element} )- \texttt{src}\f]

	cv::morphologyEx(src, dst, types, Mat::ones(Size(nSize, nSize), CV_8UC1), cv::Point(-1,-1), iter);

	return dst;
}

cv::Mat NSE::Sobel(const cv::Mat& src, int dx, int dy, int kSize)
{
	Mat dst;

	cv::Sobel(src, dst, src.depth(), dx, dy, kSize);

	return dst;
}

cv::Mat NSE::SobelAll(const cv::Mat& src, int kSize)
{
	Mat dst;
	Mat m_Sobelx1, m_Sobely1, m_Sobelx2, m_Sobely2;

	Sobel(src, m_Sobelx1, -1, 1, 0, kSize);
	Sobel(src, m_Sobely1, -1, 0, 1, kSize);

	Mat m_Flip;
	flip(src, m_Flip, -1);

	Sobel(m_Flip, m_Sobelx2, -1, 1, 0, kSize);
	Sobel(m_Flip, m_Sobely2, -1, 0, 1, kSize);
	flip(m_Sobelx2, m_Sobelx2, -1);
	flip(m_Sobely2, m_Sobely2, -1);

	Mat m_WeightSum1, m_WeightSum2;
	addWeighted(m_Sobelx1, 0.5, m_Sobely1, 0.5, 0, m_WeightSum1);
	addWeighted(m_Sobelx2, 0.5, m_Sobely2, 0.5, 0, m_WeightSum2);
	
	addWeighted(m_WeightSum1, 0.5, m_WeightSum2, 0.5, 0, dst);

	return dst;
}
cv::Mat NSE::Scharr(const cv::Mat& src, int dx, int dy, double scale)
{
	Mat dst;

	cv::Scharr(src, dst, src.depth(), dx, dy, scale);

	return dst;
}

cv::Mat NSE::Laplacian(const cv::Mat& src, int kSize)
{
	Mat dst;

	cv::Laplacian(src, dst, src.depth(), kSize);

	return dst;
}

cv::Mat NSE::Filter_2D(const cv::Mat& src, KernelTypes kType, cv::Size kernel, double gaussianKernelParam, double sharpKernelParam, int depth, cv::Point anchor, double delta, cv::BorderTypes bType)
{
	Mat dst;
	Mat Filter;
	if (kType == KernelTypes::BLUR_MEAN) Filter = Mat::ones(kernel, CV_8UC1);
	else if (kType == KernelTypes::BLUR_GAUSSIAN) Filter = GaussianFilterGen(kernel, gaussianKernelParam);
	else if (kType == KernelTypes::BLUR_MEDIAN) {
		medianBlur(src, dst, kernel.width);
		return dst;
	}
	else if (kType == KernelTypes::SHARPEN) {
		Mat temp;
		filter2D(src, temp, -1, GaussianFilterGen());
		dst = src - temp;
		dst += src;
		return dst;
	}

	if (EMPTY_CHECK(Filter)) {
		CV_TRY_CATCH_NOEXCEPTION(filter2D(src, dst, depth, kVecKernelPair[kType].second, anchor, delta, bType));
	}
	else {
		CV_TRY_CATCH_NOEXCEPTION(filter2D(src, dst, depth, Filter, anchor, delta, bType));
	}

	return dst;
}

cv::Mat NSE::GaussianFilterGen(cv::Size& kernel, double sigma)
{
	// 짝수 크기의 kernel이 입력되면 +1
	if (kernel.width % 2 == 0 || kernel.height % 2 == 0) {
		kernel.width++;
		kernel.height++;
	}

	Mat dst(kernel, CV_32FC1, cv::Scalar(0));

	// intialising standard deviation to 1.0
	double r, s = 2.0 * sigma * sigma;

	// generating KxK kernel
	int startX = -static_cast<int>(kernel.width / 2);
	int endX = static_cast<int>(kernel.width / 2);
	int startY = -static_cast<int>(kernel.height / 2);
	int endY = static_cast<int>(kernel.height / 2);
	for (int x = startX; x <= endX; x++) {
		for (int y = startY; y <= endY; y++) {
			r = sqrtf(x * x + y * y);
			dst.at<float>(Point(x + endX, y + endY)) = (exp(-(r * r) / s)) / (CV_PI * s);
		}
	}

	// normalising the kernel
	Scalar sum = cv::sum(dst);
	dst /= sum.val[0];
	return dst;
}

//////////// Histogram, Labeling, Threshold 관련
// histogram
cv::Mat NSE::MinMaxStretching(const cv::Mat &src, double min, double max)
{
	cv::Mat img = src.clone();
	int val;

	img.elemSize() * 8 == 16 ? val = 65535 : val = 255;
	Mat imgNorm = ((img - min) * 65535 / (max - min));

	return imgNorm;
}

NSE::MinMax NSE::GetMinMax(const cv::Mat &src)
{
	NSE::MinMax mm;

	Mat imgf;
	src.convertTo(imgf, CV_32F);
	cv::minMaxLoc(imgf, &mm.d_min, &mm.d_max);

	mm.n_min = (int)mm.d_min;
	mm.n_max = (int)mm.d_max;

	return mm;
}

cv::Mat NSE::HistogramStretching(const cv::Mat &src, double saturated, INT32 area)
{
	MinMax mm;
	if(src.elemSize() * 8 == 16)
	{
		if(area == 0)
			mm = GetMinAndMax16(src, saturated);
		else
			mm = GetMinAndMaxArea16(src, saturated, area);

		cv::Mat dst = (src - mm.d_min) * 65535 / (mm.d_max - mm.d_min);
		return dst;
	}
	else
	{
		if(area == 0)
			mm = GetMinAndMax(src, saturated);
		else
			mm = GetMinAndMaxArea(src, saturated, area);

		cv::Mat dst = (src - mm.d_min) * 255 / (mm.d_max - mm.d_min);
		return dst;
	}

}
NSE::MinMax NSE::GetMinAndMax(const cv::Mat &src, double saturated)
{
	NSE::MinMax mm;
	int hmin, hmax;
	int threshold;
	cv::Mat hist = NSE::CalcGrayHist(src);
	int hsize = hist.rows;
	if(saturated > 0.0)
		threshold = (int)(src.rows*src.cols*saturated / 200.0);	//
	else
		threshold = 0;

	int i = 1;
	bool found = false;
	int count = 0;
	int maxindex = hsize - 1;
	do{
		i++;
		count += hist.at<float>(i,0);
		found = count > threshold;
	} while (!found && i < maxindex);
	hmin = i;
	i = hsize;
	count = 0;
	do {
		i--;
		count += hist.at<float>(i,0);
		found = count > threshold;
	} while (!found&&i > 0);
	hmax = i;
	mm.d_min = hmin;
	mm.d_max = hmax;
	return mm;
}
NSE::MinMax NSE::GetMinAndMax16(const cv::Mat &src, double saturated)
{
	NSE::MinMax mm;
	int hmin, hmax;
	int threshold;
	cv::Mat hist = NSE::Calc16bitHist(src);
	int hsize = hist.rows;
	if(saturated > 0.0)
		threshold = (int)(src.rows*src.cols*saturated / 60000.0);	//
	else
		threshold = 0;

	int i = 1;
	bool found = false;
	int count = 0;
	int maxindex = hsize - 1;
	do{
		i++;
		count += hist.at<float>(i,0);
		found = count > threshold;
	} while (!found && i < maxindex);
	hmin = i;
	i = hsize;
	count = 0;
	do {
		i--;
		count += hist.at<float>(i,0);
		found = count > threshold;
	} while (!found&&i > 0);
	hmax = i;
	mm.d_min = hmin;
	mm.d_max = hmax;
	return mm;
}
NSE::MinMax NSE::GetMinAndMaxArea(const cv::Mat &src, double saturated, const INT32 area)
{
	MinMax mm;
	int hmin, hmax;
	int threshold;
	Mat hist = CalcGrayHist(src);
	int hsize = hist.rows;
	if (saturated > 0.0)
		threshold = (int)(area*saturated / 200.0); //
	else
		threshold = 0;
	int i = 1;
	bool found = false;
	int count = 0;
	int maxindex = hsize - 1;
	do {
		i++;
		count += hist.at<float>(i, 0);
		found = count > threshold;
	} while (!found && i < maxindex);
	hmin = i;
	i = hsize;
	count = 0;
	do {
		i--;
		count += hist.at<float>(i, 0);
		found = count > threshold;
	} while (!found&&i > 0);
	hmax = i;
	mm.d_min = hmin; mm.d_max = hmax;
	return mm;
}

NSE::MinMax NSE::GetMinAndMaxArea16(const cv::Mat &src, double saturated, const INT32 area)
{
	MinMax mm;
	int hmin, hmax;
	int threshold;
	Mat hist = Calc16bitHist(src);
	int hsize = hist.rows;
	if (saturated > 0.0)
		threshold = (int)(area*saturated / 60000.0); //
	else
		threshold = 0;
	int i = 1;
	bool found = false;
	int count = 0;
	int maxindex = hsize - 1;
	do {
		i++;
		count += hist.at<float>(i, 0);
		found = count > threshold;
	} while (!found && i < maxindex);
	hmin = i;
	i = hsize;
	count = 0;
	do {
		i--;
		count += hist.at<float>(i, 0);
		found = count > threshold;
	} while (!found&&i > 0);
	hmax = i;
	mm.d_min = hmin; mm.d_max = hmax;
	return mm;
}

cv::Mat NSE::HistogramNormalization(const cv::Mat& src)
{
	Mat dst;

	if(src.elemSize() * 8 == 16)
		cv::normalize(src, dst, 0, 65535, cv::NORM_MINMAX);
	else
	{
		Mat imgf;
		src.convertTo(imgf, CV_32F);

		vector<double> vPoint;
		double dMin, dMax;
		cv::minMaxLoc(imgf, &dMin, &dMax);
		Mat imgNorm = ((imgf - dMin) * 255 / (dMax - dMin));
		imgNorm.convertTo(dst, CV_8U);
	}

	return dst;
}

cv::Mat NSE::HistogramEqualization(const cv::Mat& src)
{
	Mat dst;

	if(src.elemSize() * 8 == 16)
	{
		dst = src.clone();

		const int hist_sz = 65536;
		int *hist = new int[hist_sz];
		int *lut = new int[hist_sz];

		for (int y = 0; y < src.rows; y++)
			for (int x = 0; x < src.cols; x++)
				hist[(int)src.at<unsigned short int>(y, x)]++;

		auto i = 0;
		while (!hist[i]) ++i;

		auto total = (int)src.total();
		if (hist[i] == total) 
		{
			dst.setTo(i);
			return dst;
		}

		float scale = (hist_sz - 1.f) / (total - hist[i]);
		auto sum = 0;

		for (lut[i++] = 0; i < hist_sz; ++i) 
		{
			sum += hist[i];
			lut[i] = cv::saturate_cast<ushort>(sum * scale);
		}

		for (int y = 0; y < src.rows; y++)
			for (int x = 0; x < src.cols; x++)
			{
				dst.at<unsigned short int>(y, x) = lut[(int)src.at<unsigned short int>(y, x)];
			}
	}
	else
		cv::equalizeHist(src, dst);

	return dst;
}

cv::Mat NSE::HIstogramClahe(const cv::Mat& src, int nLimit)
{
	if(src.elemSize() * 8 == 16)
	{
		AfxMessageBox(_T("Clahe Histogram is only 8bit grayscale image."));
		return src;
	}

	Mat dst;
	cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
	clahe->setClipLimit(nLimit);
	
	// grayscale
	if(src.channels() != 3)
		clahe->apply(src, dst);
	else
	{
		cv::Mat srcLab;
		cv::cvtColor(src, srcLab, COLOR_BGR2Lab);

		std::vector<cv::Mat> labchannels(3);
		cv::split(srcLab, labchannels);

		clahe->apply(labchannels[0], dst);
		dst.copyTo(labchannels[0]);
		cv::merge(srcLab, labchannels);

		Mat srcClahe;
		cv::cvtColor(srcLab, srcClahe, COLOR_Lab2BGR);
		dst = srcClahe.clone();
	}
	return dst;
}
// labeling
NSE::Label NSE::Labelling(const cv::Mat& src, int connectivity, int type)
{
	Label dst;
	Mat img_labels, stats, centroids;
	dst.numOfLabel = cv::connectedComponentsWithStats(src, dst.labeledImg, dst.stats, dst.centroids, connectivity, type);
	return dst;
}

cv::Mat NSE::RemoveSmallPixel(cv::Mat& src, int iMin)
{
	Mat m_imgCopy = src.clone();
	auto label = NSE::Labelling(m_imgCopy, 8);

	Mat m_Temp = label.labeledImg.clone();
	for(auto i = 1; i < label.numOfLabel; i++)
	{
		int iArea = label.stats.at<int>(i, cv::CC_STAT_AREA);
		if(iArea <= iMin)
		{
			Mat m_removelabel = (m_Temp == i) / 255 * i;
			m_removelabel.convertTo(m_removelabel, CV_32SC1);
			m_Temp -= m_removelabel;
		}
	}

	m_Temp.convertTo(m_Temp, CV_8UC1, 255);
	return m_Temp;
}

cv::Mat NSE::DistanceTransform(const cv::Mat &src, int type, int mask)
{
	Mat m_ImgCopy = src.clone();
	Mat m_Result;

	cv::distanceTransform(m_ImgCopy, m_Result, type, mask);

	return m_Result;
}


// threshold
cv::Mat NSE::CalcGrayHist(const Mat &img)
{
	CV_Assert(img.type() == CV_8UC1);

	Mat hist;
	int channels[] = { 0 };
	int dims = 1;
	const int histSize[] = { 256 };
	float graylevel[] = { 0,256 };
	const float* ranges[] = { graylevel };

	calcHist(&img, 1, channels, noArray(), hist, dims, histSize, ranges);


	return hist;
}

cv::Mat NSE::Calc16bitHist(const Mat &img)
{
	Mat hist;
	int channels[] = { 0 };
	int dims = 1;
	const int histSize[] = { 65535 };
	float level[] = { 0, 65535 };
	const float* ranges[] = { level };

	calcHist(&img, 1, channels, noArray(), hist, dims, histSize, ranges);

	return hist;
}

int NSE::IsoData(const Mat& his) {
	CV_Assert(his.type() == CV_32FC1);
	CV_Assert(his.size() == Size(1, 256));

	int length = his.rows*his.cols;
	int level;
	int maxValue = length - 1;
	double result, sum1, sum2, sum3, sum4;

	int min = 0;
	while ((his.at<float>(0, 0) == 0) && (min < maxValue))
		min++;
	int imax = maxValue;
	while ((his.at<float>(imax, 0) == 0) && (imax > 0))
		imax--;
	if (min >= imax) {
		level = length / 2;
		return level;
	}

	int movingIndex = min;
	int inc = max(imax / 40, 1);
	do {
		sum1 = sum2 = sum3 = sum4 = 0.0;
		for (int i = min; i <= movingIndex; i++) {
			sum1 += i * his.at<float>(i, 0);
			sum2 += his.at<float>(i, 0);
		}
		for (int i = (movingIndex + 1); i <= imax; i++) {
			sum3 += i * his.at<float>(i, 0);
			sum4 += his.at<float>(i, 0);
		}
		result = (sum1 / sum2 + sum3 / sum4) / 2.0;
		movingIndex++;
	} while ((movingIndex + 1) <= result && movingIndex < imax - 1);

	level = (int)floor(result + 0.5);
	return level;
}

int NSE::Huang(const Mat& his)
{
	// Implements Huang's fuzzy thresholding method 
	// Uses Shannon's entropy function (one can also use Yager's entropy function) 
	// Huang L.-K. and Wang M.-J.J. (1995) "Image Thresholding by Minimizing  
	// the Measures of Fuzziness" Pattern Recognition, 28(1): 41-51
	// M. Emre Celebi  06.15.2007
	// Ported to ImageJ plugin by G. Landini from E Celebi's fourier_0.8 routines
	CV_Assert(his.type() == CV_32FC1);
	CV_Assert(his.size() == Size(1, 256));

	int threshold = -1;
	int ih, it;
	int first_bin;
	int last_bin;
	int sum_pix;
	int num_pix;
	double term;

	double ent;  // entropy 
	double min_ent; // min entropy 
	double mu_x;
	int length = his.rows*his.cols;
	/* Determine the first non-zero bin */
	first_bin = 0;
	for (ih = 0; ih < length; ih++) {
		if (his.at<float>(ih, 0) != 0) {
			first_bin = ih;
			break;
		}
	}

	/* Determine the last non-zero bin */
	last_bin = length - 1;
	for (ih = length - 1; ih >= first_bin; ih--) {
		if (his.at<float>(ih, 0) != 0) {
			last_bin = ih;
			break;
		}
	}
	term = 1.0 / (double)(last_bin - first_bin);
	//double[] mu_0 = new double[length];
	double *mu_0 = new double[length];
	sum_pix = num_pix = 0;
	for (ih = first_bin; ih < length; ih++) {
		sum_pix += ih * his.at<float>(ih, 0);
		num_pix += his.at<float>(ih, 0);
		/* NUM_PIX cannot be zero ! */
		mu_0[ih] = sum_pix / (double)num_pix;
	}

	double* mu_1 = new double[length];
	sum_pix = num_pix = 0;
	for (ih = last_bin; ih > 0; ih--) {
		sum_pix += ih * his.at<float>(ih, 0);
		num_pix += his.at<float>(ih, 0);
		/* NUM_PIX cannot be zero ! */
		mu_1[ih - 1] = sum_pix / (double)num_pix;
	}

	/* Determine the threshold that minimizes the fuzzy entropy */
	threshold = -1;
	min_ent = 1.79E+308;
	for (it = 0; it < length; it++) {
		ent = 0.0;
		for (ih = 0; ih <= it; ih++) {
			/* Equation (4) in Ref. 1 */
			mu_x = 1.0 / (1.0 + term * abs(ih - mu_0[it]));
			if (!((mu_x < 1e-06) || (mu_x > 0.999999))) {
				/* Equation (6) & (8) in Ref. 1 */
				ent += his.at<float>(ih, 0) * (-mu_x * log(mu_x) - (1.0 - mu_x) * log(1.0 - mu_x));
			}
		}

		for (ih = it + 1; ih < length; ih++) {
			/* Equation (4) in Ref. 1 */
			mu_x = 1.0 / (1.0 + term * abs(ih - mu_1[it]));
			if (!((mu_x < 1e-06) || (mu_x > 0.999999))) {
				/* Equation (6) & (8) in Ref. 1 */
				ent += his.at<float>(ih, 0) * (-mu_x * log(mu_x) - (1.0 - mu_x) * log(1.0 - mu_x));
			}
		}
		/* No need to divide by NUM_ROWS * NUM_COLS * LOG(2) ! */
		if (ent < min_ent) {
			min_ent = ent;
			threshold = it;
		}
	}
	return threshold;
}

bool NSE::bimodal(const Mat& y) {
	int len = y.cols*y.rows;
	bool b = false;
	int modes = 0;

	for (int k = 1; k < len - 1; k++) {
		if (y.at<float>(k - 1, 0) < y.at<float>(k, 0) && y.at<float>(k + 1, 0) < y.at<float>(k, 0)) {
			modes++;
			if (modes > 2)
				return false;
		}
	}
	if (modes == 2)
		b = true;
	return b;
}

int NSE::Intermodes(const Mat& his) {
	// J. M. S. Prewitt and M. L. Mendelsohn, "The analysis of cell images," in
	// Annals of the New York Academy of Sciences, vol. 128, pp. 1035-1053, 1966.
	// ported to ImageJ plugin by G.Landini from Antti Niemisto's Matlab code (GPL)
	// Original Matlab code Copyright (C) 2004 Antti Niemisto
	// See http://www.cs.tut.fi/~ant/histthresh/ for an excellent slide presentation
	// and the original Matlab code.
	//
	// Assumes a bimodal histogram. The histogram needs is smoothed (using a
	// running average of size 3, iteratively) until there are only two local maxima.
	// j and k
	// Threshold t is (j+k)/2.
	// Images with histograms having extremely unequal peaks or a broad and
	// ﬂat valley are unsuitable for this method.
	int iter = 0;
	int threshold = -1;
	Mat t_his = his.clone();
	Mat i_his = his.clone();
	while (!bimodal(i_his)) {
		//smooth with a 3 point running mean filter
		for (int i = 1; i < 255; i++)
			t_his.at<float>(i, 0) = (i_his.at<float>(i - 1, 0) + i_his.at<float>(i, 0) + i_his.at<float>(i + 1, 0)) / 3;
		t_his.at<float>(0, 0) = (i_his.at<float>(0, 0) + i_his.at<float>(1, 0)) / 3; //0 outside
		t_his.at<float>(255, 0) = (i_his.at<float>(254, 0) + i_his.at<float>(255, 0)) / 3; //0 outside
		i_his = t_his;
		iter++;
		if (iter > 10000) {
			threshold = -1;
			return threshold;
		}
	}

	// The threshold is the mean between the two peaks.
	int tt = 0;
	for (int i = 1; i < 255; i++) {
		if (i_his.at<float>(i - 1, 0) < i_his.at<float>(i, 0) && i_his.at<float>(i + 1, 0) < i_his.at<float>(i, 0)) {
			tt += i;
		}
	}
	threshold = (int)floor(tt / 2.0);
	return threshold;
}

int NSE::Li(const Mat& his)
{
	// Implements Li's Minimum Cross Entropy thresholding method
	// This implementation is based on the iterative version (Ref. 2) of the algorithm.
	// 1) Li C.H. and Lee C.K. (1993) "Minimum Cross Entropy Thresholding" 
	//    Pattern Recognition, 26(4): 617-625
	// 2) Li C.H. and Tam P.K.S. (1998) "An Iterative Algorithm for Minimum 
	//    Cross Entropy Thresholding"Pattern Recognition Letters, 18(8): 771-776
	// 3) Sezgin M. and Sankur B. (2004) "Survey over Image Thresholding 
	//    Techniques and Quantitative Performance Evaluation" Journal of 
	//    Electronic Imaging, 13(1): 146-165 
	//    http://citeseer.ist.psu.edu/sezgin04survey.html
	// Ported to ImageJ plugin by G.Landini from E Celebi's fourier_0.8 routines
	CV_Assert(his.type() == CV_32FC1);
	CV_Assert(his.size() == Size(1, 256));

	int length = his.cols*his.rows;
	int threshold;
	int64 ih;
	int64 num_pixels;
	int64 sum_back; /* sum of the background pixels at a given threshold */
	int64 sum_obj;  /* sum of the object pixels at a given threshold */
	int64 num_back; /* number of background pixels at a given threshold */
	int64 num_obj;  /* number of object pixels at a given threshold */
	double old_thresh;
	double new_thresh;
	double mean_back; /* mean of the background pixels at a given threshold */
	double mean_obj;  /* mean of the object pixels at a given threshold */
	double mean;  /* mean gray-level in the image */
	double tolerance; /* threshold tolerance */
	double temp;


	int ithreshold = -1;
	int64 itot = 0, isum = 0;
	for (int i = 0; i < length; i++) {
		itot += his.at<float>(i, 0);
		isum += ((int64)i* (int64)his.at<float>(i, 0));
	}
	ithreshold = (int)floor((double)isum / (double)itot);


	tolerance = 0.5;
	num_pixels = 0;
	for (ih = 0; ih < length; ih++)
		num_pixels += his.at<float>(ih, 0);

	/* Calculate the mean gray-level */
	mean = 0.0;
	for (ih = 0; ih < length; ih++) //0 + 1?
		mean += ih * his.at<float>(ih, 0);
	mean /= num_pixels;
	/* Initial estimate */
	new_thresh = mean;

	do {
		old_thresh = new_thresh;
		threshold = (int)(old_thresh + 0.5);	/* range */
												/* Calculate the means of background and object pixels */
												/* Background */
		sum_back = 0;
		num_back = 0;
		for (ih = 0; ih <= threshold; ih++) {
			sum_back += ih * his.at<float>(ih, 0);
			num_back += his.at<float>(ih, 0);
		}
		mean_back = (num_back == 0 ? 0.0 : (sum_back / (double)num_back));
		/* Object */
		sum_obj = 0;
		num_obj = 0;
		for (ih = threshold + 1; ih < length; ih++) {
			sum_obj += ih * his.at<float>(ih, 0);
			num_obj += his.at<float>(ih, 0);
		}
		mean_obj = (num_obj == 0 ? 0.0 : (sum_obj / (double)num_obj));

		/* Calculate the new threshold: Equation (7) in Ref. 2 */
		temp = (mean_back - mean_obj) / (log(mean_back) - log(mean_obj));

		if (temp < -2.220446049250313E-16)
			new_thresh = (int)(temp - 0.5);
		else
			new_thresh = (int)(temp + 0.5);
		/*  Stop the iterations when the difference between the
		new and old threshold values is less than the tolerance */
	} while (abs(new_thresh - old_thresh) > tolerance);
	return threshold;
}

int NSE::Mean(const Mat& his)
{
	// C. A. Glasbey, "An analysis of histogram-based thresholding algorithms,"
	// CVGIP: Graphical Models and Image Processing, vol. 55, pp. 532-537, 1993.
	//
	// The threshold is the mean of the greyscale data
	CV_Assert(his.type() == CV_32FC1);
	CV_Assert(his.size() == Size(1, 256));

	int threshold = -1;
	double tot = 0, sum = 0;
	for (int i = 0; i < 256; i++) {
		tot += his.at<float>(i, 0);
		sum += (i*his.at<float>(i, 0));
	}
	threshold = (int)floor(sum / tot);
	return threshold;
}

int NSE::Yen(const Mat& his)
{
	CV_Assert(his.type() == CV_32FC1);
	CV_Assert(his.size() == Size(1, 256));
	int th, ih, it;
	double crit, max_crit;
	Mat temp(Size(1, 256), CV_32FC1, Scalar(0));
	Mat norm_his(Size(1, 256), CV_32FC1, Scalar(0));
	Mat P1(Size(1, 256), CV_32FC1, Scalar(0));
	Mat P1_sq(Size(1, 256), CV_32FC1, Scalar(0));
	Mat P2_sq(Size(1, 256), CV_32FC1, Scalar(0));

	int total = 0;
	for (ih = 0; ih < 256; ih++) total += his.at<float>(ih, 0);
	for (ih = 0; ih < 256; ih++) norm_his.at<float>(ih, 0) = (float)his.at<float>(ih, 0) / total;
	P1_sq.at<float>(0, 0) = norm_his.at<float>(0, 0);
	for (ih = 1; ih < 256; ih++) P1.at<float>(ih, 0) = P1.at<float>(ih - 1, 0) + norm_his.at<float>(ih, 0);
	P1_sq.at<float>(0, 0) = norm_his.at<float>(0, 0)*norm_his.at<float>(0, 0);
	for (ih = 1; ih < 256; ih++) P1_sq.at<float>(ih, 0) = P1_sq.at<float>(ih - 1, 0) + norm_his.at<float>(0, 0)*norm_his.at<float>(0, 0);
	P2_sq.at<float>(255, 0) = 0;
	for (ih = 254; ih >= 0; ih--) P2_sq.at<float>(ih, 0) = P2_sq.at<float>(ih + 1, 0) + norm_his.at<float>(ih + 1, 0)*norm_his.at<float>(ih + 1, 0);
	th = -1;
	max_crit = 49e-324;
	for (it = 0; it < 256; it++) {
		crit = -1.0*((P1_sq.at<float>(it, 0)*P2_sq.at<float>(it, 0) > 0.0 ? log(P1_sq.at<float>(it, 0)*P2_sq.at<float>(it, 0)) : 0.0)
			+ 2 * ((P1.at<float>(it, 0)*(1.0 - P1.at<float>(it, 0)) > 0.0 ? log(P1.at<float>(it, 0)*(1.0 - P1.at<float>(it, 0))) : 0.0)));
		if (crit > max_crit) {
			max_crit = crit;
			th = it;
		}
	}
	return th;
}

int NSE::Triangle(const Mat& his) 
{
	//  Zack, G. W., Rogers, W. E. and Latt, S. A., 1977,
	//  Automatic Measurement of Sister Chromatid Exchange Frequency,
	// Journal of Histochemistry and Cytochemistry 25 (7), pp. 741-753
	//
	//  modified from Johannes Schindelin plugin
	// 
	// find min and max
	CV_Assert(his.type() == CV_32FC1);
	CV_Assert(his.size() == Size(1, 256));
	int length = his.rows*his.cols;
	int min = 0, dmax = 0, max = 0, min2 = 0;
	Mat Temphis = his.clone();
	for (int i = 0; i < length; i++) {
		if (Temphis.at<float>(i, 0) > 0) {
			min = i;
			break;
		}
	}
	if (min > 0) min--; // line to the (p==0) point, not to data[min]

					  // The Triangle algorithm cannot tell whether the data is skewed to one side or another.
					  // This causes a problem as there are 2 possible thresholds between the max and the 2 extremes
					  // of the histogram.
					  // Here I propose to find out to which side of the max point the data is furthest, and use that as
					  //  the other extreme.
	for (int i = 255; i > 0; i--) {
		if (Temphis.at<float>(i, 0) > 0) {
			min2 = i;
			break;
		}
	}
	if (min2 < 255) min2++; // line to the (p==0) point, not to data[min]

	for (int i = 0; i < 256; i++) {
		if (Temphis.at<float>(i, 0) > dmax) {
			max = i;
			dmax = Temphis.at<float>(i, 0);
		}
	}
	// find which is the furthest side
	//IJ.log(""+min+" "+max+" "+min2);
	bool inverted = false;
	if ((max - min) < (min2 - max)) {
		// reverse the histogram
		//IJ.log("Reversing histogram.");
		inverted = true;
		int left = 0;          // index of leftmost element
		int right = 255; // index of rightmost element
		while (left < right) {
			// exchange the left and right elements
			int temp = his.at<float>(left, 0);
			Temphis.at<float>(left, 0) = Temphis.at<float>(right, 0);
			Temphis.at<float>(right, 0) = temp;
			// move the bounds toward the center
			left++;
			right--;
		}
		min = 255 - min2;
		max = 255 - max;
	}

	if (min == max) {
		//IJ.log("Triangle:  min == max.");
		return min;
	}

	// describe line by nx * x + ny * y - d = 0
	double nx, ny, d;
	// nx is just the max frequency as the other point has freq=0
	nx = Temphis.at<float>(max, 0);   //-min; // data[min]; //  lowest value bmin = (p=0)% in the image
	ny = min - max;
	d = sqrt(nx * nx + ny * ny);
	nx /= d;
	ny /= d;
	d = nx * min + ny * Temphis.at<float>(min, 0);

	// find split point
	int split = min;
	double splitDistance = 0;
	for (int i = min + 1; i <= max; i++) {
		double newDistance = nx * i + ny * Temphis.at<float>(i, 0) - d;
		if (newDistance > splitDistance) {
			split = i;
			splitDistance = newDistance;
		}
	}
	split--;

	if (inverted) {
		// The histogram might be used for something else, so let's reverse it back
		int left = 0;
		int right = 255;
		while (left < right) {
			int temp = Temphis.at<float>(left, 0);
			Temphis.at<float>(left, 0) = Temphis.at<float>(right, 0);
			Temphis.at<float>(right, 0) = temp;
			left++;
			right--;
		}
		return (255 - split);
	}
	else
		return split;
}

cv::Mat NSE::Threshold(const cv::Mat& src, double threshold, double threshMaxVal, cv::ThresholdTypes type)
{
	Mat dst;
	CV_TRY_CATCH_NOEXCEPTION(cv::threshold(src, dst, threshold, threshMaxVal, type));
	return dst;
}

cv::Mat NSE::Threshold(const Mat &src, NSE::AutoThTypes autoTh, double threshMaxVal, cv::ThresholdTypes type)
{
	Mat dstImg;

	if (autoTh == NSE::AutoThTypes::OTSU) {
		CV_TRY_CATCH_NOEXCEPTION(cv::threshold(src, dstImg, 0, threshMaxVal, THRESH_BINARY | THRESH_OTSU));
	}
	else {
		Mat hist = CalcGrayHist(src);

		int thVal = 0;

		/*ISODATA = 0,
		HUANG,
		INTERMODES,
		LI,
		MEAN,
		YEN,*/
		switch(autoTh){
			case ISODATA:
				thVal = NSE::IsoData(hist);
				break;
			case HUANG:
				thVal = NSE::Huang(hist);
				break;
			case INTERMODES:
				thVal = NSE::Intermodes(hist);
				break;
			case LI:
				thVal = NSE::Li(hist);
				break;
			case MEAN:
				thVal = NSE::Mean(hist);
				break;
			case YEN:
				thVal = NSE::Yen(hist);
				break;
		}

		threshold(src, dstImg, thVal, threshMaxVal, type);
	}

	return dstImg;
}

cv::Mat NSE::AdaptiveThreshold(const Mat& src, double maxVal, cv::AdaptiveThresholdTypes adaptType, cv::ThresholdTypes type, int nBlockSize, double c)
{
	Mat img;
	if(src.channels() == 3)
		cv::cvtColor(src, img, CV_8UC1);
	else
		img= src.clone();

	Mat m_adaptive;
	cv::adaptiveThreshold(img, m_adaptive, maxVal, adaptType, type, nBlockSize, c);

	return m_adaptive;
}

cv::Mat NSE::ThresholdMinMax(const Mat& src, int min, int max)
{
	Mat img;
	img = src.clone();

	uchar* pt = img.data;

	for(auto y = 0; y < img.rows; y++)
		for(auto x = 0; x < img.cols; x++)
		{
			if(pt[img.cols * y + x] >= min && pt[img.cols * y + x] <= max)
				pt[img.cols * y + x] = 255;
				//pt[img.cols * y + x] = 210;
			else
				pt[img.cols * y + x] = 0;
		}

	return img;
}

cv::Mat NSE::ThresholdMinMax16(const Mat& src, int min, int max)
{
	Mat img;
	img = src.clone();

	for (int y = 0; y < img.rows; y++) {
		ushort* pt = img.ptr<ushort>(y);
		for (int x = 0; x < img.cols; x++) {
			if(pt[x] >= min && pt[x] <= max)
				pt[x] = 65535;
			else
				pt[x] = 0;
		}
	}

	return img;
}

cv::Mat NSE::ThresholdMinMax16_2(const Mat& src, int min, int max, int val, double& dbObjMean)
{
	Mat img;
	img = src.clone();
	std::vector<double> vPixel;

	for (int y = 0; y < img.rows; y++) {
		ushort* pt = img.ptr<ushort>(y);
		for (int x = 0; x < img.cols; x++) {
			if(pt[x] >= min && pt[x] <= max)
			{
				vPixel.push_back(pt[x]);
			}
			else
				pt[x] = saturate_cast<uchar>(val);
		}
	}

	dbObjMean = std::accumulate(vPixel.begin(), vPixel.end(), 0.0) / vPixel.size();

	return img;
}

cv::Mat NSE::NSECond1(cv::Mat& src, int nTh)
{
	//Mat m_BadImg(src.size(), src.type(), Scalar(0));
	Mat m_BadImg(src.size(), CV_8UC3, Scalar(0,0,0));
	uchar* ptBad = m_BadImg.data;

	// 검사 범위
	int nExp = 1;

	uchar* pt = src.data;
	for (auto y = nExp; y < src.rows - nExp; y++)
	{
		for (auto x = nExp; x < src.cols - nExp; x++)
		{
			if (nTh <= 255 - pt[src.cols * y + x] && nTh != 0) {
				ptBad[m_BadImg.cols * y * 3 + x * 3] = 0;
				ptBad[m_BadImg.cols * y * 3 + x * 3 + 1] = 0;
				ptBad[m_BadImg.cols * y * 3 + x * 3 + 2] = 255;
			}
		}
	}

	return m_BadImg;
}

cv::Mat NSE::NSECond2(cv::Mat& src, int nTh)
{
	int nType = 0;	// Not Bulk = 0, Bulk = 1

	Mat m_BadImg(src.size(), CV_8UC3, Scalar(0));
	uchar* ptBad = m_BadImg.data;

	int a1, a2, a3, a4;
	int nArray = 16;
	int m_Temp[16];

	// 검사 범위
	int nExp = 2;

	uchar* pt = src.data;
	for (auto y = nExp + 1; y < src.rows - (nExp + 1); y++)
	{
		for (auto x = nExp + 1; x < src.cols - (nExp + 1); x++)
		{
			if (nType == 0) 
			{
				if (pt[src.cols * y + x] < 160 &&
					pt[src.cols * (y - nExp) + (x - nExp)] < 180 && pt[src.cols * (y - nExp) + x] < 180 && pt[src.cols * (y - nExp) + (x + nExp)] < 180 &&
					pt[src.cols * y + (x - nExp)] < 180 && pt[src.cols * y + (x + nExp)] < 180 &&
					pt[src.cols * (y + nExp) + (x - nExp)] < 180 && pt[src.cols * (y + nExp) + x] < 180 && pt[src.cols * (y + nExp) + (x + nExp)] < 180)
				{
					a4 = pt[src.cols * y + x];

					int nCnt = 0;
					for (auto a = 0 - nExp; a <= nExp; a++)
					{
						m_Temp[nCnt] = pt[src.cols * (y + a) + (x - nExp)] - a4;
						nCnt++;
						m_Temp[nCnt] = pt[src.cols * (y + a) + (x + nExp)] - a4;
						nCnt++;
						if (a < nExp - 1) {
							m_Temp[nCnt] = pt[src.cols * (y - nExp) + (x + a + 1)] - a4;
							nCnt++;
							m_Temp[nCnt] = pt[src.cols * (y + nExp) + (x + a + 1)] - a4;
							nCnt++;
						}
					}

					int nTotal = 0;
					for (auto z = 0; z < nArray; z++)
						nTotal += m_Temp[z];

					a1 = nTotal / nArray;

					a2 = 200 - a4;
					a3 = int((a1 * 3) + (a2 / 10) * 3);

					// 올림차순 정렬
					if (a3 >= nTh - 20) {
						int tmp = 0, max = 0;
						for(auto a=0;a<nArray-1;a++)
							for (auto b = a + 1; b < nArray; b++) {
								if (m_Temp[b] < m_Temp[a])
								{
									tmp = m_Temp[b];
									m_Temp[b] = m_Temp[a];
									m_Temp[a] = tmp;
								}
							}

						max = m_Temp[1] + m_Temp[3] + m_Temp[4];
						max = max / 3;

						a3 = int((max * 3) + ((a2 / 10) * 3));

						a3 > 255 ? a3 = 255 : a3 = a3;

						if (a3 > nTh && nTh != 0) {
							ptBad[m_BadImg.cols * y * 3 + x * 3] = 0;
							ptBad[m_BadImg.cols * y * 3 + x * 3 + 1] = 128;
							ptBad[m_BadImg.cols * y * 3 + x * 3 + 2] = 255;
						}
					}
				}
			}
		}
	}

	return m_BadImg;
}

cv::Mat NSE::NSECond3(cv::Mat& src, int nTh)
{
	int nType = 0;	// Not Bulk = 0, Bulk = 1

	Mat m_BadImg(src.size(), CV_8UC3, Scalar(0));
	uchar* ptBad = m_BadImg.data;

	int a1, a2, a3, a4;
	int nArray = 48;
	int m_Temp[48];

	// 검사 범위
	int nExp = 6;

	uchar* pt = src.data;
	for (auto y = nExp + 1; y < src.rows - (nExp + 1); y++)
	{
		for (auto x = nExp + 1; x < src.cols - (nExp + 1); x++)
		{
			if (nType == 0)
			{
				if (pt[src.cols * y + x] < 200 &&
					pt[src.cols * (y - nExp) + (x - nExp)] < 240 && pt[src.cols * (y - nExp) + x] < 240 && pt[src.cols * (y - nExp) + (x + nExp)] < 240 &&
					pt[src.cols * y + (x - nExp)] < 240 && pt[src.cols * y + (x + nExp)] < 240 &&
					pt[src.cols * (y + nExp) + (x - nExp)] < 240 && pt[src.cols * (y + nExp) + x] < 240 && pt[src.cols * (y + nExp) + (x + nExp)] < 240)
				{
					a4 = pt[src.cols * (y - 1) + (x - 1)] + pt[src.cols * (y - 1) + (x)] + pt[src.cols * (y - 1) + (x + 1)] +
						pt[src.cols * (y) + (x - 1)] + pt[src.cols * (y) + (x)] + pt[src.cols * (y - 1) + (x + 1)] +
						pt[src.cols * (y - 1) + (x + 1)] + pt[src.cols * (y) + (x + 1)] + pt[src.cols * (y + 1) + (x + 1)];
					a4 = a4 / 9;

					int nCnt = 0;
					for (auto a = 0 - nExp; a <= nExp; a++)
					{
						m_Temp[nCnt] = pt[src.cols * (y + a) + (x - nExp)] - a4;
						nCnt++;
						m_Temp[nCnt] = pt[src.cols * (y + a) + (x + nExp)] - a4;
						nCnt++;
						if (a < nExp - 1) {
							m_Temp[nCnt] = pt[src.cols * (y - nExp) + (x + a + 1)] - a4;
							nCnt++;
							m_Temp[nCnt] = pt[src.cols * (y + nExp) + (x + a + 1)] - a4;
							nCnt++;
						}
					}

					int nTotal = 0;
					for (auto z = 0; z < nArray; z++)
						nTotal += m_Temp[z];

					a1 = nTotal / nArray;

					a2 = (160 - a4) * 2 / 2.5;
					a3 = int((a1 * 3) + a2);

					// 올림차순 정렬
					if (a3 >= nTh - 20) {
						int tmp = 0, max = 0;
						for (auto a = 0; a < nArray - 1; a++)
							for (auto b = a + 1; b < nArray; b++) {
								if (m_Temp[b] < m_Temp[a])
								{
									tmp = m_Temp[b];
									m_Temp[b] = m_Temp[a];
									m_Temp[a] = tmp;
								}
							}

						max = m_Temp[1] + m_Temp[8] + m_Temp[12];
						max = max / 3;

						a3 = int((max * 3) + a2);

						a3 > 255 ? a3 = 255 : a3 = a3;

						if (a3 > nTh && nTh != 0){
							ptBad[m_BadImg.cols * y * 3 + x * 3] = 0;
							ptBad[m_BadImg.cols * y * 3 + x * 3 + 1] = 255;
							ptBad[m_BadImg.cols * y * 3 + x * 3 + 2] = 255;
						}
					}
				}
			}
		}
	}

	return m_BadImg;
}

cv::Mat NSE::NSECond4(cv::Mat& src, int nTh)
{
	int nType = 0;	// Not Bulk = 0, Bulk = 1

	Mat m_BadImg(src.size(), CV_8UC3, Scalar(0));
	uchar* ptBad = m_BadImg.data;

	int a1, a2, a3, a4;
	int nArray = 80;
	int m_Temp[80];

	// 검사 범위
	int nExp = 10;

	uchar* pt = src.data;
	for (auto y = nExp + 1; y < src.rows - (nExp + 1); y++)
	{
		for (auto x = nExp + 1; x < src.cols - (nExp + 1); x++)
		{
			if (nType == 0)
			{
				if (pt[src.cols * y + x] < 200 &&
					pt[src.cols * (y - nExp) + (x - nExp)] < 240 && pt[src.cols * (y - nExp) + x] < 240 && pt[src.cols * (y - nExp) + (x + nExp)] < 240 &&
					pt[src.cols * y + (x - nExp)] < 240 && pt[src.cols * y + (x + nExp)] < 240 &&
					pt[src.cols * (y + nExp) + (x - nExp)] < 240 && pt[src.cols * (y + nExp) + x] < 240 && pt[src.cols * (y + nExp) + (x + nExp)] < 240)
				{
					a4 = pt[src.cols * (y - 2) + (x - 2)] + pt[src.cols * (y - 2) + (x)] + pt[src.cols * (y - 2) + (x + 2)] +
						pt[src.cols * (y)+(x - 2)] + pt[src.cols * (y)+(x)] + pt[src.cols * (y - 2) + (x + 2)] +
						pt[src.cols * (y - 2) + (x + 2)] + pt[src.cols * (y)+(x + 2)] + pt[src.cols * (y + 2) + (x + 2)];
					a4 = a4 / 9;

					int nCnt = 0;
					for (auto a = 0 - nExp; a <= nExp; a++)
					{
						m_Temp[nCnt] = pt[src.cols * (y + a) + (x - nExp)] - a4;
						nCnt++;
						m_Temp[nCnt] = pt[src.cols * (y + a) + (x + nExp)] - a4;
						nCnt++;
						if (a < nExp - 1) {
							m_Temp[nCnt] = pt[src.cols * (y - nExp) + (x + a + 1)] - a4;
							nCnt++;
							m_Temp[nCnt] = pt[src.cols * (y + nExp) + (x + a + 1)] - a4;
							nCnt++;
						}
					}

					int nTotal = 0;
					for (auto z = 0; z < nArray; z++)
						nTotal += m_Temp[z];

					a1 = nTotal / nArray;

					a2 = 200 - a4;
					a3 = int((a1 * 1.5) + (a2 / 10) * 5);

					// 올림차순 정렬
					if (a3 >= nTh - 20) {
						int tmp = 0, max = 0;
						for (auto a = 0; a < nArray - 1; a++)
							for (auto b = a + 1; b < nArray; b++) {
								if (m_Temp[b] < m_Temp[a])
								{
									tmp = m_Temp[b];
									m_Temp[b] = m_Temp[a];
									m_Temp[a] = tmp;
								}
							}

						max = m_Temp[3] + m_Temp[10] + m_Temp[20];
						max = max / 3;

						a3 = int((max * 1.5) + (a2 / 10) * 5);

						a3 > 255 ? a3 = 255 : a3 = a3;

						if (a3 > nTh && nTh != 0) {
							ptBad[m_BadImg.cols * y * 3 + x * 3] = 0;
							ptBad[m_BadImg.cols * y * 3 + x * 3 + 1] = 255;
							ptBad[m_BadImg.cols * y * 3 + x * 3 + 2] = 0;
						}
					}
				}
			}
		}
	}

	return m_BadImg;
}

cv::Mat NSE::NSECond5(cv::Mat& src, int nTh)
{
	int nType = 0;	// Not Bulk = 0, Bulk = 1

	Mat m_BadImg(src.size(), CV_8UC3, Scalar(0));
	uchar* ptBad = m_BadImg.data;

	int a1, a2, a3, a4;
	int nArray = 40;
	int m_Temp[40];

	// 검사 범위
	int nExp = 5;

	uchar* pt = src.data;
	for (auto y = nExp + 1; y < src.rows - (nExp + 1); y++)
	{
		for (auto x = nExp + 1; x < src.cols - (nExp + 1); x++)
		{
			if (nType == 0)
			{
				if (pt[src.cols * y + x] < 230)
					{
					a4 = pt[src.cols * (y - 1) + (x - 1)] + pt[src.cols * (y - 1) + (x)] + pt[src.cols * (y - 1) + (x + 1)] +
						pt[src.cols * (y)+(x - 1)] + pt[src.cols * (y)+(x)] + pt[src.cols * (y - 1) + (x + 1)] +
						pt[src.cols * (y - 1) + (x + 1)] + pt[src.cols * (y)+(x + 1)] + pt[src.cols * (y + 1) + (x + 1)];
					a4 = a4 / 9;

					int nCnt = 0;
					for (auto a = 0 - nExp; a <= nExp; a++)
					{
						m_Temp[nCnt] = pt[src.cols * (y + a) + (x - nExp)] - a4;
						nCnt++;
						m_Temp[nCnt] = pt[src.cols * (y + a) + (x + nExp)] - a4;
						nCnt++;
						if (a < nExp - 1) {
							m_Temp[nCnt] = pt[src.cols * (y - nExp) + (x + a + 1)] - a4;
							nCnt++;
							m_Temp[nCnt] = pt[src.cols * (y + nExp) + (x + a + 1)] - a4;
							nCnt++;
						}
					}

					int nTotal = 0;
					for (auto z = 0; z < nArray; z++)
						nTotal += m_Temp[z];

					a1 = nTotal / nArray;

					a2 = ((160 - a4) * 2) / 6;
					a3 = int((a1 * 4) + a2);

					// 올림차순 정렬
					if (a3 >= nTh - 20) {
						int tmp = 0, max = 0;
						for (auto a = 0; a < nArray - 1; a++)
							for (auto b = a + 1; b < nArray; b++) {
								if (m_Temp[b] < m_Temp[a])
								{
									tmp = m_Temp[b];
									m_Temp[b] = m_Temp[a];
									m_Temp[a] = tmp;
								}
							}

						max = m_Temp[15] + m_Temp[14] + m_Temp[13] + m_Temp[12] + m_Temp[11] + m_Temp[10];
						max = max / 6;

						a3 = int((max * 4) + a2);

						a3 > 255 ? a3 = 255 : a3 = a3;

						if (a3 > nTh && nTh != 0) {
							ptBad[m_BadImg.cols * y * 3 + x * 3] = 255;
							ptBad[m_BadImg.cols * y * 3 + x * 3 + 1] = 255;
							ptBad[m_BadImg.cols * y * 3 + x * 3 + 2] = 0;
						}
					}
				}
			}
		}
	}

	return m_BadImg;
}

cv::Mat NSE::NSECond6(cv::Mat& src, int nTh)
{
	int nType = 0;	// Not Bulk = 0, Bulk = 1

	Mat m_BadImg(src.size(), CV_8UC3, Scalar(0));
	uchar* ptBad = m_BadImg.data;

	int a1, a2, a3, a4;
	int nArray = 96;
	int m_Temp[96];

	// 검사 범위
	int nExp = 12;

	uchar* pt = src.data;
	for (auto y = nExp + 1; y < src.rows - (nExp + 1); y++)
	{
		for (auto x = nExp + 1; x < src.cols - (nExp + 1); x++)
		{
			if (nType == 0)
			{
				if (pt[src.cols * y + x] < 200 &&
					pt[src.cols * (y - nExp) + (x - nExp)] < 240 && pt[src.cols * (y - nExp) + x] < 240 && pt[src.cols * (y - nExp) + (x + nExp)] < 240 &&
					pt[src.cols * y + (x - nExp)] < 240 && pt[src.cols * y + (x + nExp)] < 240 &&
					pt[src.cols * (y + nExp) + (x - nExp)] < 240 && pt[src.cols * (y + nExp) + x] < 240 && pt[src.cols * (y + nExp) + (x + nExp)] < 240)
				{
					a4 = pt[src.cols * (y - 2) + (x - 2)] + pt[src.cols * (y - 2) + (x)] + pt[src.cols * (y - 2) + (x + 2)] +
						pt[src.cols * (y)+(x - 2)] + pt[src.cols * (y)+(x)] + pt[src.cols * (y - 2) + (x + 2)] +
						pt[src.cols * (y - 2) + (x + 2)] + pt[src.cols * (y)+(x + 2)] + pt[src.cols * (y + 2) + (x + 2)];
					a4 = a4 / 9;

					int nCnt = 0;
					for (auto a = 0 - nExp; a <= nExp; a++)
					{
						m_Temp[nCnt] = pt[src.cols * (y + a) + (x - nExp)] - a4;
						nCnt++;
						m_Temp[nCnt] = pt[src.cols * (y + a) + (x + nExp)] - a4;
						nCnt++;
						if (a < nExp - 1) {
							m_Temp[nCnt] = pt[src.cols * (y - nExp) + (x + a + 1)] - a4;
							nCnt++;
							m_Temp[nCnt] = pt[src.cols * (y + nExp) + (x + a + 1)] - a4;
							nCnt++;
						}
					}

					int nTotal = 0;
					for (auto z = 0; z < nArray; z++)
						nTotal += m_Temp[z];

					a1 = nTotal / nArray;

					a2 = 200 - a4;
					a3 = int((a1 * 1.5) + (a2 / 10) * 5);

					// 올림차순 정렬
					if (a3 >= nTh - 20) {
						int tmp = 0, max = 0;
						for (auto a = 0; a < nArray - 1; a++)
							for (auto b = a + 1; b < nArray; b++) {
								if (m_Temp[b] < m_Temp[a])
								{
									tmp = m_Temp[b];
									m_Temp[b] = m_Temp[a];
									m_Temp[a] = tmp;
								}
							}

						max = m_Temp[3] + m_Temp[10] + m_Temp[20];
						max = max / 3;

						a3 = int((max * 1.5) + (a2 / 10) * 5);

						a3 > 255 ? a3 = 255 : a3 = a3;

						if (a3 > nTh && nTh != 0) {
							ptBad[m_BadImg.cols * y * 3 + x * 3] = 255;
							ptBad[m_BadImg.cols * y * 3 + x * 3 + 1] = 0;
							ptBad[m_BadImg.cols * y * 3 + x * 3 + 2] = 0;
						}
					}
				}
			}
		}
	}

	return m_BadImg;
}

cv::Mat NSE::ImgBuftoMat(unsigned int **Img_Buf, int w, int h)
{
	int rows = h;
	int cols = w;

	Mat mImg(cols, rows, CV_8UC1);
	uchar* pdata = mImg.data;
	for(int y = 0; y < mImg.rows; y++)
		for(int x = 0; x < mImg.cols; x++){
			 pdata[mImg.cols * y + x] = Img_Buf[y][x];
		}

	return mImg;
}

cv::Mat NSE::ImgBuftoMatchar(unsigned char **Img_Buf, int w, int h)
{
	int rows = h;
	int cols = w;

	Mat mImg(cols, rows, CV_8UC1);
	uchar* pdata = mImg.data;
	for(int y = 0; y < mImg.rows; y++)
		for(int x = 0; x < mImg.cols; x++){
			 pdata[mImg.cols * y + x] = Img_Buf[y][x];
		}

	return mImg;
}

cv::Mat NSE::ImgBuftoMat16(unsigned int **Img_Buf, int w, int h)
{
	int rows = h;
	int cols = w;

	Mat mImg(rows, cols, CV_16UC1);	// 16bit
	for (int y = 0; y < mImg.rows; y++) {
		ushort* pt = mImg.ptr<ushort>(y);
		for (int x = 0; x < mImg.cols; x++) {
			pt[x] = Img_Buf[y][x];
		}
	}

	return mImg;
}

cv::Mat NSE::ImgBuftoMat16_re(unsigned int **Img_Buf, int w, int h, int re)
{
	int rows = h;
	int cols = w;

	Mat mImg(rows, cols, CV_16UC1);	// 16bit
	for (int y = 0; y < mImg.rows; y++) {
		ushort* pt = mImg.ptr<ushort>(y);
		for (int x = 0; x < mImg.cols; x++) {
			pt[x] = Img_Buf[y * re][x * re];
		}
	}

	return mImg;
}

unsigned int** NSE::MattoImgBuf(const cv::Mat src)
{
	unsigned int **Img_Buf;
	Img_Buf = new unsigned int *[src.rows];

	for(int i = 0;i < src.rows; i++)
		Img_Buf[i] = new unsigned int [src.cols];

	uchar* pt = src.data;
	for(int row = 0; row < src.rows; row++)
		for(int col = 0; col < src.cols; col++){
			Img_Buf[row][col] = pt[src.cols * row + col];
		}

	return Img_Buf;
}

unsigned char** NSE::MattoImgBufchar(const cv::Mat src)
{
	unsigned char **Img_Buf;
	Img_Buf = new unsigned char *[src.rows];

	for(int i = 0;i < src.rows; i++)
		Img_Buf[i] = new unsigned char [src.cols];

	uchar* pt = src.data;
	for(int row = 0; row < src.rows; row++)
		for(int col = 0; col < src.cols; col++){
			Img_Buf[row][col] = pt[src.cols * row + col];
		}

	return Img_Buf;
}

unsigned int** NSE::MattoImgBuf16(const cv::Mat src)
{
	cv::Mat tmp;
	src.convertTo(tmp, CV_16UC1);

	unsigned int **Img_Buf;
	Img_Buf = new unsigned int *[src.rows];

	for(int i = 0;i < tmp.rows; i++)
		Img_Buf[i] = new unsigned int [tmp.cols];

	for (int y = 0; y < tmp.rows; y++) {
		ushort* pt = tmp.ptr<ushort>(y);
		for (int x = 0; x < tmp.cols; x++) {
			Img_Buf[y][x] = pt[x];
		}
	}

	return Img_Buf;
}

cv::Mat NSE::ConvertToColorPx(const cv::Mat& src, int nVal, Scalar sc)
{
	cv::Mat m_Input = src.clone();

	if(m_Input.channels() == 3)	return m_Input;

	cv::Mat m_Color;
	cvtColor(m_Input, m_Color, COLOR_GRAY2BGR);

	for(auto x = 0; x < m_Color.cols; x++)
		for(auto y = 0; y < m_Color.rows; y++)
		{
			if(m_Input.at<uchar>(y, x) == nVal)
			{
				m_Color.at<Vec3b>(y, x)[0] = sc[0];
				m_Color.at<Vec3b>(y, x)[1] = sc[1];
				m_Color.at<Vec3b>(y, x)[2] = sc[2];
			}
		}

	return m_Color;
}

void NSE::MinMaxAvg(std::vector<int> vPx, int &min, int &max, double &avg)
{
	min = *min_element(vPx.begin(), vPx.end());
	max = *max_element(vPx.begin(), vPx.end());

	int sum = 0;
	for(auto iter = vPx.begin(); iter != vPx.end(); iter++)
		sum += *iter;

	avg = sum / vPx.size();
}

double NSE::StandardDevitation(std::vector<int> vPx, double avg)
{
	int size = vPx.size();
	if(size < 2)
		return std::sqrt(-1.0);

	double sum = 0.0;

	for(auto i = 0; i < size; i++){
		double diff = vPx[i] - avg;
		sum += diff * diff;
	}

	return std::sqrt(sum / (size - 1));
}

double NSE::StandardDevitation(std::vector<int> vPx)
{
	int min, max;
	double avg;

	MinMaxAvg(vPx, min, max, avg);

	int size = vPx.size();
	if(size < 2)
		return std::sqrt(-1.0);

	double sum = 0.0;

	for(auto i = 0; i < size; i++){
		double diff = vPx[i] - avg;
		sum += diff * diff;
	}

	return std::sqrt(sum / (size - 1));
}

cv::Mat NSE::Normalization(cv::Mat& src, double min, double max)
{
	cv::Mat m_Input = src.clone();

	cv::Mat m_Normal;

	if(max > 255)
		cv::normalize(m_Input, m_Normal, min, max, NORM_MINMAX, CV_16UC1);
	else
		cv::normalize(m_Input, m_Normal, min, max, NORM_MINMAX, CV_8UC1);

	return m_Normal;
}

cv::Mat NSE::Normalization8bit(cv::Mat& src)
{
	cv::Mat m_Input = src.clone();

	cv::Mat m_Normal;
	cv::normalize(m_Input, m_Normal, 0, 255, NORM_MINMAX, CV_8UC1);

	return m_Normal;
}

cv::Mat NSE::Normalization16bit(cv::Mat& src)
{
	cv::Mat m_Input = src.clone();

	cv::Mat m_Normal;
	cv::normalize(m_Input, m_Normal, 0, 65535, NORM_MINMAX, CV_16UC1);

	return m_Normal;
}

cv::Mat NSE::ConvertTo8bit(cv::Mat& src)
{
	cv::Mat m_Input = src.clone();
	cv::Mat m_Img8;
	cv::convertScaleAbs(m_Input, m_Img8, 1 / 256.0);
	return m_Img8;
}

cv::Mat NSE::ConvertTo8bit_MinMax(cv::Mat& src)
{
	// 평평한 제품일때는 minmax로 해도되지만
	// 제품이 입체감이 있으면 minval을 정해서 진행 필요
	cv::Mat m_Input = src.clone();
	
	double minVal; 
	double maxVal; 
	Point minLoc; 
	Point maxLoc;

	cv::minMaxLoc(m_Input, &minVal, &maxVal, &minLoc, &maxLoc );
	minVal = 45000;
	m_Input = NSE::Subtraction(m_Input, (int)minVal);

	int nDiff = (int)(maxVal - minVal);
	int nWeight = nDiff / 255;

	cv::Mat m_Img8(m_Input.size(), CV_8UC1);
	for(auto m = 0; m < m_Input.cols; m++)
		for(auto n = 0; n < m_Input.rows; n++){
			if(m_Input.at<ushort>(n, m) == 0)
				m_Img8.at<uchar>(n, m) = 0;
			else
			{
				int nPx = m_Input.at<ushort>(n, m) / nWeight;
				if(nPx >= 65535)
					m_Img8.at<uchar>(n, m) = 65535;
				else
					m_Img8.at<uchar>(n, m) = saturate_cast<uchar>(nPx);
			}
		}

	return m_Img8;
}

cv::Mat NSE::ConvertTo16bit(cv::Mat& src)
{
	cv::Mat m_Input = src.clone();

	Mat m_Input16;
	m_Input.convertTo(m_Input16, CV_16UC1);
	cv::Mat m_Img16(m_Input.size(), CV_16UC1);
	m_Img16 = NSE::Multiply(m_Input16, 256);
	return m_Img16;
}


//////////////// 제품 전처리, 특수검사 알고리즘
cv::Mat NSE::ExceptionBottleBottom(cv::Mat& src, int nTh, int nPoint)
{
	// 병의 왼쪽 오른쪽 외곽을 찾아서 그부분까지

	if(nTh == 0 || nPoint == 0)
		return src;

	Mat img;
	img = src.clone();

	uchar* pt = img.data;
	std::vector<Point> v_ptLine, v_ptLineC, v_ptLineLS, v_ptLineRS;	// 하부, 하부-좌측면, 하부-우측면
	Point ptLB, ptRB;
	Point ptLeft;

	// 병 바닥부 픽셀 및 바닥영역 픽셀 검출
	int endFlag1 = 0;
	for (int y1 = src.rows - 1; y1 > 0; y1--) {
		for (auto x1 = 0; x1 < src.cols; x1++) {	// 0 -> cols 방향으로 탐색
			if(pt[src.cols * y1 + x1] < nTh) {		// 왼쪽 하부 픽셀
				ptLB = Point(x1, y1);
				endFlag1++;
				break;
			}
		}
		if(endFlag1 > 0)	break;
	}

	int endFlag2 = 0;
	for (int y2 = src.rows - 1; y2 > 0; y2--) {
		for (auto x2 = src.cols - 1; x2 > 0; x2--) {	// cols -> 0 방향으로 탐색
			if(pt[src.cols * y2 + x2] < nTh) {
				ptRB = Point(x2, y2);
				endFlag2++;
				break;
			}
		}
		if(endFlag2 > 0)	break;
	}

	for(int n = ptLB.y; n > 0; n--){
		if(pt[src.cols * (n - 1) + ptLB.x] > nTh)
		{
			ptLeft = Point(ptLB.x, n);	// 왼쪽 바닥부 start
			break;
		}
	}

	// 바닥영역 curve point 얻기
	v_ptLineC.push_back(ptLeft);

	for(auto n = ptLB.x; n <= ptRB.x; n++) {
		for(auto gap = 7; gap > -7; gap--) {
			int ypos = v_ptLineC.back().y + gap;
			if(ypos >= img.rows)	ypos = img.rows - 1;

			int px = img.at<uchar>(ypos, n + 1);
			if(px > nTh)	{
				v_ptLineC.push_back(Point(n + 1, ypos));
				break;
			}
		}
	}

	v_ptLineLS.push_back(v_ptLineC.front());
	v_ptLineRS.push_back(v_ptLineC.back());

	for(auto bs = 0; bs < 10; bs++) {
		for(auto gapL = 5; gapL > -5; gapL--) {			// 왼쪽 바닥부 - 측면
			int yposL = v_ptLineC.front().y + gapL;
			if(yposL < 0) yposL = 1;
			int pxL = img.at<uchar>(yposL, v_ptLineC.front().x - bs);
			if((pxL > nTh + 7) && (abs(v_ptLineLS.back().y - yposL) < 5)) {
				v_ptLineLS.push_back(Point(v_ptLineC.front().x - bs, yposL));
				break;
			}
		}

		for(auto gapR = 5; gapR > -5; gapR--) {			// 오른쪽 바닥부 - 측면
			int yposR = v_ptLineC.back().y + gapR;
			if(yposR >= img.cols) yposR = img.cols - 1;
			int pxR = img.at<uchar>(yposR, v_ptLineC.back().x + bs);
			if((pxR > nTh + 7) && (abs(v_ptLineRS.back().y - yposR) < 5)) {
				v_ptLineRS.push_back(Point(v_ptLineC.back().x + bs, yposR));
				break;
			}
		}
	}

	// 병 바닥부 윗부분 line 표시 (color 선 표현)
	Mat imgColor;
	cvtColor(img, imgColor, COLOR_GRAY2BGR);

	for(auto k = 0; k < v_ptLineC.size(); k++) {
		imgColor.at<Vec3b>(v_ptLineC[k])[0] = 0;
		imgColor.at<Vec3b>(v_ptLineC[k])[1] = 0;
		imgColor.at<Vec3b>(v_ptLineC[k])[2] = 255;
	}

	for(auto ls = 0; ls < v_ptLineLS.size(); ls++) {
		imgColor.at<Vec3b>(v_ptLineLS[ls])[0] = 0;
		imgColor.at<Vec3b>(v_ptLineLS[ls])[1] = 255;
		imgColor.at<Vec3b>(v_ptLineLS[ls])[2] = 0;
	}

	for(auto rs = 0; rs < v_ptLineRS.size(); rs++) {
		imgColor.at<Vec3b>(v_ptLineRS[rs])[0] = 0;
		imgColor.at<Vec3b>(v_ptLineRS[rs])[1] = 255;
		imgColor.at<Vec3b>(v_ptLineRS[rs])[2] = 0;
	}

	// Left, Center, Right curve 합치기
	std::reverse(v_ptLineLS.begin(), v_ptLineLS.end());
	v_ptLine.insert(v_ptLine.end(), v_ptLineLS.begin(), v_ptLineLS.end());
	v_ptLine.insert(v_ptLine.end(), v_ptLineC.begin(), v_ptLineC.end());
	v_ptLine.insert(v_ptLine.end(), v_ptLineRS.begin(), v_ptLineRS.end());

	// spline 곡선
	int ptNum = v_ptLine.size();
	for(auto a = 0; a * nPoint < ptNum; a++){
		if((a + 1) * nPoint > ptNum)
			line(imgColor, v_ptLine[a * nPoint], v_ptLine[ptNum - 1], Scalar(255, 255, 255));
		else
			line(imgColor, v_ptLine[a * nPoint], v_ptLine[(a + 1) * nPoint], Scalar(255, 255, 255));
	}

	// 예외처리 - 바닥부 (255)
	for(auto row = imgColor.rows / 2; row < imgColor.rows; row++)	{
		for(auto col = v_ptLine.front().x; col <= v_ptLine.back().x; col++){
			if(imgColor.at<Vec3b>(row, col)[0] == 255 && imgColor.at<Vec3b>(row, col)[1] == 255 && imgColor.at<Vec3b>(row, col)[2] == 255){
				for(auto k = row; k < ptLB.y + 5; k++)
					img.at<uchar>(k, col) = 210;
			}
		}
	}

	//for(auto l = 0; l < v_ptLineLS.size(); l++){
	//	for(auto py = v_ptLineLS[l].y; py < ptLB.y + 5; py++){
	//				img.at<uchar>(py, v_ptLineLS[l].x) = 255;
	//	}
	//}

	//for(auto r = 0; r < v_ptLineRS.size(); r++){
	//	for(auto py = v_ptLineRS[r].y; py < ptLB.y + 5; py++){
	//				img.at<uchar>(py, v_ptLineRS[r].x) = 255;
	//	}
	//}


	return img;
	//return imgColor;
}

cv::Mat NSE::MaskingOutside(const cv::Mat& src, int iter)
{
	cv::Mat m_Input = src.clone();

	// 이미지 내 평균 밝기 찾아서 threshold 값 정하기
	Mat bin = ThresholdMinMax(m_Input, 190, 255);

	Mat morphDil = NSE::Morphology(bin, MORPH_ERODE, 5, iter);

	Mat outside = NSE::Difference(bin, morphDil);
	
	return outside;
}

cv::Mat NSE::InspectPill(const cv::Mat& src, int min, int max, int &minPill, int &maxPill, int &numPill)
{
	cv::Mat m_Input = src.clone();
	cv::Mat m_GrayScale;
	if(m_Input.channels() == 3)
		cv::cvtColor(m_Input, m_GrayScale, cv::COLOR_BGR2GRAY);
	else
		m_GrayScale = m_Input.clone();

	cv::Mat m_Bin = NSE::ThresholdMinMax(m_GrayScale, min, max);

	cv::Mat m_Morph = NSE::Morphology(m_Bin, MORPH_DILATE, 5, 1);

	NSE::Label lb = NSE::Labelling(m_Morph);
	//NSE::Label lb = NSE::Labelling(m_Bin);
	cv::Mat m_Label = lb.labeledImg.clone();

	int maxA = 0, minA = 9999999, num = 0;
	for(auto n=1; n<lb.numOfLabel; n++)
	{
		if(n>100)	break;

		int* p = lb.stats.ptr<int>(n);

		if(p[4] > 5)
		{
			p[4] > maxA ? maxA = p[4] : maxA = maxA;
			p[4] < minA ? minA = p[4] : minA = minA;
			num++;
		}
	}
	
	maxPill = maxA;
	minPill = minA;
	numPill = num;

	return m_Label;
}

cv::Mat NSE::MaskingGridTray(const cv::Mat& src, int block, double c)
{
	cv::Mat m_Input = src.clone();
	cv::Mat m_Addition1 = NSE::Addition(m_Input, m_Input);
	cv::Mat m_Addition2 = NSE::Addition(m_Addition1, m_Input);

	cv::Mat m_AdaptTh = NSE::AdaptiveThreshold(m_Addition2, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, block, c);

	cv::Mat m_Img1 = m_AdaptTh.clone();
	cv::Mat m_Img2 = m_AdaptTh.clone();

	m_Img1 = NSE::Morphology(m_Img1, cv::MORPH_ERODE, 5, 1);

	m_Img2 = NSE::Morphology(m_Img2, cv::MORPH_DILATE, 7, 1);
	m_Img2 = NSE::Morphology(m_Img2, cv::MORPH_ERODE, 7, 1);
	m_Img2 = NSE::Morphology(m_Img2, cv::MORPH_ERODE, 5, 1);
	m_Img2 = NSE::Morphology(m_Img2, cv::MORPH_ERODE, 3, 1);

	cv::Mat m_GridTray = NSE::BitXOR(m_Img1, m_Img2);
	m_GridTray = NSE::BitNOT(m_GridTray);

	cv::Mat m_MaskingResult = NSE::HardLight(m_GridTray, m_Input);

	return m_MaskingResult;
}

cv::Mat NSE::InspectDual(const cv::Mat& src)
{
	cv::Mat m_Input = src.clone();

	cv::Mat m_Low = src(cv::Rect(0, 0, m_Input.cols / 2, m_Input.rows));
	cv::Mat m_High = src(cv::Rect(m_Input.cols / 2, 0, m_Input.cols / 2, m_Input.rows));

	cv::Mat LowM = NSE::MedianBlur(m_Low, 3);
	LowM = NSE::Addition(LowM, LowM);
	cv::Mat LowAdd = NSE::Addition(LowM, 30000);
	cv::Mat HighAdd = NSE::Addition(m_High, m_High);

	cv::Mat HighSm2 = NSE::GaussianFilter(HighAdd, 15, 4);

	cv::Mat cvResult = NSE::Subtraction(LowAdd, HighSm2);

	return cvResult;
}

cv::Mat NSE::InspectDualFish(const cv::Mat& src)
{
	cv::Mat m_Input = src.clone();

	cv::Mat m_Low = src(cv::Rect(0, 0, m_Input.cols / 2, m_Input.rows));
	cv::Mat m_High = src(cv::Rect(m_Input.cols / 2, 0, m_Input.cols / 2, m_Input.rows));

	cv::Mat LowM = NSE::MedianBlur(m_Low, 3);
	LowM = NSE::Addition(LowM, LowM);
	cv::Mat LowAdd = NSE::Addition(LowM, LowM);
	cv::Mat HighAdd = NSE::Addition(m_High, m_High);

	cv::Mat HighSm2 = NSE::GaussianFilter(HighAdd, 7, 4);

	cv::Mat cvResult = NSE::Subtraction(LowAdd, HighSm2);

	return cvResult;
}

bool NSE::InspectOmit(const cv::Mat& src, int low, int high, int OmitNum, int perL, int perH)
{
	cv::Mat m_Input = src.clone();

	cv::Mat m_Bin = NSE::ThresholdMinMax(m_Input, low, high);

	int num = 0;
	for (int row = 0; row < m_Bin.rows; row++) 	{ 
 		uchar* pointer_row = m_Bin.ptr<uchar>(row); 
		for (int col = 0; col < m_Bin.cols; col++) 	{ 
    		uchar px = pointer_row[col];
			if(px == 255)
				num++;
		}
	}

	if(num < OmitNum * perL / 100)
		return false;
	else if(num > OmitNum * perH / 100)
		return false;
	else
		return true;
}

//bool NSE::InspectBoxOmit(const cv::Mat& src, int low, int high, int size, int wCount, int hCount, int num)
cv::Mat NSE::InspectBoxOmit(const cv::Mat& src, int low, int high, int size, int wCount, int hCount, int num, int masklr, int masktb)
{
	int nProductCnt = 0;
	cv::Mat m_Input = src.clone();
	cv::Mat m_InputColor;

	// 박스 외곽 꼭지점 4Point 찾기
	Point ptLT(9999, 9999), ptRT(0, 9999), ptLB(9999, 0), ptRB(0, 0);
	cv::Mat m_Reduce;
	cv::resize(m_Input, m_Reduce, m_Input.size() / 10);

	cv::Mat m_ReduceBin = NSE::ThresholdMinMax(m_Reduce, 0, 250);
	cv::Mat m_ReduceMorph = NSE::Morphology(m_ReduceBin, MORPH_DILATE, 3, 2);

	imwrite("D:\\ReduceMorph.jpg", m_ReduceMorph);

	// 1/4 영역 내 탐색
	int x, y;
	for(y = 0; y < m_ReduceMorph.rows / 2; y++)
		for(x = 0 ; x < m_ReduceMorph.cols / 2; x++)
		{
			if(m_ReduceMorph.at<uchar>(y, x) == 255)
			{
				x < ptLT.x ? ptLT.x = x : ptLT.x;
				y < ptLT.y ? ptLT.y = y : ptLT.y;
				break;
			}
		}

		std::wcout << _T("ptLT : ") << ptLT.x << " " << ptLT.y << endl;

	for(y = 0; y < m_ReduceMorph.rows / 2; y++)
		for(x = m_ReduceMorph.cols - 1; x > m_ReduceMorph.cols / 2; x--)
		{
			if(m_ReduceMorph.at<uchar>(y, x) == 255)
			{
				x > ptRT.x ? ptRT.x = x : ptRT.x;
				y < ptRT.y ? ptRT.y = y : ptRT.y;
				break;
			}
		}

		std::wcout << _T("ptRT : ") << ptRT.x << " " << ptRT.y << endl;


	for(y = m_ReduceMorph.rows - 1; y > m_ReduceMorph.rows / 2; y--)
		for(x = 0; x < m_ReduceMorph.cols / 2; x++)
		{
			if(m_ReduceMorph.at<uchar>(y, x) == 255)
			{
				x < ptLB.x ? ptLB.x = x : ptLB.x;
				y > ptLB.y ? ptLB.y = y : ptLB.y;
				break;
			}
		}
		
		std::wcout << _T("ptLB : ") << ptLB.x << " " << ptLB.y << endl;


	for(y = m_ReduceMorph.rows - 1; y > m_ReduceMorph.rows / 2; y--)
		for(x = m_ReduceMorph.cols -1; x > m_ReduceMorph.cols / 2; x--)
		{
			if(m_ReduceMorph.at<uchar>(y, x) == 255)
			{
				x > ptRB.x ? ptRB.x = x : ptRB.x;
				y > ptRB.y ? ptRB.y = y : ptRB.y;
				break;
			}
		}

		std::wcout << _T("ptRB : ") << ptRB.x << " " << ptRB.y << endl;


	//  회전 각도 계산
	int dx = ptRT.x - ptLT.x;	
	int dy = ptRT.y - ptLT.y;
	double rad = atan2((double)dy, (double)dx);
	double degree = (rad * 180) / PI;

	Mat matrix = cv::getRotationMatrix2D(Point(m_Input.rows / 2, m_Input.cols / 2), degree, 1);
	Mat m_Rotate;
	cv::warpAffine(m_Input, m_Rotate, matrix, m_Input.size(), 1, 0, Scalar(255));
	cv::cvtColor(m_Rotate, m_InputColor, cv::COLOR_GRAY2BGR);

	imwrite("D:\\Rotate.jpg", m_Rotate);

	// W, H length
	double dbLenW, dbLenH;
	dbLenW = sqrt(pow((ptRT.x - ptLT.x), 2.0) + pow((ptRT.y - ptLT.y), 2.0)) * 10;
	dbLenH = sqrt(pow((ptLB.x - ptLT.x), 2.0) + pow((ptLB.y - ptLT.y), 2.0)) * 10;

	std::wcout << _T("dbLenW : ") << dbLenW << _T(", dbLenH : ") << dbLenH << endl;

	// 객체 중심 좌표
	Point ptCenter((ptLT.x + ptLB.x + ptRT.x + ptRB.x) / 4 * 10, (ptLT.y + ptLB.y + ptRT.y + ptRB.y) / 4 * 10);
	std::wcout << _T("ptCenter : ") << ptCenter.x << " " << ptCenter.y << endl;

	// 회전된 객체의 Rect
	Rect rtRotate(ptCenter.x - dbLenW / 2, ptCenter.y - dbLenH / 2, dbLenW, dbLenH);

	// masking
	rtRotate.x += masklr * 10;
	rtRotate.width -= masklr * 10;
	rtRotate.y += masktb * 10;
	rtRotate.height -= masktb * 10;

	cv::rectangle(m_InputColor, rtRotate, cv::Scalar(255, 0, 0));
	imwrite("D:\\RotateRectangle.jpg", m_InputColor);

	cv::Mat m_Bin = NSE::ThresholdMinMax(m_Input, low, high);
	cv::Mat m_CropBin = m_Bin(rtRotate);

	int nStepW = m_CropBin.cols / wCount;
	int nStepH = m_CropBin.rows / hCount;
	for(int h = 0; h < hCount; h++){
		for(auto w = 0; w < wCount; w++){
			cv::Rect rc(w * nStepW, h * nStepH, nStepW, nStepH);
			cv::Mat cvCrop = m_CropBin(rc);
			if(cv::countNonZero(cvCrop) > size){
				nProductCnt++;
				// 라벨 영역 color 변환
				cv::Scalar sc(rand() * 255, rand() * 255, rand() * 255);
				
				for(auto yy = 0; yy < cvCrop.rows; yy++){
					for(auto xx = 0; xx < cvCrop.cols; xx++){
						if(cvCrop.at<uchar>(yy, xx) != 0){
							m_InputColor.at<Vec3b>(yy + rtRotate.y + rc.y - 1, xx + rtRotate.x + rc.x - 1)[0] = sc[0];
							m_InputColor.at<Vec3b>(yy + rtRotate.y + rc.y - 1, xx + rtRotate.x + rc.x - 1)[1] = sc[1];
							m_InputColor.at<Vec3b>(yy + rtRotate.y + rc.y - 1, xx + rtRotate.x + rc.x - 1)[2] = sc[2];
						}
					}
				}
			}
		}
	}
	
	// 분할되는 영역 draw
	for(auto lr = 1; lr < hCount; lr++){
		int nPtY = rtRotate.y + (((rtRotate.height) / hCount) * lr);
		cv::line(m_InputColor, cv::Point(rtRotate.x, nPtY), cv::Point(rtRotate.x + rtRotate.width, nPtY), cv::Scalar(0, 255, 0));
	}

	for(auto lc = 1; lc < wCount; lc++)
	{
		int nPtX = rtRotate.x + (((rtRotate.width) / wCount) * lc);
		cv::line(m_InputColor, cv::Point(nPtX, rtRotate.y), cv::Point(nPtX, rtRotate.y + rtRotate.height), cv::Scalar(0, 255, 0));
	}

	// Count draw
	cv::putText(m_InputColor, to_string(nProductCnt), cv::Point(1, 1), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 0, 0), 1);

	//if(nProductCnt == num)
	//	return true;
	//else
	//	return false;
	return m_InputColor;
}
