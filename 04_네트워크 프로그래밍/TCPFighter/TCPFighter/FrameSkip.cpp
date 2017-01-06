#include "stdafx.h"
#include "FrameSkip.h"

CFrameSkip::CFrameSkip(int iMaxFPS)
{
	m_dwTick = 0;
	m_dwSystemTick = 0;
	m_iMaxFPS = iMaxFPS;

	//----------------------------------------------------
	// 1000�� FPS�� ������ ����:						
	// Tick�� ������ �и� ���ϵ��̹Ƿ� 0.001�ʰ� �ȴ�.	
	// ���� ���� ���ϴ� FPS�� 1000�� ������,			
	// 1 ������ �� �ɸ��� Tick�� ���´�.				
	// ��� ������ ���� tick * 0.001�ʸ� �ϸ�			
	// 1 �����Ӵ� �ɸ��� �ð��� ���´�.				
	//----------------------------------------------------
	m_iOneFrameTick = 1000 / iMaxFPS;
	m_iTick = 0;

	m_fCurFPS = 0.0f;
	m_fOldFPS = 0.0f;
	m_dwFrameCount = 0;		//������ ī��Ʈ��
	m_fTimeElapsed = 0.0f;	//�帥 �ð�
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
		// ���� ���� 1������ �� tick ������ ������	
		// ���ڶ� 1�����ӿ� �ɸ��� tick �� ��ŭ	
		// Sleep ����							
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
		// �׻� ���� ���� 1������ �� tick ���� ���ش�	
		// �׷��� �ʰ��Ǵ� tick ���� �� �� �ִ�.		
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

	m_dwCurTime = timeGetTime();      //���� �ð�

	//timeDelta(1�������� �帥 �ð�) 1�ʴ����� �ٲ��ش�.
	if (m_dwLastTime != 0)
		m_fTimeDelta = (m_dwCurTime - m_dwLastTime) * 0.001f;


	m_fTimeElapsed += m_fTimeDelta;

	m_dwFrameCount++;

	if (m_fTimeElapsed >= 1.0f)         //�帥�ð��� 1���̻��̸� ���� �ϰ������ ó��
	{
		m_fCurFPS = (float)m_dwFrameCount;
		m_dwFrameCount = 0;
		m_fTimeElapsed = 0.0f;
		m_fOldFPS = m_fCurFPS;

		return m_fCurFPS;
	}

	m_dwLastTime = timeGetTime();   //������ �ð�(temp����)

	return m_fOldFPS;
}