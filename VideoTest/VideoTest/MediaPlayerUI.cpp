#include "StdAfx.h"
#include "MediaPlayerUI.h"

// Unicode to UTF-8    
std::string UnicodeToUTF8( const std::wstring& str )
{
	char*     pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte( CP_UTF8,
		0,
		str.c_str(),
		-1,
		NULL,
		0,
		NULL,
		NULL );
	pElementText = new char[iTextLen + 1];
	memset( ( void* )pElementText, 0, sizeof( char ) * ( iTextLen + 1 ) );
	::WideCharToMultiByte( CP_UTF8,
		0,
		str.c_str(),
		-1,
		pElementText,
		iTextLen,
		NULL,
		NULL );
	std::string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}

class CMediaPlayerWnd : public CWindowWnd
{
public:
	CMediaPlayerWnd();
	void Init(CMediaPlayerUI* pOwner);
	RECT CalPos();
	LPCTSTR GetWindowClassName() const;
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	CMediaPlayerUI* m_pOwner;
};

CMediaPlayerWnd::CMediaPlayerWnd() : m_pOwner(NULL)
{
}

void CMediaPlayerWnd::Init(CMediaPlayerUI* pOwner)
{
	m_pOwner = pOwner;
	RECT rcPos = CalPos();
	Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, UI_WNDSTYLE_CHILD, 0, rcPos);
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_BORDER;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	::ShowWindow(m_hWnd, SW_SHOW);
}

LPCTSTR CMediaPlayerWnd::GetWindowClassName() const
{
	return _T("CMediaPlayerWnd");
}

RECT CMediaPlayerWnd::CalPos()
{
	CDuiRect rcPos = m_pOwner->GetPos();
	CControlUI* pParent = m_pOwner;
	RECT rcParent;
	while( pParent = pParent->GetParent() ) {
		if( !pParent->IsVisible() ) {
			rcPos.left = rcPos.top = rcPos.right = rcPos.bottom = 0;
			break;
		}
		rcParent = pParent->GetClientPos();
		if( !::IntersectRect(&rcPos, &rcPos, &rcParent) ) {
			rcPos.left = rcPos.top = rcPos.right = rcPos.bottom = 0;
			break;
		}
	}
	return rcPos;
}

LRESULT CMediaPlayerWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	if( uMsg == WM_PAINT )
	{
		PAINTSTRUCT ps = { 0 };
		HDC hDC = ::BeginPaint(m_hWnd, &ps);
		::RECT rect;
		::GetClientRect(m_hWnd, &rect);
		CRenderEngine::DrawColor(hDC, rect, 0xffbebebe);
		::EndPaint(m_hWnd, &ps);
	}
	else
	{
		bHandled = FALSE;
	}

	if( !bHandled ) return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return lRes;
}

CMediaPlayerUI::CMediaPlayerUI(void)
{
	m_pWindow = NULL;
	m_vlc_inst = libvlc_new (0, NULL);
	m_vlc_player = NULL;
	m_vlc_media = NULL;
	m_hBkbitmap = NULL;
	m_pDest = NULL;
	m_pDecodedBuf = NULL;
	InitializeCriticalSection (&m_cs_dest);
}


CMediaPlayerUI::~CMediaPlayerUI(void)
{
	if(m_pWindow) delete m_pWindow;
	if(m_vlc_player) libvlc_media_player_release (m_vlc_player);
	if(m_vlc_inst) libvlc_release (m_vlc_inst);
	if(m_pDecodedBuf) free(m_pDecodedBuf);
	if(m_hBkbitmap) DeleteObject(m_hBkbitmap);
	DeleteCriticalSection(&m_cs_dest);
}

void CMediaPlayerUI::Init()
{
	if( m_pWindow == NULL )
	{
		//m_pWindow = new CMediaPlayerWnd();
		//((CMediaPlayerWnd*)m_pWindow)->Init(this);
	}
}

void CMediaPlayerUI::SetPos(RECT rc, bool bNeedInvalidate)
{
	CControlUI::SetPos(rc,bNeedInvalidate);
	if( m_pWindow != NULL ) 
	{
		RECT rcPos = ((CMediaPlayerWnd*)m_pWindow)->CalPos();
		::SetWindowPos(((CMediaPlayerWnd*)m_pWindow)->GetHWND(), NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left, 
			rcPos.bottom - rcPos.top, SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

bool CMediaPlayerUI::Play2(LPCTSTR mediaPath)
{
	if(m_vlc_player != NULL && libvlc_media_player_is_playing(m_vlc_player))
	{
		libvlc_media_player_stop(m_vlc_player);
	}
	m_vlc_media = libvlc_media_new_location (m_vlc_inst, UnicodeToUTF8(mediaPath).c_str());
	if(m_vlc_media == NULL) return false;
	m_vlc_player = libvlc_media_player_new_from_media (m_vlc_media);
	if(m_vlc_player == NULL) return false;
	libvlc_media_release (m_vlc_media);
	libvlc_media_player_set_hwnd (m_vlc_player, ((CMediaPlayerWnd*)m_pWindow)->GetHWND());
	libvlc_media_player_set_rate(m_vlc_player,30);
	if( 0 != libvlc_media_player_play (m_vlc_player) ) return false;
}

bool CMediaPlayerUI::Play(LPCTSTR mediaPath)
{
	if(m_vlc_player != NULL && libvlc_media_player_is_playing(m_vlc_player))
	{
		libvlc_media_player_stop(m_vlc_player);
	}
	m_vlc_media = libvlc_media_new_path (m_vlc_inst, UnicodeToUTF8(mediaPath).c_str());
	if(m_vlc_media == NULL) return false;
	m_vlc_player = libvlc_media_player_new_from_media (m_vlc_media);
	if(m_vlc_player == NULL) return false;
	libvlc_media_release (m_vlc_media);
	libvlc_media_player_set_hwnd (m_vlc_player, ((CMediaPlayerWnd*)m_pWindow)->GetHWND());
	if( 0 != libvlc_media_player_play (m_vlc_player) ) return false;

	//libvlc_media_player_get_length(m_vlc_player);
	//libvlc_media_player_get_position(m_vlc_player);
	return true;
}

void* CMediaPlayerUI::my_libvlc_video_lock_cb(void *opaque, void **planes)
{
	CMediaPlayerUI* me = (CMediaPlayerUI*)opaque;
	EnterCriticalSection(&me->m_cs_dest);
	*planes = me->m_pDecodedBuf;
	return me->m_pDecodedBuf;
}
void CMediaPlayerUI::my_libvlc_video_unlock_cb(void *opaque, void *picture,void *const *planes)
{
	CMediaPlayerUI* me = (CMediaPlayerUI*)opaque;
	if(me->m_pDest)
	{
		unsigned char* p = (unsigned char*)picture;
		unsigned char* q = (unsigned char*)me->m_pDest;
		for(int i=0;i<me->m_pixel_count;i++)
		{
			*q++ = *(p + 2);
			*q++ = *(p + 1);
			*q++ = *(p + 0);
			*q++ = *(p + 3);
			p += 4;
		}
	}
	LeaveCriticalSection(&me->m_cs_dest);
}
void CMediaPlayerUI::my_libvlc_video_display_cb(void *opaque, void *picture)
{
	CMediaPlayerUI* me = (CMediaPlayerUI*)opaque;
	me->Invalidate();
}

void CMediaPlayerUI::PaintBkImage(HDC hDC)
{
	RECT rcCorner = { 0 };
	RECT rcPart = { 0,0,640,480 };
	if(NULL == m_hBkbitmap)
	{
		BITMAPINFO bmi;
		::ZeroMemory(&bmi, sizeof(BITMAPINFO));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = 640;
		bmi.bmiHeader.biHeight = -480;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage = 640 * 480 * 4;

		m_pixel_count = 640 * 480;
		EnterCriticalSection(&m_cs_dest);
		m_hBkbitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&m_pDest, NULL, 0);
		LeaveCriticalSection(&m_cs_dest);
		if( !m_hBkbitmap ) {
			//::MessageBox(0, _T("CreateDIBSectionÊ§°Ü"), _T("×¥BUG"), MB_OK);
			return ;
		}
	}
	CRenderEngine::DrawImage(hDC,m_hBkbitmap,m_rcItem, m_rcPaint,rcPart,rcCorner,false);
}

bool CMediaPlayerUI::OpenCamera()
{
	if(m_vlc_player != NULL && libvlc_media_player_is_playing(m_vlc_player))
	{
		libvlc_media_player_stop(m_vlc_player);
	}
	m_vlc_media = libvlc_media_new_location (m_vlc_inst, "dshow://");
	if(m_vlc_media == NULL) return false;
	libvlc_media_add_option(m_vlc_media,":dshow-size=640*480");
	libvlc_media_add_option(m_vlc_media,":live-caching=0");
	m_vlc_player = libvlc_media_player_new_from_media (m_vlc_media);
	if(m_vlc_player == NULL) return false;
	libvlc_media_release (m_vlc_media);
	//libvlc_media_player_set_hwnd (m_vlc_player, ((CMediaPlayerWnd*)m_pWindow)->GetHWND());
	//libvlc_video_set_callbacks(m_vlc_player,my_libvlc_video_lock_cb,my_libvlc_video_unlock_cb,my_libvlc_video_display_cb,this);
	m_pDecodedBuf = (LPBYTE)malloc(640 * 480 * 4);
	libvlc_video_set_callbacks(m_vlc_player,my_libvlc_video_lock_cb,my_libvlc_video_unlock_cb,my_libvlc_video_display_cb,this);
	libvlc_video_set_format(m_vlc_player, "RGBA", 640, 480,640*4);
	if( 0 != libvlc_media_player_play (m_vlc_player) ) return false;

	
	return true;
}

float CMediaPlayerUI::GetPercent()
{
	if(m_vlc_player)
	{
		return libvlc_media_player_get_position(m_vlc_player);
	}
	else
	{
		return 0;
	}
}

int CMediaPlayerUI::GetLength()
{
	if(m_vlc_player)
	{
		return libvlc_media_player_get_length(m_vlc_player);
	}
	else
	{
		return 0;
	}
}