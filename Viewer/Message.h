#pragma once
#include "stdafx.h"
#include <vector>

using namespace std;

const CString csDirPath = _T("D:\\nse-temp\\image\\");

const std::vector<CString> vMsgSavePath{
	_T("저장할 경로를 선택하세요."),
	_T("Select the folder to save."),
	_T("选择存储路径."),
	_T("Выберите папку для сохранения."),
	_T("Виберіть папку для збереження."),
	_T("Seleccione la carpeta para guardar."),
	_T("Wybierz folder do zapisania."),
	_T("選取要儲存的資料夾.")
};

const std::vector<CString> vMsgSaveTotal{
	_T("총 %d개 파일 복사가 완료되었습니다"),
	_T("Completed to copy %d files"),
	_T("%d个文件复制完成"),
	_T("Завершено копирование%d файлов"),
	_T("Завершено для копіювання %d файлів"),
	_T("Completado para copiar %d archivos"),
	_T("Ukończono, aby skopiować %d plików"),
	_T("已完成以複製 %d 個檔案")
};

const std::vector<CString> vMsgSaveFail{
	_T("총 %d개의 파일 중 %d개의 파일 복사가 실패하였습니다.")
	_T("Failed to copy %d files / %d total."),
	_T("%d/%d个文件复制失败"),
	_T("Не удалось скопировать %d файлов / %d всего"),
	_T("Не вдалося скопіювати %d файлів / %d всього."),
	_T("Completado para copiar %d archivos / %d total"),
	_T("Nie udało się skopiować %d plików / łącznie %d"),
	_T("無法複製 %d 個檔案 / %d 個總計")
};

const std::vector<CString> vMsgSaveImage{
	_T("저장된 영상이 없습니다."),
	_T("There is no saved image."),
	_T("没有保存的图像。"),
	_T("Нет сохраненного изображения."),
	_T("Збереженого зображення немає.。"),
	_T("No hay imagen guardada."),
	_T("Nie ma zapisanego obrazu."),
	_T("沒有儲存的映像檔.")
};

const std::vector<CString> vFilename{
	_T("D:\\Database\\daychart2.htm"),
	_T("D:\\Database\\daychart2_eng.htm"),
	_T("D:\\Database\\daychart2_chi.htm"),
	_T("D:\\Database\\daychart2_rus.htm"),
	_T("D:\\Database\\daychart2_ukr.htm"),
	_T("D:\\Database\\daychart2_spn.htm"),
	_T("D:\\Database\\daychart2_pol.htm"),
	_T("D:\\Database\\daychart2_tiw.htm")
};