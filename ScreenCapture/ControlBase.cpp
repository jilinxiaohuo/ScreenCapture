#include "StdAfx.h"
#include "ControlBase.h"
#include "ButtonCtrl.h"

extern HINSTANCE hInst;
CControlBase::CControlBase(void)
{
	
}

VOID CControlBase::ImageFromResource(UINT uID, LPCTSTR pszResType, Image** ppImage)
{
	//HINSTANCE hInst =  GetModuleHandle(L"ScreenCapture.dll");
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

CControlBase::~CControlBase(void)
{
}


GdiplusStartupInput  CControlSet::m_gdiplusStartupInput;
ULONG_PTR			 CControlSet::m_pGdiToken = NULL;

std::vector<CControlBase*>		CControlSet::m_vecCtrl;

CControlSet::CControlSet(HWND hWnd, wchar_t* wszBackgroundImage)
{
	if(m_pGdiToken == NULL)
	{
		InitGdiplus();
	}
	this->m_hWndParent = hWnd;
	this->m_pImage = Image::FromFile(wszBackgroundImage);
	assert(m_pImage != NULL);
	this->m_nWidth = m_pImage->GetWidth();
	this->m_nHeight = m_pImage->GetHeight();

	m_bFirstTime = true;
	m_nOldTick = GetTickCount();

	m_hDC		= GetDC(hWnd);
	m_hMemDC	= CreateCompatibleDC(m_hDC);
	m_hBitmap	= CreateCompatibleBitmap(m_hDC, m_nWidth, m_nHeight);
	SelectObject(m_hMemDC, m_hBitmap);	

	Graphics g(m_hMemDC);
	g.DrawImage(m_pImage, 0, 0, m_nWidth, m_nHeight);	
}

VOID CControlSet::ImageFromResource(UINT uID, LPCTSTR pszResType, Image** ppImage)
{
	//HINSTANCE hInst =  GetModuleHandle(L"ScreenCapture.dll");
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

CControlSet::CControlSet(HWND hWnd, int nIDBackgroundImage)
{
	if(m_pGdiToken == NULL)
	{
		InitGdiplus();
	}
	this->m_hWndParent = hWnd;
	
	ImageFromResource(nIDBackgroundImage, _T("PNG"), &this->m_pImage);
	assert(m_pImage != NULL);
	this->m_nWidth = m_pImage->GetWidth();
	this->m_nHeight = m_pImage->GetHeight();

	m_bFirstTime = true;
	m_nOldTick = GetTickCount();

	m_hDC		= GetDC(hWnd);
	m_hMemDC	= CreateCompatibleDC(m_hDC);
	m_hBitmap	= CreateCompatibleBitmap(m_hDC, m_nWidth, m_nHeight);
	SelectObject(m_hMemDC, m_hBitmap);	

	Graphics g(m_hMemDC);
	g.DrawImage(m_pImage, 0, 0, m_nWidth, m_nHeight);	
}

CControlSet::~CControlSet()
{
	delete m_pImage;
	std::vector<CControlBase*>::iterator it = m_vecCtrl.begin();
	for(; it!= m_vecCtrl.end(); ++it)
	{
		delete (*it);
	}
	DeleteObject(m_hMemDC);
	DeleteObject(m_hDC);
	DeleteObject(m_hBitmap);
}

void CControlSet::InitGdiplus()
{
	GdiplusStartup(&m_pGdiToken, &m_gdiplusStartupInput, NULL);
}

void CControlSet::DrawControls(HDC hMemDC, Graphics* g)
{
	std::vector<CControlBase*>::iterator it;
	for(it = m_vecCtrl.begin(); it != m_vecCtrl.end(); ++it)
	{
		//¶àÌ¬
		(*it)->SetGraphic(g);
		(*it)->Draw();
	}	
}

void CControlSet::DispatchEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	std::vector<CControlBase*>::iterator it;
	for(it = m_vecCtrl.begin(); it != m_vecCtrl.end(); ++it)
	{
		if((*it)->m_nType == Button)
		{
			((CButtonCtrl*)(*it))->MouseEvent(uMsg, wParam, lParam);
		}
	}
	
}

void CControlSet::AddControl(CControlBase* pCtrl)
{
	m_vecCtrl.push_back(pCtrl);
}

void CControlSet::UpdateWindow(HWND hWnd)
{
	if(m_bFirstTime || GetTickCount() - m_nOldTick > UPDATE_DELAY)
	{
		m_nOldTick = GetTickCount();
		m_bFirstTime = false;
		UpdateWindowWithoutDelay(hWnd);
	}
}

void CControlSet::UpdateWindowWithoutDelay(HWND hWnd)
{
	HDC hdc     = GetDC(hWnd);
	HDC hMemDC	= CreateCompatibleDC(hdc);
	HBITMAP hBitmap	= CreateCompatibleBitmap(m_hDC, m_nWidth, m_nHeight);

	SelectObject(hMemDC, hBitmap);

	BitBlt(hMemDC, 0, 0, m_nWidth, m_nHeight, m_hMemDC, 0, 0 ,SRCCOPY);
	Graphics g(hMemDC);

	DrawControls(hMemDC, &g);

	BitBlt(hdc, 0, 0, m_nWidth, m_nHeight, hMemDC, 0, 0, SRCCOPY);

	DeleteObject(hMemDC);
	DeleteObject(hdc);
	DeleteObject(hBitmap);
}