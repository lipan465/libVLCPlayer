#include "StdAfx.h"
#include "FrameWindowWnd.h"
#include "commdlg.h"

DUI_BEGIN_MESSAGE_MAP(CFrameWindowWnd, WindowImplBase)
	DUI_ON_CLICK_CTRNAME(_T("btn_media_open"), CFrameWindowWnd::OnClickOpenMedia)
DUI_END_MESSAGE_MAP()

CFrameWindowWnd::CFrameWindowWnd(void)
{
}


CFrameWindowWnd::~CFrameWindowWnd(void)
{
}

void CFrameWindowWnd::OnFinalMessage( HWND hWnd )
{
	PostQuitMessage(0);
}
CDuiString CFrameWindowWnd::GetSkinFile()
{
	return _T("MainWnd.xml");
}

LPCTSTR CFrameWindowWnd::GetWindowClassName(void) const
{
	return _T("CFrameWindowWnd");
}

void CFrameWindowWnd::InitWindow()
{
	m_player = static_cast<CMediaPlayerUI*>(m_PaintManager.FindControl(_T("player_1")));
	m_play_process = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("slider_play_process")));
	m_label_play_time = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lbl_play_time")));

	m_play_process->SetMaxValue(100);
	//m_player->Play2(_T("dshow://"));
	CenterWindow();
	::SetTimer(m_hWnd,8007,80,NULL);
}

CControlUI* CFrameWindowWnd::CreateControl(LPCTSTR pstrClass)
{
	if(_tcscmp(pstrClass,_T("MediaPlayer")) == 0) return new CMediaPlayerUI();
	else return NULL;
}

LRESULT CFrameWindowWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = false;
	switch(uMsg)
	{
		case WM_TIMER:
			{
				float percent = m_player->GetPercent();
				int length = m_player->GetLength()/1000;
				int pos = length*percent;
				CDuiString str;
				str.Format(_T("%02d:%02d:%02d/%02d:%02d:%02d"),pos/3600,pos%3600/60,pos%60,length/3600,length%3600/60,length%60);
				m_label_play_time->SetText(str);
				m_play_process->SetValue(percent*100);
			}
			break;
	}

	return 0;
}

void CFrameWindowWnd::OnClickOpenMedia(TNotifyUI & msg)
{
	//TCHAR szBuffer[MAX_PATH] = {0}; 
	//OPENFILENAME ofn= {0}; 
	//ofn.lStructSize = sizeof(ofn); 
	//ofn.hwndOwner = m_hWnd; 
	//ofn.lpstrFilter = _T("all file (*.*)\0*.*\0");
	//ofn.lpstrTitle = _T("Please select");
	//ofn.lpstrFile = szBuffer;
	//ofn.nMaxFile = MAX_PATH; 
	//ofn.nFilterIndex = 1;
	//ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER ;//标志如果是多选要加上OFN_ALLOWMULTISELECT

	//if(GetOpenFileName(&ofn))
	//{
	//	m_player->Play(szBuffer);
	//}
	m_player->OpenCamera();
}