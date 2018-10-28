#pragma once
class CMediaPlayerUI : public CControlUI
{
private:
	libvlc_instance_t * m_vlc_inst;  
	libvlc_media_player_t *m_vlc_player;  
	libvlc_media_t *m_vlc_media;
	void* m_pWindow;
	HBITMAP m_hBkbitmap;
	LPBYTE m_pDest;
	CRITICAL_SECTION m_cs_dest;
	LPBYTE m_pDecodedBuf;
	DWORD m_pixel_count;

	static void *my_libvlc_video_lock_cb(void *opaque, void **planes);
	static void my_libvlc_video_unlock_cb(void *opaque, void *picture,void *const *planes);
	static void my_libvlc_video_display_cb(void *opaque, void *picture);
public:
	CMediaPlayerUI(void);
	~CMediaPlayerUI(void);
	void Init();
	void SetPos(RECT rc, bool bNeedInvalidate);
	void PaintBkImage(HDC hDC);

	bool Play(LPCTSTR mediaPath);
	bool Play2(LPCTSTR mediaPath);
	bool OpenCamera();
	float GetPercent();
	int GetLength();
};

