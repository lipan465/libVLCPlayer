#pragma once
#include "MediaPlayerUI.h"
class CFrameWindowWnd : public DuiLib::WindowImplBase
{
	DUI_DECLARE_MESSAGE_MAP()
private:
	CMediaPlayerUI* m_player;
	CSliderUI* m_play_process;
	CLabelUI* m_label_play_time;
public:
	CFrameWindowWnd(void);
	~CFrameWindowWnd(void);
	void InitWindow();
	void OnFinalMessage( HWND hWnd );
	CControlUI* CreateControl(LPCTSTR pstrClass);
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName(void) const;

	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void OnClickOpenMedia(TNotifyUI & msg);
};

