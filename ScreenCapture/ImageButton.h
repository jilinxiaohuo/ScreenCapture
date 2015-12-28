#pragma once
#include "stdafx.h"
#include <vector>
#include <GdiPlus.h>
#include <tchar.h>
using namespace std;
using namespace Gdiplus;

extern HINSTANCE hInst;

class CImageButton
{	
	typedef void (*EVENTHANDLE)(HWND);
protected:
	

public:
	int m_nButtonID;
	int m_nPosX;
	int m_nPosY;
	int m_nWidth;
	int m_nHeight;
	
	LPTSTR m_pszResType;
	EVENTHANDLE m_pFunClick;

	int m_nImgHoverID;
	int m_nImgLeaveID;
	int m_nImgDownID;
	BOOL	m_fIn;
	BOOL	m_fOut;
	BOOL	m_fDown;
	int m_nCurrentResID;

	CImageButton(const CImageButton& tmp)
	{
		this->m_nButtonID = tmp.m_nButtonID;
		this->m_nPosX = tmp.m_nPosX;
		this->m_nPosY = tmp.m_nPosY;
		this->m_nWidth = tmp.m_nWidth;
		this->m_nHeight = tmp.m_nHeight;
		this->m_nImgHoverID = tmp.m_nImgHoverID;
		this->m_nImgLeaveID = tmp.m_nImgLeaveID;
		this->m_nImgDownID = tmp.m_nImgDownID;
		this->m_pFunClick = tmp.m_pFunClick;
		this->m_fIn = tmp.m_fIn;
		this->m_fOut = tmp.m_fOut;
		this->m_nCurrentResID = tmp.m_nCurrentResID;
		this->m_fDown = tmp.m_fDown;
		size_t len = _tcslen(tmp.m_pszResType);
		
		this->m_pszResType = new TCHAR[len + 1];
		_tcsncpy(this->m_pszResType, tmp.m_pszResType, len); 
		this->m_pszResType[len] = 0;
	}
	CImageButton& operator=(const CImageButton& tmp)
	{
		if((int)this == (int)&tmp)
		{
			return (*this);
		}
		delete [] m_pszResType;
		this->m_nButtonID = tmp.m_nButtonID;
		this->m_nPosX = tmp.m_nPosX;
		this->m_nPosY = tmp.m_nPosY;
		this->m_nWidth = tmp.m_nWidth;
		this->m_nHeight = tmp.m_nHeight;
		this->m_nImgHoverID = tmp.m_nImgHoverID;
		this->m_nImgLeaveID = tmp.m_nImgLeaveID;
		this->m_nImgDownID = tmp.m_nImgDownID;
		this->m_pFunClick = tmp.m_pFunClick;
		this->m_fIn = tmp.m_fIn;
		this->m_fOut = tmp.m_fOut;
		this->m_nCurrentResID = tmp.m_nCurrentResID;
		this->m_fDown = tmp.m_fDown;

		size_t len = _tcslen(tmp.m_pszResType);
		this->m_pszResType = new TCHAR[len + 1];
		_tcsncpy(this->m_pszResType, tmp.m_pszResType, len);
		this->m_pszResType[len] = 0;
		return (*this);
	}
	~CImageButton(void)
	{
		if(m_pszResType)
		{
			delete [] m_pszResType;
		}
	}

public:

	CImageButton()
	{
		this->m_pszResType = NULL;
		this->m_pFunClick = NULL;
	}

	int   GetButtonID()
	{
		return m_nButtonID;
	}
	RECT GetButtonRect()
	{
		RECT rc = {m_nPosX, m_nPosY, m_nWidth + m_nPosX, m_nHeight + m_nPosY};
		return rc;
	}
	
	CImageButton(int nButtonID, 
		int nPosX, 
		int nPosY, 
		int nWidth, 
		int nHeight, 
		int nImgHoverID, 
		int nImgLeaveID, 
		int nImgDownID,
		LPTSTR pszResType)
	{
		m_nButtonID = nButtonID;
		m_nPosX = nPosX;
		m_nPosY = nPosY;
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_nImgHoverID = nImgHoverID;
		m_nImgLeaveID = nImgLeaveID;
		m_nImgDownID = nImgDownID;
		m_fIn = TRUE;
		m_fOut = TRUE;
		m_nCurrentResID = m_nImgLeaveID;
		m_pFunClick = NULL;
		m_fDown = FALSE;
		size_t len = _tcslen(pszResType);
		this->m_pszResType = new TCHAR[len + 1];
		_tcsncpy(this->m_pszResType, pszResType, len + 1);
	}

	
	
	static void ImageFromResource(UINT uID, LPCTSTR pszResType, Image** ppImage)
	{
	//	HINSTANCE hInst =  GetModuleHandle(hInst);
		HRSRC hRsrc = FindResource(hInst, MAKEINTRESOURCE(uID), pszResType);
		if (!hRsrc) {
			return;
		}

		HGLOBAL hMemRsrc = LoadResource(hInst, hRsrc);
		BYTE * pMemRsrc = (BYTE *)LockResource(hMemRsrc);

		size_t sizeRsrc = SizeofResource(hInst, hRsrc);
		HGLOBAL hMemStrm = GlobalAlloc(GMEM_FIXED, sizeRsrc);
		//ASSERT(m_hMemStrm != NULL);
		if (!hMemStrm) {
			return;
		}

		BYTE * pMemStrm = (BYTE *)GlobalLock(hMemStrm);
		memcpy(pMemStrm, pMemRsrc, sizeRsrc);

		IStream * pStrm = NULL;
		if(CreateStreamOnHGlobal(pMemStrm, TRUE, &pStrm) == S_OK)
		{					
			*ppImage = Image::FromStream(pStrm);				
		}
		pStrm->Release();
	}

	void RegisterEventHandle(EVENTHANDLE pFun)
	{
		m_pFunClick = pFun;
	}

	EVENTHANDLE GetEventHandle()
	{
		return m_pFunClick;
	}

	void DrawImage(HWND hWnd, UINT nResID, BOOL bDrawBtnBkg = FALSE)
	{		
		HDC hdc = GetDC(hWnd);			
		RECT rect = GetButtonRect();		
		HDC hMemDC = ::CreateCompatibleDC(hdc);		
		HBITMAP hBmp = ::CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
		HGDIOBJ hOldBmp = SelectObject(hMemDC, hBmp);

		Graphics graphic(hMemDC);	


		Image *img = NULL;

		ImageFromResource(nResID, m_pszResType, &img);
		graphic.DrawImage(img, 0, 0, rect.right - rect.left, rect.bottom - rect.top);
		delete img;				

		BitBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, hMemDC, 0, 0, SRCCOPY);

		SelectObject(hMemDC, hOldBmp);
		DeleteObject(hBmp);
		DeleteObject(hOldBmp);
		DeleteObject(hMemDC);
		DeleteObject(hdc);
	}
};


class CImageButtonList
{
private:
	BOOL m_fDown;

	CImageButton m_btnCurrent;
	vector<CImageButton> m_vecButton;
public:
	CImageButtonList():m_fDown(false){
	}

	size_t GetButtonCount()
	{
		return m_vecButton.size();
	}

	void AddButton(CImageButton& btn)
	{
		m_vecButton.push_back(btn);
	}
	
	BOOL DispatchEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch(message)
		{
		case WM_MOUSEMOVE:
			{
				POINT point = {LOWORD(lParam), HIWORD(lParam)};
				CImageButton tmp;
				
				vector<CImageButton>::iterator it;
				for(it = m_vecButton.begin(); it != m_vecButton.end(); ++ it)
				{
					if(PtInRect(&it->GetButtonRect(), point))
					{
						if(it->m_fOut == TRUE)
						{
							if(it->m_fDown)
							{
								it->DrawImage(hWnd, it->m_nImgDownID);
							}
							else
							{
								it->DrawImage(hWnd, it->m_nImgHoverID);
							}
							it->m_nCurrentResID = it->m_nImgHoverID;
						}
						
						it->m_fIn = TRUE;
						it->m_fOut = FALSE;
					}
					else
					{
						if(it->m_fIn == TRUE)
						{
							it->DrawImage(hWnd, it->m_nImgLeaveID);
							it->m_nCurrentResID = it->m_nImgLeaveID;
						}
						it->m_fIn = FALSE;
						it->m_fOut = TRUE;
					}
					
				}
				if(m_btnCurrent.m_pszResType != NULL && m_btnCurrent.m_nButtonID != 4)
				{
					m_btnCurrent.DrawImage(hWnd, m_btnCurrent.m_nImgDownID);
				}
			}
			return FALSE;
		case WM_LBUTTONDOWN:
			{
				POINT point = {LOWORD(lParam), HIWORD(lParam)};
				vector<CImageButton>::iterator it;
				for(it = m_vecButton.begin(); it != m_vecButton.end(); ++ it)
				{
					if(PtInRect(&it->GetButtonRect(), point))
					{
						it->DrawImage(hWnd, it->m_nImgDownID);
						it->m_nCurrentResID = it->m_nImgDownID;
						it->m_fDown = true;
						return TRUE;
					}
				}			
			}
			return FALSE;
		case WM_LBUTTONUP:
			{
				POINT point = {LOWORD(lParam), HIWORD(lParam)};
				vector<CImageButton>::iterator it;
				for(it = m_vecButton.begin(); it != m_vecButton.end(); ++ it)
				{
					if(PtInRect(&it->GetButtonRect(), point))
					{					
						if(it->m_fDown)
						{
							it->DrawImage(hWnd, it->m_nImgHoverID);
							it->m_nCurrentResID = it->m_nImgHoverID;
					
							if(it->GetEventHandle())
							{
								it->GetEventHandle()(hWnd);
								m_btnCurrent.DrawImage(hWnd, m_btnCurrent.m_nImgLeaveID);
								m_btnCurrent = (*it);
								it->m_fDown = false;
								break;
							}
						}	
						
					}
					else
					{
						it->m_fDown = false;
					}
					
				}	
			}
			return FALSE;
		case WM_PAINT:
			{
				HDC hdc = GetDC(hWnd);
				RECT rect;
				GetClientRect(hWnd, &rect);
				
				HDC hMemDC = ::CreateCompatibleDC(hdc);		
				HBITMAP hBmp = ::CreateCompatibleBitmap(hdc, rect.right - rect.left,26);
				HGDIOBJ hOldBmp = SelectObject(hMemDC, hBmp);

				Graphics graphic(hMemDC);

				vector<CImageButton>::iterator it;
				Image *img = NULL;

				//ÌùButtonBar±³¾°
				CImageButton::ImageFromResource(IDB_BUTTONBAR, _T("PNG"), &img);
				graphic.DrawImage(img, 0, 0, rect.right - rect.left, 26);
				delete img;

				for(it = m_vecButton.begin(); it != m_vecButton.end(); ++ it)
				{
					it->ImageFromResource(it->m_nCurrentResID, it->m_pszResType, &img);
					graphic.DrawImage(img, it->m_nPosX, it->m_nPosY, it->m_nWidth, it->m_nHeight);
					delete img;
				}
							
				BitBlt(hdc, 0, 0, rect.right - rect.left, 26, hMemDC, 0, 0, SRCCOPY);
		
				SelectObject(hMemDC, hOldBmp);
				DeleteObject(hBmp);
				DeleteObject(hOldBmp);
				DeleteObject(hMemDC);
				DeleteObject(hdc);
			}
			return FALSE;
		default:
			return TRUE;
		}
	}
};

