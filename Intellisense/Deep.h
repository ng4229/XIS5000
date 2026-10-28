#pragma once

#ifdef DEEP_EXPORTS
#define DEEP_API __declspec(dllexport)
#else
#define DEEP_API __declspec(dllimport)
#endif

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <time.h>
#include <ctime>
#include <chrono>
#include <fstream>
#include <exception>
#include <sstream>
#include <thread>
#include <atomic>
#include <windows.h>
#include <cmath>
#include <iomanip>
#include <mutex>
#include <shobjidl.h>  // IFileDialog 사용
#include <atlbase.h>   // CComPtr
#include <atlconv.h>   // CT2A

//#include <cuda_runtime.h>

#if (ENABLE_NSEAI_CLA || ENABLE_NSEAI_DET || ENABLE_NSEAI_SEG)
#pragma comment(lib, "onnxruntime.lib")
#pragma comment(lib, "onnxruntime_providers_shared.lib")
//#pragma comment(lib, "onnxruntime_providers_tensorrt.lib")
#endif

#if ENABLE_NSEAI_VINO
#pragma comment(lib, "openvino.lib")
#endif

//#pragma comment(lib, "nrt.lib")

namespace NSE {

    // ===================== AI =====================
    namespace AI {

        /// @brief Classification GPU/CPU 기반  추론 컨트롤러(모델 로드/추론/로깅)
        class DEEP_API GpuCla {
        public:
            GpuCla();
            ~GpuCla();

            /// @brief 제품명 기반 모델 초기화(경로/옵션 로드)
            int initialAIModel(std::string product_name);

            /// @brief 2D 이미지 추론(전처리→추론→결과 반환)
            int inferenceAIModel(cv::Mat& Frame, float probThresh = 0.5f);

            /// @brief 1D 버퍼 입력으로 추론(센서/프레임 버퍼 처리)
            int inferenceAIModel1D(unsigned char* Img_Buf, int w, int h, float probThresh = 0.5f);

            /// @brief 추론 수행 + 히트맵만 생성(클래스 결과 제외)
            int inferenceAIModelOnly(cv::Mat& Frame, cv::Mat& heatmap, float probThresh = 0.5f);

            /// @brief 추론 수행 + 히트맵 생성(시각화)
            int inferenceAIModelHeatmap(cv::Mat& Frame, cv::Mat& heatmap, float probThresh = 0.5f);
            
            // @brief 원본 이미지 좌표계로 변환된 CAM 중심점 반환 (없으면 {-1,-1})
            cv::Point getCamCenterOrig();

            /// @brief CAM 계산 부분 분리
            bool buildCAMAndOverlay(const float* featData, const std::vector<int64_t>& featShape,
                int classId, const std::string& camWeightPath, const cv::Mat& Frame, cv::Mat& heatmap, cv::Mat& camResized, cv::Point& hotspot);

            /// @brief FC 가중치 행/열 모양 검증(모델 호환 체크)
            void check_fc_weights_shape(const std::string& weight_path, int expected_rows, int expected_cols);

            /// @brief 예열/첫 추론 지연 최소화(더미 인퍼런스)
            int useGpuPeriod();

            /// @brief 검사 이미지/히트맵 저장(모드/클래스 기준)
            void saveInspectImage(const cv::Mat& image, const cv::Mat& heatmap, int result_cla, int confidence);

            /// @brief 검사 결과 CSV 로그 기록
            void saveCSVLog(int classId);

            /// @brief 임계치 기반 클래스 보정(신뢰도 미달→0 등)
            int checkProbabilityThreshold(int classId, double confidence, float probThresh);

            /// @brief 원격/스토리지 파일 존재 확인(비동기 큐)
            void checkFileExists();

            /// @brief 원격 모델 다운로드(비동기 큐)
            void downloadModel();

            /// @brief 폴더 업로드(비동기 큐)
            void uploadFolder();

            /// @brief 폴더 다운로드(비동기 큐)
            void downloadFolder();

            /// @brief 네트워크/서비스 헬스체크(비동기 큐)
            void checkHealth();

            /// @brief 자동 유지보수 작업 트리거(파일 정리 등)
            void RunAutoMaintenance();

            struct CoordTransform
            {
                // [0] Flip (cv::flip(image, image, 0) 같은 세로 flip)
                bool flipped = false;
                // flip 적용 "전" 원본 이미지 높이
                int  flipImageHeight = 0;

                // [1] Crop (원본 기준 최종 ROI)
                cv::Rect cropRect = cv::Rect(0, 0, 0, 0);

                // [2] Rotation (선택: Seg에서 alignByTopEdge 등)
                double rotateAngleDeg = 0.0;
                cv::Size croppedImageSize = cv::Size(0, 0); // 회전 전 크기
                cv::Size rotatedImageSize = cv::Size(0, 0); // rotateBound 결과 크기

                // [3] Letterbox (standTransImageSimple 내부)
                cv::Size standSrcSize = cv::Size(0, 0);
                // stand output (모델 입력 크기, 예: 512x1024, 640x640)
                cv::Size standDstSize = cv::Size(0, 0);
                // letterbox scale (resize ratio)
                float scale = 1.0f;
                // padding offset (left/top)
                cv::Point padOffset = cv::Point(0, 0);
            };

        private:
#if ENABLE_NSEAI_CLA
            // ONNX 관련 리소스 캡슐화 구조체(.cpp에서 정의)
            struct OrtObjects;
            std::unique_ptr<OrtObjects> ortObjects;

            // 멀티스레드 추론 보호용 전역 뮤텍스
            static std::mutex globalInferenceMutex;

#endif
            CoordTransform m_lastCoordTransform;
            cv::Size       m_lastOriSize;
        };


        /// @brief Detection GPU/CPU 기반  추론 컨트롤러(모델 로드/추론/로깅)
        class DEEP_API GpuDet {
        public:
            GpuDet();
            ~GpuDet();

            int initialAIModel(std::string product_name);
            int inferenceAIModel(cv::Mat& Frame, float probThresh = 0.5f);

            void printModelInfo(std::string product_name);
            void saveInspectImage(const cv::Mat& image, const cv::Mat& heatmap, int result_det, int confidence);

        private:
#if ENABLE_NSEAI_DET
            // ONNX 관련 리소스 캡슐화 구조체(.cpp에서 정의)
            struct OrtObjects;
            std::unique_ptr<OrtObjects> ortObjects;

            // 멀티스레드 추론 보호용 전역 뮤텍스
            static std::mutex globalInferenceMutex;
#endif
        };


        /// @brief Segmentation GPU/CPU 기반  추론 컨트롤러(모델 로드/추론/로깅)
        class DEEP_API GpuSeg {
        public:
            GpuSeg();
            ~GpuSeg();

            int initialAIModel(std::string product_name);
            int inferenceAIModel(cv::Mat& imageOri, cv::Mat& imageCam, std::vector<cv::Point>& centers);
            int inferenceAIModelOnly(cv::Mat& imageOri, cv::Mat& imageCam, std::vector<cv::Point>& centers);

            void printModelInfo(std::string product_name);
            void saveInspectImage(const cv::Mat& image, const cv::Mat& heatmap, int result_det, int confidence);

            void LetterBox(const cv::Mat& src, cv::Mat& dst, cv::Vec4d& params, const cv::Size& new_shape, bool scaleUp = true);

            struct OutputParams {
                int id;                     // class id
                float confidence;           // score
                cv::Rect box;               // bounding box
                cv::Mat boxMask;            // final binary mask for this box
            };

            struct MaskParams {
                cv::Vec4d params;      // box: scale, scale, pad_w, pad_h
                cv::Size srcImgShape;  // original image size
                int netHeight;         // 640
                int netWidth;          // 640
                float maskThreshold;   // 0.5
            };

            void GetMask2(const cv::Mat& mask_coeff,
                const cv::Mat& proto,
                OutputParams& det,
                const MaskParams& maskParams);


        private:
#if ENABLE_NSEAI_SEG
            // ONNX 관련 리소스 캡슐화 구조체(.cpp에서 정의)
            struct OrtObjects;
            std::unique_ptr<OrtObjects> ortObjects;
           // std::mutex m_imageQueueMutex;
            static std::mutex globalInferenceMutex;
#endif
        };


        /// @brief Segmentation GPU/CPU 기반  추론 컨트롤러(모델 로드/추론/로깅)
        class DEEP_API CpuCla {
        public:
            CpuCla();
            ~CpuCla();

            int initialAIModel(std::string product_name);
            int inferenceAIModel(const cv::Mat& image, float probThresh, int& classId, float& confidence);

        private:
#if ENABLE_NSEAI_VINO
            struct VinoObjects;                       // 전방 선언
            std::unique_ptr<VinoObjects> vino_;       // 구현 캡슐화(PImpl)
#endif
        };

#if ENABLE_NEURO_CLA
        /// @brief NeuroCla GPU/CPU 기반  추론 컨트롤러(모델 로드/추론/로깅)
        class DEEP_API NeuroCla {
        public:
            NeuroCla();
            ~NeuroCla();

            int initialAIModel(std::string product_name);
            int inferenceAIModel(cv::Mat& Frame, cv::Mat& Cam);
            int inferenceAIModel_bulk(cv::Mat& imageOri, cv::Mat& imageCam, std::vector<cv::Point>& centers);
            int ShowResult(cv::Mat& Frame, cv::Mat& Cam);
            int ShowResult_bulk(const cv::Mat& Frame, cv::Mat& Cam, const std::vector<cv::Mat>& tiles, std::vector<cv::Point>& centers);
            std::vector<cv::Point> getCamCenterOrig();
            void saveInspectImage(const cv::Mat& image, const cv::Mat& heatmap, int result_cla, int confidence);


        private:
            struct NeuroClaObjects;
            std::unique_ptr<NeuroClaObjects> neuroClaObjects;

            int device_idx;
            bool fp16_flag;
            bool threshold_flag;
            int batch_size;

            static std::mutex globalInferenceMutex;
        };
#endif

#if ENABLE_NEURO_SEG
        /// @brief NeuroSeg GPU/CPU 기반  추론 컨트롤러(모델 로드/추론/로깅)
        class DEEP_API NeuroSeg {
        public:
            NeuroSeg();
            ~NeuroSeg();

            int initialAIModel(std::string product_name);
            int inferenceAIModel(cv::Mat Frame, cv::Mat& Cam, std::vector<std::pair<int, std::vector<cv::Point>>>& outSegObjects);
            int inferenceAIModelOnly(cv::Mat& Frame, cv::Mat& Cam, std::vector<std::pair<int, std::vector<cv::Point>>>& outSegObjects);
            int ShowResult(cv::Mat& Frame, cv::Mat& Cam, std::vector<std::pair<int, std::vector<cv::Point>>>& outSegObjects);
            int get_patch_size(const wchar_t* img_path);

            struct SegCoordTransform
            {
                // [0] flip
                bool flipped = false;
                int  flipImageHeight = 0;   // flip 되기 전 "원본" 이미지 높이

                // [1] crop (원본에서 잘라낸 ROI)
                cv::Rect cropRect = cv::Rect(0, 0, 0, 0);

                // [2] rotation (alignByTopEdge로 회전했을 때)
                double rotateAngleDeg = 0.0;
                cv::Size croppedImageSize = cv::Size(0, 0); // 회전 전 이미지 크기
                cv::Size rotatedImageSize = cv::Size(0, 0); // rotateBound 결과 이미지 크기

                // [3] standTransImageSimple 내부 letterbox
                float scale = 1.0f;
                cv::Point padOffset = cv::Point(0, 0); // padding offset (x,y)
            };

            void RestoreSegPointsToOriginal(std::vector<std::pair<int, std::vector<cv::Point>>>& segObjects, const SegCoordTransform& T);

            /// @brief 1D 버퍼 입력으로 추론(센서/프레임 버퍼 처리)
            int inferenceAIModel1D(unsigned char* Img_Buf, int w, int h);

            /// @brief 검사 이미지/히트맵 저장(모드/클래스 기준)
            void saveInspectImage(const cv::Mat& image, const cv::Mat& heatmap, int result_seg, int confidence);

            int useGpuPeriod();


        private:
            struct NeuroSegObjects;
            std::unique_ptr<NeuroSegObjects> neuroSegObjects;

            int device_idx;
            bool fp16_flag = false;
            bool threshold_flag = false;

            int batch_unit;
            int num_of_images;
            int batch_size;

            // 멀티스레드 추론 보호용 전역 뮤텍스
            std::mutex globalInferenceMutex;

            std::unique_ptr<NeuroSeg> m_neuroSegSecond;
            
        };
#endif
    } // namespace AI


    // ===================== CV =====================
    namespace CV {

        struct TopEdgeDebugInfo
        {
            // [입력 & 기본 전처리]
            cv::Mat src;             // 입력 crop (cropped_bgr)
            cv::Mat gray;            // 그레이
            cv::Mat bin;             // 이진/마스크 (makeBinaryMaskForDark 결과)

            // [에지 & 상단 영역]
            cv::Mat edges;           // Canny 에지 전체
            cv::Mat top_band_mask;   // 상단 영역 마스크
            cv::Mat edges_top;       // 상단 영역 에지

            // [Hough 결과]
            cv::Mat hough_vis;       // HoughLinesP 시각화 이미지
            std::vector<cv::Vec4i> lines_all;   // 전체 Hough 라인
            std::vector<cv::Vec4i> lines_used;  // 최종 사용 라인(들)

            // [최종 결과]
            double best_angle_deg = 0.0;        // 선택된 라인 각도(도)
            bool success = false;               // 추정 성공 여부
        };


        /// @brief OpenCV 기반 전처리/보정 유틸리티
        class DEEP_API BaseIsOpencv {
        public:
            BaseIsOpencv();
            ~BaseIsOpencv();

            /// @brief 8UC1 버퍼를 Mat(BGR/512)로 표준 변환
            cv::Mat convert8UC1ToMat(unsigned char* Img_Buf, int w, int h);

            /// @brief 입력 이미지를 표준화(BGR/512)로 변환
            cv::Mat standTransImage(const cv::Mat& input_image);
            cv::Mat standTransImageSimple(const cv::Mat& input_image);
            cv::Mat standTransImageSimple_WithLetterboxInfo(const cv::Mat& input, float& outScale, cv::Point& outPad);
            cv::Mat standTransImageSimple_coordTransform(const cv::Mat& input_image, float& outScale,
                cv::Point& outPadOffset, cv::Size& outStandSrcSize, cv::Size& outStandDstSize);

            /// @brief 스포이드(수직선) 길이 추정(픽셀 단위)
            double detectSpoidLength(const cv::Mat& input_image);

            /// @brief 기울어진 대상 회전 보정
            cv::Mat correctImageRotation(const cv::Mat& input_image);

            /// @brief 앰플 병 수직 정렬(머리 방향 보정 포함)
            cv::Mat straightenAmpoule(const cv::Mat& input_image);

            /// @brief ROI를 이미지 크기에 맞춰 clamp 후 crop하여 반환.
            cv::Mat cropROI(const cv::Mat& input, int left, int top, int right, int bottom, cv::Rect& out_roi);

            

            cv::Mat Overlay_1(cv::Mat& src1, cv::Mat& src2);
            cv::Mat to8UC3Safe(const cv::Mat& src);
            cv::Mat overlayInvert_1(const cv::Mat& src, float alpha, float beta, float threshold);
            cv::Mat applyGamma_1(const cv::Mat& src, float gamma);
            cv::Mat overlayInvert_2(const cv::Mat& src, float alpha, float beta, float threshold);
            cv::Mat applyGamma_2(const cv::Mat& src, float gamma);

            cv::Mat to8UC1Safe(const cv::Mat& src); // 8UC1 변환
            cv::Mat overlayInvert(const cv::Mat& src, float alpha, float beta, float threshold); // // 원본 반전 Overlay S-커브 대비 강화
            cv::Mat applyGamma(const cv::Mat& src, float gamma); // 감마 LUT



            // ───── 도메인 매칭 파라미터 (당신이 올린 Params 그대로) ─────
            struct DomainMatchParams
            {
                // 1) Green mask (HSV, OpenCV 스케일)
                int h_low;   // 예: 35
                int h_high;  // 예: 90
                int s_low;   // 예: 40
                int v_low;   // 예: 40

                // 2) Lab mean/std match
                double alpha;      // 0~1, 색/밝기 분포 블렌딩 강도 (예: 0.35)

                // 3) Low-freq L* lightness alignment
                double beta;       // 0~1, 조도 보정 강도 (예: 0.30)
                double gain_min;   // 예: 0.8
                double gain_max;   // 예: 1.2

                // 4) Sharpness alignment (Var(Laplacian))
                double sharp_cap;  // 예: 0.10 (±10%)

                // Low-frequency kernel scale
                int kernel_divisor; // 예: 18
            };

            // 기본 preset (정의/초기화는 .cpp에서 한 번만)
            static DomainMatchParams s_DomainMatchDefault;

            // 2025 → 2023 매칭 (파라미터 지정 버전)
            cv::Mat match_2025_to_2023(const cv::Mat& ref2023, const cv::Mat& src2025, const DomainMatchParams& p);

            // 2025 → 2023 매칭 (기본 preset 사용)
            cv::Mat match_2025_to_2023(const cv::Mat& ref2023, const cv::Mat& src2025);


            // '큰 객체'들을 모아 Union BBox 계산
            bool findUnionBBoxOfLargeComponents(const cv::Mat& bgr,
                double min_area_ratio,   // 예: 0.10 = 10%
                cv::Rect& out_union,
                bool invert_for_dark = true);

            // 여유 포함 크롭 (경계 안전)
            cv::Mat cropWithMarginFromRect(const cv::Mat& src,
                const cv::Rect& rect,
                int margin_px);

            // 어두운 객체(검은색)를 전경으로 보는 바이너리 마스크 생성 (Otsu + Blur + Morph)
            cv::Mat makeBinaryMaskForDark(const cv::Mat& bgr,
                const cv::Size& gaussK = cv::Size(5, 5),
                bool invert_for_dark = true);

            // 잘림 없이 회전
            cv::Mat rotateBound(const cv::Mat& src,
                double angle_deg,
                const cv::Scalar& border = cv::Scalar(255, 255, 255));

            // 상단 엣지 각도 추정 (디버그 정보 포함)
            bool estimateTopEdgeAngleEx(const cv::Mat& cropped_bgr,
                double top_band_frac,
                bool invert_for_dark,
                int canny_lo, int canny_hi,
                int hough_thresh,
                double min_line_len_ratio,
                int max_line_gap,
                double& angle_deg_out,
                TopEdgeDebugInfo* dbg);

            // 상단 엣지를 기준으로 영상 정렬
            cv::Mat alignByTopEdge(const cv::Mat& cropped_bgr,
                double top_band_frac,
                bool invert_for_dark,
                int canny_lo, int canny_hi,
                int hough_thresh,
                double min_line_len_ratio,
                int max_line_gap,
                double* angle_used,
                TopEdgeDebugInfo* dbg);

            // 디버그 이미지/정보를 파일로 저장
            void saveTopEdgeDebug(const cv::Mat& src,
                const TopEdgeDebugInfo& dbg,
                const std::string& folder,
                const std::string& prefix);


            bool BaseIsOpencv::findLargestBBoxOfComponents(
                const cv::Mat& bgr,
                double min_area_ratio,
                cv::Rect& out_box,
                bool invert_for_dark);

            cv::Mat BaseIsOpencv::cropBottomByRatio(const cv::Mat& src, double ratio);   // 예: 0.25 → 하단 25%

            std::vector<cv::Mat> BaseIsOpencv::splitAndStandTransImageBatch8(const cv::Mat& input_image);
            std::vector<cv::Mat> BaseIsOpencv::splitAndStandTransImageBatch2(const cv::Mat& input_image);
            std::vector<cv::Mat> BaseIsOpencv::splitAndStandTransImageBatch(const cv::Mat& input_image, int tile_rows, int tile_cols);

        private:
        };
    } // namespace CV


    // ===================== Utils =====================
    namespace Utils {

        /// @brief 로깅/시간/경로/디스크/에러 핸들 유틸 모음
        class DEEP_API Utils {
        public:
            /// @brief 콘솔/파일 로그 출력(레벨별)
            void logMessagePrint(const std::string& message);

            /// @brief 현재 시간(초) 반환
            double getCurrentTime();

            /// @brief 실행시간 로깅(초, 소수점 고정)
            double logExecutionTime(const std::string& prefix, double startTime, double endTime);

            /// @brief nvidia-smi 명령 실행(예: 클럭 설정)
            void runNvidiaSMI();

            /// @brief 관리자 권한 실행 여부 확인
            bool checkProgramAdmin();

            /// @brief 시간 문자열 생성(yyyy-mm-dd, hh-mm-ss, all 등)
            std::string makeTime(const std::string& format = "yearmonday");

            /// @brief 폴더 없으면 생성(재귀)
            void makeFolder(const std::string& path);

            /// @brief 에러 로그(파일) 기록
            void logError(const std::string& context, const std::string& message);

            /// @brief 커스텀 에러 로깅(필요 시 예외 재던지기)
            void logErrorCustom(const std::string& message);

            /// @brief 예외 처리 공통 루틴(로그/전파)
            void handleException(const std::exception& e, const std::string& context);

            /// @brief 파일 존재 여부
            bool isFileExists(const std::string& path);

            /// @brief 폴더 존재 여부
            bool isFolderExists(const std::string& path);

            /// @brief 폴더 선택 다이얼로그(ANSI)
            std::string pathSelectFolder();

            /// @brief 파일 선택 다이얼로그(ANSI)
            std::string pathSelectFile();

            /// @brief 파일 선택 다이얼로그(유니코드)
            std::wstring pathSelectFileW();

            /// @brief wstring→string 변환(코드페이지 지정)
            std::string wstringToString(const std::wstring& wstr, UINT codePage = CP_ACP);

            /// @brief string→wstring 변환(코드페이지 지정)
            std::wstring stringToWstring(const std::string& str, UINT codePage = CP_ACP);

            /// @brief 파일명만 추출(확장자 포함)
            std::string getFilenameOnly(const std::string& full_path);

            /// @brief 순수 파일명(확장자 제외)
            std::string getPureFilename(const std::string& full_path);

            /// @brief 드라이브 사용량 임계 초과 여부
            bool isDriveUsageOverThreshold(const std::string& drivePath, double thresholdPercent);

            /// @brief 파일/폴더가 N일보다 오래됐는지
            bool isOlderThan(const std::string& filePath, int daysThreshold);

            /// @brief 오래된 파일/빈 폴더 재귀 삭제
            void deleteOldFilesRecursive(const std::string& folderPathStr, int daysThreshold);

            /// @brief STAGE 폴더 파일이 threshold 이상이면 자동 리네임
            void renameStageFoldersOnThreshold(const std::string& basePath, int threshold);

            int GetImageSavePeriod();
            int GetConfigInputPrePro();
            
            std::string addSuffixBeforeExtension(const std::string& originalPath, const std::string& suffix);

            void InitCudaRuntime();

            void GpuWarmup();

        };

        /// @brief 비동기 이미지 저장 싱글톤(큐→디스크)
        class DEEP_API ImageSaver {
        public:
            /// @brief 싱글톤 인스턴스 획득
            static ImageSaver& getInstance() {
                static ImageSaver instance;
                return instance;
            }

            /// @brief 저장 스레드 시작
            void startImageSaveThread();

            /// @brief 저장 스레드 종료
            void stopImageSaveThread();

            /// @brief OpenCV imwrite로 저장(.jpg/.png)
            bool saveImageWithOpenCV(const std::string& filename, const cv::Mat& image);

            /// @brief 저장 파일 확인(재시도/슬립)
            bool verifyFileSave(const std::string& filename, int maxRetries = 40, int sleepTime = 5);

        private:
            ImageSaver() = default;
            ~ImageSaver() { stopImageSaveThread(); }

            ImageSaver(const ImageSaver&) = delete;
            ImageSaver& operator=(const ImageSaver&) = delete;
            ImageSaver(ImageSaver&&) = delete;
            ImageSaver& operator=(ImageSaver&&) = delete;

            /// @brief 내부 스레드 루프(큐 소비)
            void runImageSaveThread();

            std::atomic<bool> keepRunning{ true };
            std::thread imageSaveThread;
        };

        /// @brief 비동기 CSV 로그 저장 싱글톤
        class DEEP_API CSVLogSaver {
        public:
            /// @brief 싱글톤 인스턴스 획득
            static CSVLogSaver& getInstance() {
                static CSVLogSaver instance;
                return instance;
            }

            /// @brief 로그 저장 스레드 시작
            void startCSVLogSaveThread();

            /// @brief 로그 저장 스레드 종료
            void stopCSVLogSaveThread();

            /// @brief 검사 결과 라인 큐에 추가(시간/제품/수치)
            void pushCSVLogEntry(const std::string& currentTime, const std::string& productName, int total, int ok, int ng);

        private:
            CSVLogSaver() = default;
            ~CSVLogSaver() { stopCSVLogSaveThread(); }

            CSVLogSaver(const CSVLogSaver&) = delete;
            CSVLogSaver& operator=(const CSVLogSaver&) = delete;
            CSVLogSaver(CSVLogSaver&&) = delete;
            CSVLogSaver& operator=(CSVLogSaver&&) = delete;

            /// @brief 내부 스레드 루프(큐→파일)
            void runCSVLogSaveThread();

            /// @brief CSV 파일에 한 줄 쓰기(날짜별 파일 분리)
            bool writeCSVLogToFile(const std::string& line, const std::string& date);

            /// @brief 로그 큐 엔트리 구조
            struct LogEntry {
                std::string timestamp;
                std::string productName;
                int total;
                int good;
                int bad;
            };

            std::deque<LogEntry> logQueue;
            std::mutex queueMutex;
            std::condition_variable dataCV;

            std::atomic<bool> running{ false };
            std::thread logThread;
        };

        /// @brief 단순 Key-Value 설정 관리(로드/세이브)
        class DEEP_API ConfigManager {
        private:
            std::unordered_map<std::string, std::string> configs;

            ConfigManager() = default;
            ~ConfigManager() = default;

            ConfigManager(const ConfigManager&) = delete;
            ConfigManager& operator=(const ConfigManager&) = delete;

        public:
            /// @brief 싱글톤 인스턴스
            static ConfigManager& getInstance() {
                static ConfigManager instance;
                return instance;
            }

            /// @brief 설정 값 쓰기
            void setSetting(const std::string& key, const std::string& value);

            /// @brief 설정 값 읽기(없으면 기본값)
            std::string getSetting(const std::string& key, const std::string& defaultValue = "");

            /// @brief 설정 파일 저장
            void saveSettings();

            /// @brief 설정 파일 로드(+전역 설정 반영)
            void loadSettings();

            /// @brief 전체 설정 덤프 출력
            void printAllSettings();

            /// @brief config int 미입력 시 디폴트 값 추가
            int getIntOrDefault(const std::string& key, int defaultValue);

            /// @brief config string 미입력 시 디폴트 값 추가
            std::string getStringOrDefault(const std::string& key, const std::string& defaultValue);

        };

        /// @brief 단일 타이머(고정 주기 콜백 실행)
        class DEEP_API SingleTimer {
        public:
            SingleTimer();
            ~SingleTimer();

            /// @brief 주기(ms)와 콜백 등록하고 시작
            void start(int interval_ms, std::function<void()> callback);

            /// @brief 타이머 중지(스레드 종료)
            void stop();

            /// @brief 실행 중 여부
            bool isRunning() const;

        private:
            /// @brief 내부 스레드 루프(주기 수행)
            void timerLoop();

            std::thread worker;
            std::atomic<bool> running{ false };
            int interval{ 0 };
            std::function<void()> task;
        };

        /// @brief 자동 유지보수 작업 큐(백그라운드 실행)
        class DEEP_API AutoMaintenanceManager {
        public:
            /// @brief 싱글톤 인스턴스
            static AutoMaintenanceManager& getInstance() {
                static AutoMaintenanceManager instance;
                return instance;
            }

            /// @brief 워커 스레드 시작
            void start();

            /// @brief 워커 스레드 종료
            void stop();

            /// @brief 작업 추가(파일 정리 등)
            void enqueueTask(std::function<void()> task);

        private:
            AutoMaintenanceManager() = default;
            ~AutoMaintenanceManager() { stop(); }

            AutoMaintenanceManager(const AutoMaintenanceManager&) = delete;
            AutoMaintenanceManager& operator=(const AutoMaintenanceManager&) = delete;

            /// @brief 내부 스레드 루프(큐 소비)
            void threadLoop();

            std::deque<std::function<void()>> taskQueue;
            std::mutex queueMutex;
            std::condition_variable cv;
            std::atomic<bool> running{ false };
            std::thread workerThread;
        };

        class Atimer {
        public:
            using Callback = std::function<void()>;

            static Atimer& getInstance() {
                static Atimer instance;
                return instance;
            }

            // 핵심 API (OnTimer 대체)
            void start(std::chrono::milliseconds interval, Callback cb);

            void stop();

        private:
            Atimer() = default;
            ~Atimer() { stop(); }

            Atimer(const Atimer&) = delete;
            Atimer& operator=(const Atimer&) = delete;

            void threadLoop();

            std::thread worker;
            std::mutex mtx;
            std::condition_variable cv;
            std::atomic<bool> running{ false };

            std::chrono::milliseconds period{ 0 };
            Callback task;
        };
    } // namespace Utils


    // ===================== Net =====================
    namespace Net {

        /// @brief MinIO 파일 목록/업다운/상태 체크 클라이언트
        class DEEP_API MinIOClient {
        public:
            MinIOClient();
            ~MinIOClient();

            /// @brief 파일 목록 조회(프로젝트/라인/날짜/타입 기준)
            std::vector<std::string> getFileList(const std::string& project, const std::string& line, const std::string& date, const std::string& type, const std::string& bucket);

            /// @brief 폴더 다운로드(저장 경로 기준 트리 생성)
            bool downloadFolder(const std::string& project, const std::string& line, const std::string& date, const std::string& type, const std::string& bucket, const std::string& save_base_path);

            /// @brief 모델(onnx) 다운로드
            bool downloadModel();

            /// @brief 폴더 업로드(이미지/로그 등)
            bool uploadFolder(const std::string& folder_path, const std::string& project, const std::string& line, const std::string& date, const std::string& type, const std::string& bucket);

            /// @brief 파일 존재 확인(bucket/경로)
            bool checkFileExists(const std::string& bucket, const std::string& filename = "");

            /// @brief 헬스체크: 네트워크/Flask/MinIO/MLflow
            bool checkNetwork();
            bool checkFlask();
            bool checkMinio();
            bool checkMLflow();

        private:
            /// @brief URL 인코딩
            std::string url_encode(const std::string& str);

            /// @brief UTF-16 → UTF-8 변환
            std::string utf8_encode(const std::wstring& wstr);

            /// @brief ANSI → UTF-8 변환
            std::string ansi_to_utf8(const std::string& str);
        };

        /// @brief MinIO 비동기 작업 큐(단일 워커 스레드)
        class DEEP_API MinIOClientAsyncManager {
        public:
            /// @brief 싱글톤 인스턴스
            static MinIOClientAsyncManager& getInstance() {
                static MinIOClientAsyncManager instance;
                return instance;
            }

            /// @brief 워커 시작
            void start();

            /// @brief 워커 정지
            void stop();

            /// @brief 작업 추가(MinIOClient 주입 콜백)
            void enqueueTask(std::function<void(MinIOClient&)> task);

        private:
            MinIOClientAsyncManager() = default;
            ~MinIOClientAsyncManager() { stop(); }

            MinIOClientAsyncManager(const MinIOClientAsyncManager&) = delete;
            MinIOClientAsyncManager& operator=(const MinIOClientAsyncManager&) = delete;

            /// @brief 내부 스레드 루프(큐 소비)
            void threadLoop();

            std::deque<std::function<void(MinIOClient&)>> taskQueue;
            std::mutex queueMutex;
            std::condition_variable cv;
            std::atomic<bool> running{ false };
            std::thread workerThread;
        };

    } // namespace Net

} // namespace NSE
