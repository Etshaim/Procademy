#include "stdafx.h"
#include "PlayerObject.h"
#include "SpriteDIB.h"
#include "Common.h"
#include "LinkedList.h"
#include "EffectObject.h"

//CBaseObject(int objectType, int iCurX, int iCurY);
extern CSpriteDib		g_cSpriteDib;
extern CDoubleLinkedList<CBaseObject*> objectList;


CPlayerObject::CPlayerObject(int objectType, int iPosX, int iPosY, int iDir, int iObjectID)
	: CBaseObject(objectType, iPosX, iPosY, iObjectID)
	, m_bPlayerCharacter(TRUE)
	, m_chHP(100)
	, m_dwActionCur(dfACTION_STAND)
	, m_dwActionOld(dfACTION_STOP)
	, m_iDirCur(iDir)
	, m_iDirOld(iDir)
{
	OutputDebugString(L"CPlayerObject()\n");
	// Sprite 세팅
	if (dfDIRECTION_LEFT == m_iDirCur)
	{
		SetSprite(ePLAYER_STAND_L01, ePLAYER_STAND_L01, dfDELAY_STAND);
	}
	else if (dfDIRECTION_RIGHT == m_iDirCur)
	{
		SetSprite(ePLAYER_STAND_R01, ePLAYER_STAND_R_MAX, dfDELAY_STAND);
	}
}

CPlayerObject::~CPlayerObject()
{
	OutputDebugString(L"~CPlayerObject()\n");
}

void	CPlayerObject::Action(void)
{
	//---------------------------------
	// 다음 프레임으로 애니메이션
	//---------------------------------
	NextFrame();
	ActionProc();
}

void	CPlayerObject::ActionProc(void)
{
	//----------------------------------------------------------------------
	// 몇몇 동작시 (공격)의 경우 강제적으로 해당 동작 처리를 완료해야만 한다
	//----------------------------------------------------------------------
	switch (m_dwActionCur)
	{
		//--------------------------------------------------------------------------------------------
		// 공격 동작 또는 데미지 동작은 애니메이션이 끝날 때까지 강제적으로 애니메이션이 되어야만 하며,
		// 애니메이션이 끝난 후 기본 동작으로 자동으로 돌아가야 한다
		//--------------------------------------------------------------------------------------------
	case dfACTION_ATTACK1:
	case dfACTION_ATTACK2:
	case dfACTION_ATTACK3:
		if (IsEndFrame())
		{
			SetActionStand();
			//-------------------------------------------------------------------------------
			// 공격이 끝났으면, 액션을 바꿔줘서 연속으로 공격을 할 때 재 전송이 가능하도록 함
			//-------------------------------------------------------------------------------
			m_dwActionCur = dfACTION_STAND;
			ActionInput(dfACTION_STAND);
		}
		break;
	default:
		//-----------------------------------------
		// 이외의 경우에는 사용자 입력 처리를 해줌
		//-----------------------------------------
		InputActionProc();
		break;
	}

}

void	CPlayerObject::Draw(BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	// 그림자 출력
	g_cSpriteDib.DrawSprite50(eSHADOW, GetCurX(), GetCurY(), bypDest, iDestWidth, iDestHeight, iDestPitch);

	// 플레이어 객체 출력
	g_cSpriteDib.DrawSprite(GetSprite(), GetCurX(), GetCurY(), bypDest, iDestWidth, iDestHeight, iDestPitch);
	
	// HP 게이지 바 출력
	g_cSpriteDib.DrawSprite(eGUAGE_HP, GetCurX() - 35, GetCurY() + 9, bypDest, iDestWidth, iDestHeight, iDestPitch, GetHP());
}

int	CPlayerObject::GetDirection(void)
{
	// 좌우만 필요한지 체크할 것
	// 좌 우 위 아래 define 했음
	return m_iDirCur;
}

char		CPlayerObject::GetHP(void)
{
	return m_chHP ;
}

void	CPlayerObject::InputActionProc(void)
{
	// 사용자 입력 처리 부분
	// dwActionInput에 따라 처리

	// 현재 액션을 이전 액션으로
	m_dwActionOld = m_dwActionCur;

	// 새로운 액션을 입력 받은 액션으로
	m_dwActionCur = m_dwActionInput;


	// 입력 받은 액션에 따라 분기
	switch (m_dwActionInput)
	{
		case dfACTION_MOVE_LL:
		{
			// SetDirection
			SetDirection(dfDIRECTION_LEFT);

			// SetActionMove
			SetActionMove();

			// 캐릭터 위치 변경
			SetCurPosition(m_iCurX - dfOFFSET_PLAYER_MOVE, m_iCurY);
		}
		break;

		case dfACTION_MOVE_LU:
		{
			// SetDirection
			SetDirection(dfDIRECTION_LEFT);

			// SetActionMove
			SetActionMove();

			// 캐릭터 위치 변경
			SetCurPosition(m_iCurX - dfOFFSET_PLAYER_MOVE, m_iCurY - dfOFFSET_PLAYER_MOVE);
		}
		break;

		case dfACTION_MOVE_UU:
		{
			// SetActionMove
			SetActionMove();

			// 캐릭터 위치 변경
			SetCurPosition(m_iCurX, m_iCurY - dfOFFSET_PLAYER_MOVE);
		}
		break;

		case dfACTION_MOVE_RU:
		{
			// SetDirection
			SetDirection(dfDIRECTION_RIGHT);

			// SetActionMove
			SetActionMove();

			// 캐릭터 위치 변경
			SetCurPosition(m_iCurX + dfOFFSET_PLAYER_MOVE, m_iCurY - dfOFFSET_PLAYER_MOVE);
		}
		break;

		case dfACTION_MOVE_RR:
		{
			SetDirection(dfDIRECTION_RIGHT);

			// SetActionMove
			SetActionMove();
			SetCurPosition(m_iCurX + dfOFFSET_PLAYER_MOVE, m_iCurY);
			
		}
		break;

		case dfACTION_MOVE_RD:
		{
			SetDirection(dfDIRECTION_RIGHT);

			// SetActionMove
			SetActionMove();

			SetCurPosition(m_iCurX + dfOFFSET_PLAYER_MOVE, m_iCurY + dfOFFSET_PLAYER_MOVE);

		}
		break;

		case dfACTION_MOVE_DD:
		{
			// 방향 전환 하지 않음
			SetActionMove();
			
			SetCurPosition(m_iCurX, m_iCurY + dfOFFSET_PLAYER_MOVE);
		}
		break;

		case dfACTION_MOVE_LD:
		{
			SetDirection(dfDIRECTION_LEFT);

			// SetActionMove
			SetActionMove();

			SetCurPosition(m_iCurX - dfOFFSET_PLAYER_MOVE, m_iCurY + dfOFFSET_PLAYER_MOVE);
		}
		break;

		case dfACTION_STAND:
		{
			SetActionStand();
		}
		break;

		case dfACTION_ATTACK1:
		{
			SetActionAttack1();
			CreateEffect(m_iCurX, m_iCurY, m_iObjectID);
		}
		break;

		case dfACTION_ATTACK2:
		{
			SetActionAttack2();
			CreateEffect(m_iCurX, m_iCurY, m_iObjectID);
		}
		break;

		case dfACTION_ATTACK3:
		{
			SetActionAttack3();
			CreateEffect(m_iCurX, m_iCurY, m_iObjectID);
		}
		break;

		case dfACTION_STOP:
		{
			system("puase");
		}
		break;
	}

	// 정해진 영역을 벗어나지 못하게 함
	//#define dfRANGE_MOVE_TOP		50
	//#define dfRANGE_MOVE_LEFT		10
	//#define dfRANGE_MOVE_RIGHT	630
	//#define dfRANGE_MOVE_BOTTOM	470

	m_iCurX = max(10, m_iCurX);
	m_iCurY = max(50, m_iCurY);
	m_iCurX = min(630, m_iCurX);
	m_iCurY = min(470, m_iCurY);
}

BOOL	CPlayerObject::IsPlayer(void)
{
	// TRUE이면 플레이어
	return m_bPlayerCharacter;
}

void	CPlayerObject::SetActionAttack1(void)
{
	if (dfDIRECTION_LEFT == m_iDirCur)
	{
		SetSprite(ePLAYER_ATTACK1_L01, ePLAYER_ATTACK1_L_MAX, dfDELAY_ATTACK1);
	}

	if (dfDIRECTION_RIGHT == m_iDirCur)
	{
		SetSprite(ePLAYER_ATTACK1_R01, ePLAYER_ATTACK1_R_MAX, dfDELAY_ATTACK1);
	}
}

void	CPlayerObject::SetActionAttack2(void)
{
	if (dfDIRECTION_LEFT == m_iDirCur)
	{
		SetSprite(ePLAYER_ATTACK2_L01, ePLAYER_ATTACK2_L_MAX, dfDELAY_ATTACK2);
	}

	if (dfDIRECTION_RIGHT == m_iDirCur)
	{
		SetSprite(ePLAYER_ATTACK2_R01, ePLAYER_ATTACK2_R_MAX, dfDELAY_ATTACK2);
	}
}

void	CPlayerObject::SetActionAttack3(void)
{
	if (dfDIRECTION_LEFT == m_iDirCur)
	{
		SetSprite(ePLAYER_ATTACK3_L01, ePLAYER_ATTACK3_L_MAX, dfDELAY_ATTACK3);
	}

	if (dfDIRECTION_RIGHT == m_iDirCur)
	{
		SetSprite(ePLAYER_ATTACK3_R01, ePLAYER_ATTACK3_R_MAX, dfDELAY_ATTACK3);
	}
}

void	CPlayerObject::SetActionMove(void)
{
	// 만약 애니메이션이 끝나지 않았는데 호출되면 넘어감
	// 이상하면 m_iDirCur과 m_iDirOld 비교해서 다를 때는 SetSprite 호출하도록 변경
	//if (FALSE == m_bEndFrame && m_dwActionCur == m_dwActionOld)
	if ( (FALSE == m_bEndFrame) && (m_iDirCur == m_iDirOld)	)
	{
		if (dfACTION_STAND == m_dwActionCur)
		{

		}
		return;
	}

	if (dfDIRECTION_LEFT == m_iDirCur)
	{
		SetSprite(ePLAYER_MOVE_L01, ePLAYER_MOVE_L_MAX, dfDELAY_MOVE);
	}

	if (dfDIRECTION_RIGHT == m_iDirCur)
	{
		SetSprite(ePLAYER_MOVE_R01, ePLAYER_MOVE_R_MAX, dfDELAY_MOVE);
	}
}

void	CPlayerObject::SetActionStand(void)
{
	// 만약 애니메이션이 끝나지 않았는데 호출되면 넘어감
	// 이상하면 m_iDirCur과 m_iDirOld 비교해서 다를 때는 SetSprite 호출하도록 변경
	
	if (	FALSE == m_bEndFrame && m_iDirCur == m_iDirOld /*&& */ )
	{
		if ( m_dwActionCur == m_dwActionOld )
		{
			return;
		}
	}

	if (dfDIRECTION_LEFT == m_iDirCur)
	{
		SetSprite(ePLAYER_STAND_L01, ePLAYER_STAND_L_MAX, dfDELAY_STAND);
	}

	if (dfDIRECTION_RIGHT == m_iDirCur)
	{
		SetSprite(ePLAYER_STAND_R01, ePLAYER_STAND_R_MAX, dfDELAY_STAND);
	}
}

void	CPlayerObject::SetDirection(int iDirection)
{
	m_iDirOld = m_iDirCur;
	m_iDirCur = iDirection;
}

void	CPlayerObject::SetHP(char chHP)
{
	m_chHP = chHP;
}

void CPlayerObject::CreateEffect(int iPosX, int iPosY, int iObjectID)
{
	objectList.PushBack( new CEffectObject(0, m_iCurX, m_iCurY, m_iObjectID, m_iDirCur) );
}