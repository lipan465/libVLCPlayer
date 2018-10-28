// VideoTest.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "VideoTest.h"
#include "FrameWindowWnd.h"


int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	//_CrtSetBreakAlloc(429);
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));
	//CPaintManagerUI::SetResourceZip(CPaintManagerUI::GetInstancePath() + _T("skin.zip"));
	//HINSTANCE hDll=::LoadLibrary(CPaintManagerUI::GetInstancePath() + _T("recourse_pts.dll"));
	//if(hDll)
	//{
	//	CPaintManagerUI::SetResourceDll(hDll);
	//	HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceDll(), _T("RT_SKIN"), _T("RT_ZIP"));
	//	if( hResource != NULL ) {
	//		DWORD dwSize = 0;
	//		HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceDll(), hResource);
	//		if( hGlobal != NULL ) {
	//			dwSize = ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource);
	//			if( dwSize > 0 ) {
	//				CPaintManagerUI::SetResourceZip((LPBYTE)::LockResource(hGlobal), dwSize);
	//			}
	//		}
	//		::FreeResource(hResource);
	//	}
	//}

	CFrameWindowWnd* pFrame = new CFrameWindowWnd(); 

	if( pFrame == NULL ) return 0; 

	pFrame->Create(NULL, _T("MultiPlatformPTS"), UI_WNDSTYLE_FRAME, 0, 0, 0, 0, 0, NULL); 
	pFrame->SetIcon(IDI_VIDEOTEST);

	pFrame->ShowWindow(true);
	CPaintManagerUI::MessageLoop();
	CControlFactory::GetInstance()->Release();
	CResourceManager::GetInstance()->Release();
	CPaintManagerUI::RemoveAllShareResource();
	CPaintManagerUI::Term();
	delete pFrame;
	//FreeLibrary(hDll);
	return 0;
}