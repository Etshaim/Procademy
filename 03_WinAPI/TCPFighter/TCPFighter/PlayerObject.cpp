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
	// Sprite ����
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
	// ���� ���������� �ִϸ��̼�
	//---------------------------------
	NextFrame();
	ActionProc();
}

void	CPlayerObject::ActionProc(void)
{
	//----------------------------------------------------------------------
	// ��� ���۽� (����)�� ��� ���������� �ش� ���� ó���� �Ϸ��ؾ߸� �Ѵ�
	//----------------------------------------------------------------------
	switch (m_dwActionCur)
	{
		//--------------------------------------------------------------------------------------------
		// ���� ���� �Ǵ� ������ ������ �ִϸ��̼��� ���� ������ ���������� �ִϸ��̼��� �Ǿ�߸� �ϸ�,
		// �ִϸ��̼��� ���� �� �⺻ �������� �ڵ����� ���ư��� �Ѵ�
		//--------------------------------------------------------------------------------------------
	case dfACTION_ATTACK1:
	case dfACTION_ATTACK2:
	case dfACTION_ATTACK3:
		if (IsEndFrame())
		{
			SetActionStand();
			//-------------------------------------------------------------------------------
			// ������ ��������, �׼��� �ٲ��༭ �������� ������ �� �� �� ������ �����ϵ��� ��
			//-------------------------------------------------------------------------------
			m_dwActionCur = dfACTION_STAND;
			ActionInput(dfACTION_STAND);
		}
		break;
	default:
		//-----------------------------------------
		// �̿��� ��쿡�� ����� �Է� ó���� ����
		//-----------------------------------------
		InputActionProc();
		break;
	}

}

void	CPlayerObject::Draw(BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	// �׸��� ���
	g_cSpriteDib.DrawSprite50(eSHADOW, GetCurX(), GetCurY(), bypDest, iDestWidth, iDestHeight, iDestPitch);

	// �÷��̾� ��ü ���
	g_cSpriteDib.DrawSprite(GetSprite(), GetCurX(), GetCurY(), bypDest, iDestWidth, iDestHeight, iDestPitch);
	
	// HP ������ �� ���
	g_cSpriteDib.DrawSprite(eGUAGE_HP, GetCurX() - 35, GetCurY() + 9, bypDest, iDestWidth, iDestHeight, iDestPitch, GetHP());
}

int	CPlayerObject::GetDirection(void)
{
	// �¿츸 �ʿ����� üũ�� ��
	// �� �� �� �Ʒ� define ����
	return m_iDirCur;
}

char		CPlayerObject::GetHP(void)
{
	return m_chHP ;
}

void	CPlayerObject::InputActionProc(void)
{
	// ����� �Է� ó�� �κ�
	// dwActionInput�� ���� ó��

	// ���� �׼��� ���� �׼�����
	m_dwActionOld = m_dwActionCur;

	// ���ο� �׼��� �Է� ���� �׼�����
	m_dwActionCur = m_dwActionInput;


	// �Է� ���� �׼ǿ� ���� �б�
	switch (m_dwActionInput)
	{
		case dfACTION_MOVE_LL:
		{
			// SetDirection
			SetDirection(dfDIRECTION_LEFT);

			// SetActionMove
			SetActionMove();

			// ĳ���� ��ġ ����
			SetCurPosition(m_iCurX - dfOFFSET_PLAYER_MOVE, m_iCurY);
		}
		break;

		case dfACTION_MOVE_LU:
		{
			// SetDirection
			SetDirection(dfDIRECTION_LEFT);

			// SetActionMove
			SetActionMove();

			// ĳ���� ��ġ ����
			SetCurPosition(m_iCurX - dfOFFSET_PLAYER_MOVE, m_iCurY - dfOFFSET_PLAYER_MOVE);
		}
		break;

		case dfACTION_MOVE_UU:
		{
			// SetActionMove
			SetActionMove();

			// ĳ���� ��ġ ����
			SetCurPosition(m_iCurX, m_iCurY - dfOFFSET_PLAYER_MOVE);
		}
		break;

		case dfACTION_MOVE_RU:
		{
			// SetDirection
			SetDirection(dfDIRECTION_RIGHT);

			// SetActionMove
			SetActionMove();

			// ĳ���� ��ġ ����
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
			// ���� ��ȯ ���� ����
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

	// ������ ������ ����� ���ϰ� ��
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
	// TRUE�̸� �÷��̾�
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
	// ���� �ִϸ��̼��� ������ �ʾҴµ� ȣ��Ǹ� �Ѿ
	// �̻��ϸ� m_iDirCur�� m_iDirOld ���ؼ� �ٸ� ���� SetSprite ȣ���ϵ��� ����
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
	// ���� �ִϸ��̼��� ������ �ʾҴµ� ȣ��Ǹ� �Ѿ
	// �̻��ϸ� m_iDirCur�� m_iDirOld ���ؼ� �ٸ� ���� SetSprite ȣ���ϵ��� ����
	
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