#pragma once

#include "resource.h"
#include <Windows.h>
#include <comdef.h>
#include <GdiPlus.h>
#include <process.h>
#include <vector>
#include "ImageButton.h"
using namespace Gdiplus;
using namespace std;
#pragma comment(lib, "gdiplus.lib")

#define EXPORT_API __declspec(dllexport)
#define SAFE_RELEASE(p) if(p){DeleteObject(p);}
CImageButtonList   btnList;
extern HINSTANCE   hInst;
HDC			hdcMem;
HDC			hdcTemp;
HDC			hdcFinish;
HDC			hdcPaint;
int			nWidth;
int			nHeight;
bool		bDown;
bool		bSelected;
bool		bDrawRect;
bool		bDrawText;
bool		bDrawLine;
POINT		ptBegin;
POINT		ptEnd;
POINT		ptRectBegin;
POINT		ptRectEnd;
POINT		ptTextBegin;
POINT		ptTextEnd;
POINT       ptOld;
POINT		ptCurrent;
HPEN		hPen;
HBITMAP		hBitmap;
HBITMAP		hBmpTemp;
HBITMAP		hBmpFinish; 
HBITMAP		hBmpPaint;
HWND		hWndEdit;
HANDLE		hEvent;
wchar_t		szText[1024];
wchar_t		wszFilePath[MAX_PATH];

IStream*	pStream;
GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR			gdiplusGdiToken;
Image*				pImage = NULL;
HWND				hWndCommandBar;

#define WM_FINISHED WM_USER + 1
#define WM_DRAWRECT WM_USER + 2
#define WM_DRAWTEXT WM_USER + 3
#define WM_DRAWLINE	WM_USER + 4
// Forward declarations of functions included in this code module:

INT_PTR CALLBACK	CaptureProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	CommandBarProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

HBITMAP				CreateCompatibleBitmap(RECT* rcClient);
BOOL				SaveHDCToFile(HDC hDC, LPRECT lpRect);
VOID				ImageFromResource(UINT uID, LPCTSTR pszResType, Image** ppImage);
VOID				InitDialog(HWND hDlg);
VOID				AbortCapture(HWND hDlg);
VOID				Draw(HWND hDlg, HDC hdc);
VOID				OnLButtonDown(HWND hDlg, WPARAM wParam, LPARAM lParam);
VOID				OnLButtonUp(HWND hDlg, WPARAM wParam, LPARAM lParam);
VOID				OnDoubleClick(HWND hDlg, WPARAM wParam, LPARAM lParam);
VOID				OnMouseMove(HWND hDlg, WPARAM wParam, LPARAM lParam);
VOID				DrawText(HWND hDlg, WPARAM wParam, LPARAM lParam);
VOID				DrawLine(HWND hDlg, WPARAM wParam, LPARAM lParam);

VOID				CommandFinished(HWND hDlg);
VOID				CommandDrawRect(HWND hDlg);
VOID				CommandDrawLine(HWND hDlg);
VOID				CommandDrawText(HWND hDlg);

extern "C" EXPORT_API	void ScreenShot(wchar_t* wszPath);