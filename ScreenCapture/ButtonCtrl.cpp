#include "StdAfx.h"
#include "ButtonCtrl.h"


CButtonCtrl::CButtonCtrl(
	int nID, 
	int nPosX, 
	int nPosY, 
	int nWidth, 
	int nHeight, 	
	wchar_t* wszImgNormal, 
	wchar_t* wszImgHover, 
	wchar_t* wszImgDown,
	wchar_t* wszImgDisable,
	wchar_t* wszTitle,
	HWND hWndParent):m_pClickedHandle(NULL)
{
	m_nID		= nID;
	m_nPosX		= nPosX;
	m_nPosY		= nPosY;
	m_nWidth	= nWidth;
	m_nHeight	= nHeight;
	m_bDisable  = FALSE;
	m_bDown		= FALSE;
	m_bIn		= FALSE;
	m_bOut		= TRUE;

	m_hWndParent    = hWndParent;
	m_pImgHover		= Image::FromFile(wszImgHover);
	m_pImgNormal    = Image::FromFile(wszImgNormal);
	m_pImgDown      = Image::FromFile(wszImgDown);
	m_pImgDisable   = Image::FromFile(wszImgDisable);
	m_pImgCurrent   = m_pImgNormal;
	m_pOldImage		= m_pImgNormal;
	m_wszTitle = new wchar_t[wcslen(wszTitle) + 1];
	wcscpy(m_wszTitle, wszTitle);

	m_rectBtn.left		= m_nPosX;
	m_rectBtn.top		= m_nPosY;
	m_rectBtn.right		= m_rectBtn.left + m_nWidth;
	m_rectBtn.bottom	= m_rectBtn.top + m_nHeight;

	m_nType = Button;
}

CButtonCtrl::CButtonCtrl(
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
	HWND hWndParent):m_pClickedHandle(NULL)
{
	m_nID		= nID;
	m_nPosX		= nPosX;
	m_nPosY		= nPosY;
	m_nWidth	= nWidth;
	m_nHeight	= nHeight;
	m_bDisable  = FALSE;
	m_bDown		= FALSE;
	m_bIn		= FALSE;
	m_bOut		= TRUE;

	m_hWndParent    = hWndParent;
	ImageFromResource(nIDImgHover, _T("PNG"), &m_pImgHover);
	ImageFromResource(nIDImgNormal, _T("PNG"), &m_pImgNormal);
	ImageFromResource(nIDImgDown, _T("PNG"), &m_pImgDown);
	ImageFromResource(nIDImgDisable, _T("PNG"), &m_pImgDisable);

	m_pImgCurrent   = m_pImgNormal;
	m_pOldImage		= m_pImgNormal;
	m_wszTitle = new wchar_t[wcslen(wszTitle) + 1];
	wcscpy(m_wszTitle, wszTitle);

	m_rectBtn.left		= m_nPosX;
	m_rectBtn.top		= m_nPosY;
	m_rectBtn.right		= m_rectBtn.left + m_nWidth;
	m_rectBtn.bottom	= m_rectBtn.top + m_nHeight;

	m_nType = Button;
}

CButtonCtrl::~CButtonCtrl(void)
{
	delete m_pImgHover;
	delete m_pImgNormal;
	delete m_pImgDown;
	delete m_pImgDisable;
	delete [] m_wszTitle;
}

int CButtonCtrl::GetID()
{
	return m_nID;
}
void CButtonCtrl::Draw()
{
	assert(m_pGraphic != NULL);
	m_pGraphic->DrawImage(m_pImgCurrent, m_nPosX, m_nPosY, m_nWidth, m_nHeight);
	
	if(wcslen(m_wszTitle) != 0)
	{
		Gdiplus::Font myFont(L"微软雅黑", 10);
		
		PointF origin(m_nPosX, m_nPosY + 2);
		SolidBrush blackBrush(Color(254, 0, 0, 0));

		RectF  rect;
		rect.X = m_nPosX;
		rect.Y = m_nPosY;
		rect.Width = m_nWidth;
		rect.Height = m_nHeight;
		
		Gdiplus::StringFormat strFormat;
		strFormat.SetAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
		strFormat.SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);

		m_pGraphic->DrawString(m_wszTitle, wcslen(m_wszTitle), &myFont, rect, &strFormat, &blackBrush);
	}
}

void CButtonCtrl::SetGraphic(Graphics *g)
{
	m_pGraphic = g;
}

void CButtonCtrl::MouseEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt = {LOWORD(lParam), HIWORD(lParam)};
	if(!m_bDisable)
	{
		switch(uMsg)
		{
		case WM_MOUSEMOVE:
			{			
				if(PtInRect(&m_rectBtn, pt) == TRUE)
				{
					m_bIn = TRUE;
					m_bOut = FALSE;
				}
				else
				{
					m_bIn = FALSE;
					m_bOut = TRUE;
				}
			}
			break;
		case WM_LBUTTONDOWN:
			{
				if(PtInRect(&m_rectBtn, pt) == TRUE)
				{
					m_bDown = TRUE;
				}
				else
				{
					m_bDown = FALSE;
				}
			}
			break;
		case WM_LBUTTONUP:
			{
				if(PtInRect(&m_rectBtn, pt) == TRUE)
				{
					if(m_pClickedHandle)
					{
						m_pClickedHandle(m_hWndParent, (WPARAM)m_wszTitle, lParam);
					}
				}
				m_bDown = FALSE;
			}
			break;
		}
	}
	if(!m_bDisable)
	{
		if(m_bIn && m_bDown)//点击按钮
		{
			this->m_pImgCurrent = this->m_pImgDown;
		}
		else if(m_bIn && !m_bDown)//悬浮按钮之上
		{
			this->m_pImgCurrent = this->m_pImgHover;
		}
		else//在按钮区域外
		{
			this->m_pImgCurrent = this->m_pImgNormal;
		}
	}
	else
	{
		this->m_pImgCurrent = this->m_pImgDisable;
	}
	if(this->m_pImgCurrent != this->m_pOldImage)//界面有更新
	{
		this->m_pOldImage = this->m_pImgCurrent;
		SendMessage(m_hWndParent, WM_UPDATE, 1, 0);		
	}
}

void CButtonCtrl::RegisterClickedHandle(void (WINAPI *pClickedHandle)(HWND, WPARAM, LPARAM))
{
	this->m_pClickedHandle = pClickedHandle;
}

void CButtonCtrl::SetTextA(char* szText)
{
	int nConvert = MultiByteToWideChar(CP_ACP, 0, szText, -1, NULL, 0);
    wchar_t* wszDst = new wchar_t[nConvert + 1];
	memset(wszDst, 0, sizeof(wchar_t) * (nConvert + 1));
    MultiByteToWideChar(CP_ACP, 0, szText, -1, wszDst, nConvert);
	SetTextW(wszDst);
	delete [] wszDst;
}

void CButtonCtrl::SetTextW(wchar_t* wszText)
{
	delete [] m_wszTitle;
	m_wszTitle = new wchar_t[wcslen(wszText) + 1];
	wcscpy(m_wszTitle, wszText);
}

wchar_t* CButtonCtrl::GetText()
{
	return m_wszTitle;
}

void CButtonCtrl::Enable(bool bEnable)
{
	m_bDisable = !bEnable;
	MouseEvent(0,0,0);
}
