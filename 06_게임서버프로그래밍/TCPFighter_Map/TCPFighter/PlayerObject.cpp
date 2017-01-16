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
	// Sprite ����
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

	// 1. �ִϸ��̼��� ���� ���������� 
	NextFrame();

	// ��Ŷ�� �������ϴ��� üũ�ϴ� ����
	m_bSendPacket = FALSE;

	// ������ �������� üũ 
	switch ( m_byActionCur )
	{
	// ���� action�� attack�̶�� �ִϸ��̼� ���� ������ action �ٲ��� �ʴ´�
	case dfACTION_ATTACK1:
	case dfACTION_ATTACK2:
	case dfACTION_ATTACK3:
	{
		// ���� ACTION�� ATTACK�� ���� �ٸ� �׼��� �Է¹��� �ʴ´�
		// ATTACK�� ������ �ڵ����� STAND���·� �ٲ��ֱ⸸ �ϰ� ACTION ����
		if (TRUE == IsEndFrame())
		{
			// attack�� ������ �ڵ����� stand ���·� �������
			m_byActionOld = m_byActionCur;
			m_byActionCur	= dfACTION_STAND;
			SetActionStand();
		}
		break;
	}

	default:
		// �Է� ���� �׼����� ��ü��
		InputActionProc();

		if (m_byActionCur != m_byActionOld)
		{
			m_bSendPacket = TRUE;

			// attack -> stop �� false�� ���� ����
		}

	
		// �׼� ������
		ActionProc();

		//// ����ó��
		//// move�߿� attack ������ �߰��� stop ���ֱ�
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

	// �׼��� ����� ���� �ְ� �� ĳ���̸� ��Ŷ�� ����� ������
	if ( TRUE == m_bPlayerCharacter && TRUE == m_bSendPacket)
	{
		// ���� �ȿ� ����
		if (dfRANGE_MOVE_TOP		>= m_wCurY ||
			dfRANGE_MOVE_LEFT		>= m_wCurX ||
			dfRANGE_MOVE_RIGHT		<= m_wCurX ||
			dfRANGE_MOVE_BOTTOM		<= m_wCurY)
		{
			return;
		}

		// ���� ���� �׼��� move�̰� ���� �׼��� attack�̸� stand(stop) ��Ŷ�� �� ������
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
	// �Է� ���� �׼��� ���ٸ� ���� �׼��� ������
	if(dfACTION_NONE != m_byActionInput)
	{
		// �׼��� �Է� ���� �׼����� ��ü���ش�
		m_byActionOld = m_byActionCur;
		m_byActionCur = m_byActionInput;

		// �Է� ���� �׼��� �ʱ�ȭ ����
		m_byActionInput = dfACTION_NONE;
	}

	else
	{
		m_byActionOld = m_byActionCur;
	}

}

// �ִϸ��̼� action��
void	CPlayerObject::ActionProc(void)
{
	// �׼� ������ ���� �б�
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
		// ĳ���� ���� ����
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
		// ���� �׼��� attack�̾��ٸ� packet������ ����
		if (dfACTION_ATTACK1 == m_byActionOld ||
			dfACTION_ATTACK2 == m_byActionOld ||
			dfACTION_ATTACK3 == m_byActionOld )
		{
			m_bSendPacket = FALSE;
		}

		// ���� �׼��� stand�� �ƴϾ��ٸ� ��������
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
//�ܺο����� �׼� �Է��� �ܼ� ��� ������ ���ø� ���ش�
// �������� ��������Ʈ ����, �ִϸ��̼� ����, ��ǥ �̵� ���� �ؼ� �� �Ǹ�
// �׼� ���濡 ���� �������� ó���� Action�Լ������� �ϵ��� �Ѵ�.
//-----------------------------------------------------------------------
void CPlayerObject::InputAction(BYTE byAction)
{
	m_byActionInput = byAction;
}

void	CPlayerObject::Draw(BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	// �׸��� ���
	//g_cSpriteDib.DrawSprite50(eSHADOW, GetCurX(), GetCurY(), bypDest, iDestWidth, iDestHeight, iDestPitch);
	g_cSpriteDib.DrawSprite50(eSHADOW, GetCurX(), GetCurY(), bypDest, iDestWidth, iDestHeight, iDestPitch);

	// �÷��̾� ��ü ���
	if(TRUE == m_bPlayerCharacter )
	{
		g_cSpriteDib.DrawSpriteRed(GetSprite(), GetCurX(), GetCurY(), bypDest, iDestWidth, iDestHeight, iDestPitch);
	}
	else
	{
		g_cSpriteDib.DrawSprite(GetSprite(), GetCurX(), GetCurY(), bypDest, iDestWidth, iDestHeight, iDestPitch);
	}
	
	// HP ������ �� ���
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
	// �¿츸 �ʿ����� üũ�� ��
	// �� �� �� �Ʒ� define ����
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
	// TRUE�̸� �÷��̾�
	return m_bPlayerCharacter;
}

void CPlayerObject::MovePositon(void)
{

	// ���� ��ǥ �ӽ� ������ ����
	// �ӽ� ������ �̵��غ��� �� �� ���� ��ġ�� �̵����� ���� ����

	WORD m_wTempX = m_wCurX;
	WORD m_wTempY = m_wCurY;
	
	// �׼� ������ ���� �б�
	switch (m_byActionCur)
	{
	case dfMOVE_DIR_LL:
	{
		// ĳ���� ��ġ ����
		//SetCurPosition(m_wTempX - dfSPEED_PLAYER_X, m_wTempY);
		m_wTempX -= dfSPEED_PLAYER_X;

		// ĳ���� ���� ����
		SetSideDirection(dfDIRECTION_LEFT);
	}
	break;

	case dfMOVE_DIR_LU:
	{
		// ĳ���� ��ġ ����
		//SetCurPosition(m_wTempX - dfSPEED_PLAYER_X, m_wTempY - dfSPEED_PLAYER_Y);
		m_wTempX -= dfSPEED_PLAYER_X;
		m_wTempY -= dfSPEED_PLAYER_Y;
		// ĳ���� ���� ����
		SetSideDirection(dfDIRECTION_LEFT);
	}
	break;

	case dfMOVE_DIR_UU:
	{
		// ĳ���� ��ġ ����
		//SetCurPosition(m_wTempX, m_wTempY - dfSPEED_PLAYER_Y);
		m_wTempY -= dfSPEED_PLAYER_Y;
	}
	break;

	case dfMOVE_DIR_RU:
	{
	
		// ĳ���� ��ġ ����
		//SetCurPosition(m_wTempX + dfSPEED_PLAYER_X, m_wTempY - dfSPEED_PLAYER_Y);

		m_wTempX += dfSPEED_PLAYER_X;
		m_wTempY -= dfSPEED_PLAYER_Y;

		// ĳ���� ���� ����
		SetSideDirection(dfDIRECTION_RIGHT);
	}
	break;

	case dfMOVE_DIR_RR:
	{

		//SetCurPosition(m_wTempX + dfSPEED_PLAYER_X, m_wTempY);

		m_wTempX += dfSPEED_PLAYER_X;

		// ĳ���� ���� ����
		SetSideDirection(dfDIRECTION_RIGHT);
	}
	break;

	case dfMOVE_DIR_RD:
	{
		//SetCurPosition(m_wTempX + dfSPEED_PLAYER_X, m_wTempY + dfSPEED_PLAYER_Y);

		m_wTempX = m_wTempX + dfSPEED_PLAYER_X;
		m_wTempY = m_wTempY + dfSPEED_PLAYER_Y;

		// ĳ���� ���� ����
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

		// ĳ���� ���� ����
		SetSideDirection(dfDIRECTION_LEFT);
	}
	break;

	default:
		return;
	}


	// ������ ������� Ȯ��
	//#define dfRANGE_MOVE_TOP		50		51
	//#define dfRANGE_MOVE_LEFT		10		12
	//#define dfRANGE_MOVE_RIGHT	630		627
	//#define dfRANGE_MOVE_BOTTOM	470		469
	/*m_wCurX = max(12, m_wCurX);
	m_wCurY = max(51, m_wCurY);
	m_wCurX = min(627, m_wCurX);
	m_wCurY = min(469, m_wCurY);*/


	// ���� �ȿ� ����
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
	// ���� �ִϸ��̼��� ������ �ʾҴµ� ȣ��Ǹ� �Ѿ
	// �̻��ϸ� m_iDirCur�� m_iDirOld ���ؼ� �ٸ� ���� SetSprite ȣ���ϵ��� ����
	
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
