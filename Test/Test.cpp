// Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	HINSTANCE hInst = LoadLibrary(_T("ScreenCapture.dll"));
	if (hInst)
	{
		void(*p)(wchar_t*) = (void (*)(wchar_t*))GetProcAddress(hInst, "ScreenShot");
		p(L".");
	}
	return 0;
}

