#include "stdafx.h"
#include "PlayerObject.h"
#include "SpriteDIB.h"
#include "Common.h"
#include "LinkedList.h"
#include "EffectObject.h"
#include "MakePacket.h"
#include "Protocol.h"
#include "Map.h"

//CBaseObject(int objectType, int iCurX, int iCurY);
extern CSpriteDib						g_cSpriteDib;
extern CDoubleLinkedList<CBaseObject*>	objectList;
extern CMap								g_cMap;


CPlayerObject::CPlayerObject(
	int objectType, WORD wPosX, WORD wPosY,
	BYTE byHP, BYTE iDir, int iObjectID)
	: CBaseObject(objectType, wPosX, wPosY, iObjectID)
	, m_bPlayerCharacter(FALSE)
	, m_byHP(byHP)
	, m_byActionInput(dfACTION_STAND)
	, m_byActionCur(dfACTION_NONE)
	, m_byActionOld(dfACTION_NONE)
	, m_bySideDirCur(iDir)
	, m_bySideDirOld(iDir)
{
	// Sprite 세팅
	if (dfDIRECTION_LEFT == m_bySideDirCur)
	{
		SetSprite(ePLAYER_STAND_L01, ePLAYER_STAND_L01, dfDELAY_STAND);
	}
	else if (dfDIRECTION_RIGHT == m_bySideDirCur)
	{
		SetSprite(ePLAYER_STAND_R01, ePLAYER_STAND_R_MAX, dfDELAY_STAND);
	}
}

CPlayerObject::~CPlayerObject()
{
	
}

void	CPlayerObject::Action(void)
{	

	// 1. 애니메이션을 다음 프레임으로 
	NextFrame();

	// 패킷을 보내야하는지 체크하는 변수
	m_bSendPacket = FALSE;

	// 프레임 끝났는지 체크 
	switch ( m_byActionCur )
	{
	// 현재 action이 attack이라면 애니메이션 끝날 때까지 action 바꾸지 않는다
	case dfACTION_ATTACK1:
	case dfACTION_ATTACK2:
	case dfACTION_ATTACK3:
	{
		// 현재 ACTION이 ATTACK일 경우는 다른 액션을 입력받지 않는다
		// ATTACK이 끝나면 자동으로 STAND상태로 바꿔주기만 하고 ACTION 종료
		if (TRUE == IsEndFrame())
		{
			// attack이 끝나면 자동으로 stand 상태로 만들어줌
			m_byActionOld = m_byActionCur;
			m_byActionCur	= dfACTION_STAND;
			SetActionStand();
		}
		break;
	}

	default:
		// 입력 받은 액션으로 교체함
		InputActionProc();

		if (m_byActionCur != m_byActionOld)
		{
			m_bSendPacket = TRUE;

			// attack -> stop 은 false로 해줄 것임
		}

	
		// 액션 수행함
		ActionProc();

		//// 예외처리
		//// move중에 attack 누르면 중간에 stop 껴주기
		//switch (m_byActionOld)
		//{
		//case	dfMOVE_DIR_LL:
		//case	dfMOVE_DIR_LU:
		//case	dfMOVE_DIR_UU:
		//case	dfMOVE_DIR_RU:
		//case	dfMOVE_DIR_RR:
		//case	dfMOVE_DIR_RD:
		//case	dfMOVE_DIR_DD:
		//case	dfMOVE_DIR_LD:
		//{
		//	if (m_byActionCur == dfDELAY_ATTACK1 ||
		//		m_byActionCur == dfDELAY_ATTACK2 ||
		//		m_byActionCur == dfDELAY_ATTACK3)
		//	{
		//		MakePacket(dfPACKET_CS_MOVE_STOP, m_wCurX, m_wCurY, m_bySideDirCur);
		//	}
		//}
		//}

		break;

	}

	// 액션이 변경된 적이 있고 내 캐릭이면 패킷을 만들어 보낸다
	if ( TRUE == m_bPlayerCharacter && TRUE == m_bSendPacket)
	{
		// 영역 안에 들어옴
		if (dfRANGE_MOVE_TOP		>= m_wCurY ||
			dfRANGE_MOVE_LEFT		>= m_wCurX ||
			dfRANGE_MOVE_RIGHT		<= m_wCurX ||
			dfRANGE_MOVE_BOTTOM		<= m_wCurY)
		{
			return;
		}

		// 만약 이전 액션이 move이고 현재 액션이 attack이면 stand(stop) 패킷을 더 보낸다
		if (m_byActionOld == dfACTION_MOVE)
		{
			switch (m_byActionCur)
			{
			case dfACTION_ATTACK1:
			case dfACTION_ATTACK2:
			case dfACTION_ATTACK3:
			{
				MakePacket(dfPACKET_CS_MOVE_STOP, m_wCurX, m_wCurY, m_bySideDirCur);
				break;
			}
			default:
				break;
			}
		}

		MakePacket(m_byActionCur, m_wCurX, m_wCurY, m_bySideDirCur);
	}

	
}

void	CPlayerObject::InputActionProc(void)
{
	// 입력 받은 액션이 없다면 현재 액션을 유지함
	if(dfACTION_NONE != m_byActionInput)
	{
		// 액션을 입력 받은 액션으로 교체해준다
		m_byActionOld = m_byActionCur;
		m_byActionCur = m_byActionInput;

		// 입력 받은 액션을 초기화 해줌
		m_byActionInput = dfACTION_NONE;
	}

	else
	{
		m_byActionOld = m_byActionCur;
	}

}

// 애니메이션 action임
void	CPlayerObject::ActionProc(void)
{
	// 액션 종류에 따라 분기
	switch (m_byActionCur)
	{
	case	dfMOVE_DIR_LL:
	case	dfMOVE_DIR_LU:
	case	dfMOVE_DIR_UU:
	case	dfMOVE_DIR_RU:
	case	dfMOVE_DIR_RR:
	case	dfMOVE_DIR_RD:
	case	dfMOVE_DIR_DD:
	case	dfMOVE_DIR_LD:
	{
		// 캐릭터 방향 세팅
		//SetSideDirection(m_bySideDirCur);

		MovePositon();

		if (TRUE == IsEndFrame())
		{
			SetActionMove();
			break;
		}

		if (m_bySideDirCur != m_bySideDirOld)
		{
			SetActionMove();
			break;
		}

		if (m_byActionOld == dfACTION_STAND		||
			m_byActionOld == dfACTION_ATTACK1	||
			m_byActionOld == dfACTION_ATTACK2	||
			m_byActionOld == dfACTION_ATTACK3)
		{
			SetActionMove();
			break;
		}
		
	}
	break;

	case dfACTION_STAND:
	{
		// 이전 액션이 attack이었다면 packet보내지 않음
		if (dfACTION_ATTACK1 == m_byActionOld ||
			dfACTION_ATTACK2 == m_byActionOld ||
			dfACTION_ATTACK3 == m_byActionOld )
		{
			m_bSendPacket = FALSE;
		}

		// 이전 액션이 stand가 아니었다면 세팅해줌
		if (dfACTION_STAND != m_byActionOld)
		{
			SetActionStand();
			break;
		}

		if (TRUE == IsEndFrame())
		{
			SetActionStand();
			break;
		}
	}
	break;

	case dfACTION_ATTACK1:
	{
		SetActionAttack1();
	}
	break;

	case dfACTION_ATTACK2:
	{
		SetActionAttack2();
	}
	break;

	case dfACTION_ATTACK3:
	{
		SetActionAttack3();
	}
	break;

	default:
		break;
	}

	

}



//-----------------------------------------------------------------------
//외부에서의 액션 입력은 단순 멤버 변수에 세팅만 해준다
// 직접적인 스프라이트 변경, 애니메이션 변경, 좌표 이동 등을 해선 안 되며
// 액션 변경에 대한 실질적인 처리는 Action함수에서만 하도록 한다.
//-----------------------------------------------------------------------
void CPlayerObject::InputAction(BYTE byAction)
{
	m_byActionInput = byAction;
}

void	CPlayerObject::Draw(BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	// 그림자 출력
	//g_cSpriteDib.DrawSprite50(eSHADOW, GetCurX(), GetCurY(), bypDest, iDestWidth, iDestHeight, iDestPitch);
	g_cSpriteDib.DrawSprite50(eSHADOW, GetCurX(), GetCurY(), bypDest, iDestWidth, iDestHeight, iDestPitch);

	// 플레이어 객체 출력
	if(TRUE == m_bPlayerCharacter )
	{
		g_cSpriteDib.DrawSpriteRed(GetSprite(), GetCurX(), GetCurY(), bypDest, iDestWidth, iDestHeight, iDestPitch);
	}
	else
	{
		g_cSpriteDib.DrawSprite(GetSprite(), GetCurX(), GetCurY(), bypDest, iDestWidth, iDestHeight, iDestPitch);
	}
	
	// HP 게이지 바 출력
	g_cSpriteDib.DrawSprite(eGUAGE_HP, GetCurX() - 35, GetCurY() + 9, bypDest, iDestWidth, iDestHeight, iDestPitch, GetHP());

}

void	CPlayerObject::SetSideDirection(BYTE bySideDir)
{
	switch (bySideDir)
	{
	case dfMOVE_DIR_LL:
	case dfMOVE_DIR_LD:
	case dfMOVE_DIR_LU:
	{
		m_bySideDirOld = m_bySideDirCur;
		m_bySideDirCur = dfDIRECTION_LEFT;
	}
		break;

	case dfMOVE_DIR_RU:
	case dfMOVE_DIR_RR:
	case dfMOVE_DIR_RD:
	{
		m_bySideDirOld = m_bySideDirCur;
		m_bySideDirCur = dfDIRECTION_RIGHT;
	}
		break;

	default:
		break;
	}
}

BYTE	CPlayerObject::GetSideDirection(void)
{
	// 좌우만 필요한지 체크할 것
	// 좌 우 위 아래 define 했음
	return m_bySideDirCur;
}

//void CPlayerObject::SetMoveDirection(BYTE byMoveDir)
//{
//	m_byMoveDirOld = m_byMoveDirOld;
//	m_byMoveDirCur = byMoveDir;
//}
//
//BYTE CPlayerObject::GetMoveDirection(void)
//{
//	return m_byMoveDirCur;
//}


//
//void CPlayerObject::SetDirection(BYTE byDirection)
//{
//	switch (byDirection)
//	{
//	case dfACTION_MOVE_LL: 
//	case dfACTION_MOVE_LD: 
//	case dfACTION_MOVE_LU: 
//		m_byDirCur = dfDIRECTION_LEFT;
//		break;
//
//	case dfACTION_MOVE_RU: 
//	case dfACTION_MOVE_RR: 
//	case dfACTION_MOVE_RD:
//		m_byDirCur = dfDIRECTION_RIGHT;
//		break;
//
//	default:
//			break;
//	}
//}



BYTE		CPlayerObject::GetHP(void)
{
	return m_byHP ;
}




BOOL	CPlayerObject::IsPlayer(void)
{
	// TRUE이면 플레이어
	return m_bPlayerCharacter;
}

void CPlayerObject::MovePositon(void)
{

	// 현재 좌표 임시 변수에 저장
	// 임시 변수로 이동해보고 갈 수 없는 위치면 이동하지 않을 것임

	WORD m_wTempX = m_wCurX;
	WORD m_wTempY = m_wCurY;
	
	// 액션 종류에 따라 분기
	switch (m_byActionCur)
	{
	case dfMOVE_DIR_LL:
	{
		// 캐릭터 위치 변경
		//SetCurPosition(m_wTempX - dfSPEED_PLAYER_X, m_wTempY);
		m_wTempX -= dfSPEED_PLAYER_X;

		// 캐릭터 방향 세팅
		SetSideDirection(dfDIRECTION_LEFT);
	}
	break;

	case dfMOVE_DIR_LU:
	{
		// 캐릭터 위치 변경
		//SetCurPosition(m_wTempX - dfSPEED_PLAYER_X, m_wTempY - dfSPEED_PLAYER_Y);
		m_wTempX -= dfSPEED_PLAYER_X;
		m_wTempY -= dfSPEED_PLAYER_Y;
		// 캐릭터 방향 세팅
		SetSideDirection(dfDIRECTION_LEFT);
	}
	break;

	case dfMOVE_DIR_UU:
	{
		// 캐릭터 위치 변경
		//SetCurPosition(m_wTempX, m_wTempY - dfSPEED_PLAYER_Y);
		m_wTempY -= dfSPEED_PLAYER_Y;
	}
	break;

	case dfMOVE_DIR_RU:
	{
	
		// 캐릭터 위치 변경
		//SetCurPosition(m_wTempX + dfSPEED_PLAYER_X, m_wTempY - dfSPEED_PLAYER_Y);

		m_wTempX += dfSPEED_PLAYER_X;
		m_wTempY -= dfSPEED_PLAYER_Y;

		// 캐릭터 방향 세팅
		SetSideDirection(dfDIRECTION_RIGHT);
	}
	break;

	case dfMOVE_DIR_RR:
	{

		//SetCurPosition(m_wTempX + dfSPEED_PLAYER_X, m_wTempY);

		m_wTempX += dfSPEED_PLAYER_X;

		// 캐릭터 방향 세팅
		SetSideDirection(dfDIRECTION_RIGHT);
	}
	break;

	case dfMOVE_DIR_RD:
	{
		//SetCurPosition(m_wTempX + dfSPEED_PLAYER_X, m_wTempY + dfSPEED_PLAYER_Y);

		m_wTempX = m_wTempX + dfSPEED_PLAYER_X;
		m_wTempY = m_wTempY + dfSPEED_PLAYER_Y;

		// 캐릭터 방향 세팅
		SetSideDirection(dfDIRECTION_RIGHT);
	}
	break;

	case dfMOVE_DIR_DD:
	{
		//SetCurPosition(m_wTempX, m_wTempY + dfSPEED_PLAYER_Y);
		m_wTempY = m_wTempY + dfSPEED_PLAYER_Y;
	}
	break;

	case dfMOVE_DIR_LD:
	{
		//SetCurPosition(m_wTempX - dfSPEED_PLAYER_X, m_wTempY + dfSPEED_PLAYER_Y);

		m_wTempX -= dfSPEED_PLAYER_X;
		m_wTempY += dfSPEED_PLAYER_Y;

		// 캐릭터 방향 세팅
		SetSideDirection(dfDIRECTION_LEFT);
	}
	break;

	default:
		return;
	}


	// 영역을 벗어났는지 확인
	//#define dfRANGE_MOVE_TOP		50		51
	//#define dfRANGE_MOVE_LEFT		10		12
	//#define dfRANGE_MOVE_RIGHT	630		627
	//#define dfRANGE_MOVE_BOTTOM	470		469
	/*m_wCurX = max(12, m_wCurX);
	m_wCurY = max(51, m_wCurY);
	m_wCurX = min(627, m_wCurX);
	m_wCurY = min(469, m_wCurY);*/


	// 영역 안에 들어옴
	if (dfRANGE_MOVE_TOP		< m_wTempY &&
		dfRANGE_MOVE_LEFT		< m_wTempX &&
		dfRANGE_MOVE_RIGHT		> m_wTempX &&
		dfRANGE_MOVE_BOTTOM		> m_wTempY)
	{
		SetPosition(m_wTempX, m_wTempY);
	}
	/*else
	{
		m_bSendPacket = FALSE;
	}*/

	
}

void	CPlayerObject::SetActionAttack1(void)
{
	if (dfDIRECTION_LEFT == m_bySideDirCur)
	{
		SetSprite(ePLAYER_ATTACK1_L01, ePLAYER_ATTACK1_L_MAX, dfDELAY_ATTACK1);
	}

	if (dfDIRECTION_RIGHT == m_bySideDirCur)
	{
		SetSprite(ePLAYER_ATTACK1_R01, ePLAYER_ATTACK1_R_MAX, dfDELAY_ATTACK1);
	}
}

void	CPlayerObject::SetActionAttack2(void)
{
	if (dfDIRECTION_LEFT == m_bySideDirCur)
	{
		SetSprite(ePLAYER_ATTACK2_L01, ePLAYER_ATTACK2_L_MAX, dfDELAY_ATTACK2);
	}

	if (dfDIRECTION_RIGHT == m_bySideDirCur)
	{
		SetSprite(ePLAYER_ATTACK2_R01, ePLAYER_ATTACK2_R_MAX, dfDELAY_ATTACK2);
	}
}

void	CPlayerObject::SetActionAttack3(void)
{
	if (dfDIRECTION_LEFT == m_bySideDirCur)
	{
		SetSprite(ePLAYER_ATTACK3_L01, ePLAYER_ATTACK3_L_MAX, dfDELAY_ATTACK3);
	}

	if (dfDIRECTION_RIGHT == m_bySideDirCur)
	{
		SetSprite(ePLAYER_ATTACK3_R01, ePLAYER_ATTACK3_R_MAX, dfDELAY_ATTACK3);
	}
}

void	CPlayerObject::SetActionMove(void)
{
	if (dfDIRECTION_LEFT == m_bySideDirCur)
	{
		SetSprite(ePLAYER_MOVE_L01, ePLAYER_MOVE_L_MAX, dfDELAY_MOVE);
	}

	if (dfDIRECTION_RIGHT == m_bySideDirCur)
	{
		SetSprite(ePLAYER_MOVE_R01, ePLAYER_MOVE_R_MAX, dfDELAY_MOVE);
	}
}

void	CPlayerObject::SetActionStand(void)
{
	// 만약 애니메이션이 끝나지 않았는데 호출되면 넘어감
	// 이상하면 m_iDirCur과 m_iDirOld 비교해서 다를 때는 SetSprite 호출하도록 변경
	
	//if (	FALSE == m_bEndFrame && m_byDirCur == m_byDirOld /*&& */ )
	//{
	//	if ( m_byActionCur == m_byActionOld )
	//	{
	//		return;
	//	}
	//}

	if (dfDIRECTION_LEFT == m_bySideDirCur)
	{
		SetSprite(ePLAYER_STAND_L01, ePLAYER_STAND_L_MAX, dfDELAY_STAND);
	}

	if (dfDIRECTION_RIGHT == m_bySideDirCur)
	{
		SetSprite(ePLAYER_STAND_R01, ePLAYER_STAND_R_MAX, dfDELAY_STAND);
	}
}



void CPlayerObject::SetMyCharacter(void)
{
	m_bPlayerCharacter = TRUE;
}

BOOL CPlayerObject::CheckMyCharacter(void)
{
	return m_bPlayerCharacter;
}

void	CPlayerObject::SetHP(BYTE chHP)
{
	m_byHP = chHP;
}
