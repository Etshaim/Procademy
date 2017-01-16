#include "stdafx.h"
#include "FrameSkip.h"

CFrameSkip::CFrameSkip(int iMaxFPS)
{
	m_dwTick = 0;
	m_dwSystemTick = 0;
	m_iMaxFPS = iMaxFPS;

	//----------------------------------------------------
	// 1000을 FPS로 나누는 이유:						
	// Tick의 단위가 밀리 세턴드이므로 0.001초가 된다.	
	// 따라서 내가 원하는 FPS로 1000을 나눠야,			
	// 1 프레임 당 걸리는 Tick이 나온다.				
	// 결과 값으로 나온 tick * 0.001초를 하면			
	// 1 프레임당 걸리는 시간이 나온다.				
	//----------------------------------------------------
	m_iOneFrameTick = 1000 / iMaxFPS;
	m_iTick = 0;

	m_fCurFPS = 0.0f;
	m_fOldFPS = 0.0f;
	m_dwFrameCount = 0;		//프레임 카운트수
	m_fTimeElapsed = 0.0f;	//흐른 시간
	m_dwLastTime = 0;
	m_dwCurTime = 0;
}

CFrameSkip::~CFrameSkip()
{

}

BOOL CFrameSkip::FrameSkip()
{
	m_dwTick = timeGetTime();

	if (m_dwSystemTick != 0)
	{
		m_iTick += m_dwTick - m_dwSystemTick;

		//-----------------------------------------------
		// 누적 값이 1프레임 당 tick 값보다 작으면	
		// 모자란 1프레임에 걸리는 tick 값 만큼	
		// Sleep 해줌							
		//-----------------------------------------------
		if (m_iTick < m_iOneFrameTick)
		{
			Sleep(m_iOneFrameTick - m_iTick);
			m_iTick = m_iOneFrameTick;

			m_bSkipFalg = FALSE;
		}

		if (m_iTick - m_iOneFrameTick >= m_iOneFrameTick)
		{
			m_bSkipFalg = TRUE;
		}

		//----------------------------------------------------
		// 항상 누적 값에 1프레임 당 tick 값을 빼준다	
		// 그래야 초과되는 tick 값을 알 수 있다.		
		//----------------------------------------------------
		m_iTick -= m_iOneFrameTick;
	}
	else
	{
		m_bSkipFalg = FALSE;
	}

	m_dwSystemTick = timeGetTime();

	return m_bSkipFalg;
}

FLOAT CFrameSkip::CheckFPS(void)
{

	m_dwCurTime = timeGetTime();      //현재 시간

	//timeDelta(1번생성후 흐른 시간) 1초단위로 바꿔준다.
	if (m_dwLastTime != 0)
		m_fTimeDelta = (m_dwCurTime - m_dwLastTime) * 0.001f;


	m_fTimeElapsed += m_fTimeDelta;

	m_dwFrameCount++;

	if (m_fTimeElapsed >= 1.0f)         //흐른시간이 1초이상이면 내가 하고싶은것 처리
	{
		m_fCurFPS = (float)m_dwFrameCount;
		m_dwFrameCount = 0;
		m_fTimeElapsed = 0.0f;
		m_fOldFPS = m_fCurFPS;

		return m_fCurFPS;
	}

	m_dwLastTime = timeGetTime();   //마지막 시간(temp변수)

	return m_fOldFPS;
}