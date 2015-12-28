#pragma once
#include "ControlBase.h"
#include "controlbase.h"
class CButtonCtrl :
	public CControlBase
{
private:
	int m_nID;
	int m_nPosX;
	int m_nPosY;
	int m_nWidth;
	int m_nHeight;
	wchar_t* m_wszTitle;
	void (WINAPI *m_pClickedHandle)(HWND, WPARAM, LPARAM);
	Image* m_pImgHover;
	Image* m_pImgNormal;
	Image* m_pImgDown;
	Image* m_pImgDisable;
	Image* m_pImgCurrent;
	Image* m_pOldImage;

	BOOL	m_bIn;
	BOOL	m_bOut;
	BOOL	m_bDown;
	BOOL	m_bDisable;

	RECT	m_rectBtn;

	Graphics* m_pGraphic;
	HWND	  m_hWndParent;

public:
	CButtonCtrl(int nID, 
	int nPosX, 
	int nPosY, 
	int nWidth, 
	int nHeight, 	
	wchar_t* wszImgNormal, 
	wchar_t* wszImgHover, 
	wchar_t* wszImgDown,
	wchar_t* wszImgDisable,
	wchar_t* wszTitle,
	HWND hWndParent);

	CButtonCtrl(
	int nID, 
	int nPosX, 
	int nPosY, 
	int nWidth, 
	int nHeight, 	
	int nIDImgNormal, 
	int nIDImgHover, 
	int nIDImgDown,
	int nIDImgDisable,
	wchar_t* wszTitle,
	HWND hWndParent);

	~CButtonCtrl(void);

public:
	virtual int GetID();
	virtual void Draw();
	virtual void SetGraphic(Graphics *g);
	void MouseEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void RegisterClickedHandle(void (WINAPI *)(HWND, WPARAM, LPARAM));
	void SetTextW(wchar_t* wszText);
	void SetTextA(char* szText);
	void Enable(bool bEnable);
	wchar_t* GetText();
};

