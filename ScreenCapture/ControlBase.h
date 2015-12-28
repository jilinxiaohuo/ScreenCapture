#pragma once
#include <comdef.h>
#include <GdiPlus.h>
using namespace Gdiplus;
#include <vector>
#include <assert.h>

#define WM_UPDATE				  WM_USER + 1	
#define UPDATE_DELAY			  100		  
enum CtrlType{Button, Progress, Label};
	

class CControlBase
{
public:
	CControlBase(void);
	virtual ~CControlBase(void);
public:

	int m_nType;
	
public:
	virtual void Draw() = 0;
	virtual void SetGraphic(Graphics *g) = 0;
	virtual int  GetID() = 0;
	VOID ImageFromResource(UINT uID, LPCTSTR pszResType, Image** ppImage);
};


class CControlSet
{
private:
	static std::vector<CControlBase*>		m_vecCtrl;
	Image* m_pImage;
	int	   m_nWidth;
	int    m_nHeight;
	DWORD  m_nOldTick;
	bool   m_bFirstTime;
	HWND   m_hWndParent;
	static GdiplusStartupInput  m_gdiplusStartupInput;
	static ULONG_PTR			m_pGdiToken;
	HDC	   m_hDC;
	HDC    m_hMemDC;
	HBITMAP m_hBitmap;
private:
	void DrawControls(HDC hMemDC, Graphics* g);
	void InitGdiplus();
public:
	CControlSet(HWND hWndParent, wchar_t* wszBackgroundImage);	
	CControlSet(HWND hWnd, int nIDBackgroundImage);
	~CControlSet();
	VOID ImageFromResource(UINT uID, LPCTSTR pszResType, Image** ppImage);
public:
	void AddControl(CControlBase* pCtrl);
	template <typename T>
	T*	GetControl(int nID)
	{
		std::vector<CControlBase*>::iterator it;
		for(it = m_vecCtrl.begin(); it != m_vecCtrl.end(); ++it)
		{
			if(((T*)(*it))->GetID() == nID)
			{
				return dynamic_cast<T*>(*it);
			}
		}
		return NULL;
	}
	void UpdateWindow(HWND hWnd);
	void UpdateWindowWithoutDelay(HWND hWnd);

public:
	static void DispatchEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

