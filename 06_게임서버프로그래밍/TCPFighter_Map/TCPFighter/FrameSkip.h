#ifndef __FRAME_SKIP__
#define __FRAME_SKIP__

class CFrameSkip
{
public:
	CFrameSkip(int iMaxFPS);
	~CFrameSkip();

	BOOL FrameSkip(void);
	FLOAT CheckFPS(void);

private:
	DWORD	m_dwTick;
	//<
	DWORD	m_dwSystemTick;
	//<
	int		m_iMaxFPS;
	//<
	int		m_iOneFrameTick;
	//<
	int		m_iTick;


	BOOL	m_bSkipFalg;
	FLOAT	m_fCurFPS;
	FLOAT	m_fOldFPS;
	DWORD	m_dwFrameCount;
	FLOAT	m_fTimeElapsed;
	DWORD	m_dwLastTime;
	DWORD	m_dwCurTime;
	FLOAT	m_fTimeDelta;
};

#endif // !__FRAME_SKIP__