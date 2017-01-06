#ifndef		__COMMON_H__
#define		__COMMON_H__

// Direction
// 이동 방향이랑 맞추기 위해 0, 4로
#define		dfDIRECTION_LEFT	0
#define		dfDIRECTION_RIGHT	4

#define		dfMOVE_DIR_LL		0
#define		dfMOVE_DIR_LU		1
#define		dfMOVE_DIR_UU		2
#define		dfMOVE_DIR_RU		3
#define		dfMOVE_DIR_RR		4
#define		dfMOVE_DIR_RD		5
#define		dfMOVE_DIR_DD		6
#define		dfMOVE_DIR_LD		7

#define		dfACTION_NONE			8
#define		dfACTION_MOVE			9
#define		dfACTION_STAND			10
#define		dfACTION_ATTACK1		11
#define		dfACTION_ATTACK2		12
#define		dfACTION_ATTACK3		13


// 화면 이동영역
#define		dfRANGE_MOVE_TOP		50
#define		dfRANGE_MOVE_LEFT		10
#define		dfRANGE_MOVE_RIGHT		630
#define		dfRANGE_MOVE_BOTTOM		470



//#define		dfDIRECTION_UP		2
//#define		dfDIRECTION_DOWN	3
//#define		dfDIRECTION_NONE	4

// 애니메이션 프레임 딜레이
#define		dfDELAY_STAND			5
#define		dfDELAY_MOVE			4
#define		dfDELAY_ATTACK1			3
#define		dfDELAY_ATTACK2			4
#define		dfDELAY_ATTACK3			4
#define		dfDELAY_EFFECT			3

// 플레이어 이동 오프셋
//#define		dfOFFSET_PLAYER_MOVE	3
//-----------------------------------------------------------------
// 캐릭터 이동 속도
//-----------------------------------------------------------------
#define dfSPEED_PLAYER_X	3
#define dfSPEED_PLAYER_Y	2

enum e_OBJECT_TYPE
{
	ePLAYER,
	eOTHER_PLAYER,
	eNCP,
	eEFFECT,
	eEND_EFFECT
};

enum e_SPRITE
{
	eMAP,

	ePLAYER_STAND_L01,
	ePLAYER_STAND_L02,
	ePLAYER_STAND_L03,
	//ePLAYER_STAND_L04,
	//ePLAYER_STAND_L05,
	ePLAYER_STAND_L_MAX,

	ePLAYER_STAND_R01,
	ePLAYER_STAND_R02,
	ePLAYER_STAND_R03,
	//ePLAYER_STAND_R04,
	//ePLAYER_STAND_R05,
	ePLAYER_STAND_R_MAX,

	ePLAYER_MOVE_L01,
	ePLAYER_MOVE_L02,
	ePLAYER_MOVE_L03,
	ePLAYER_MOVE_L04,
	ePLAYER_MOVE_L05,
	ePLAYER_MOVE_L06,
	ePLAYER_MOVE_L07,
	ePLAYER_MOVE_L08,
	ePLAYER_MOVE_L09,
	ePLAYER_MOVE_L10,
	ePLAYER_MOVE_L11,
	ePLAYER_MOVE_L12,
	ePLAYER_MOVE_L_MAX,

	ePLAYER_MOVE_R01,
	ePLAYER_MOVE_R02,
	ePLAYER_MOVE_R03,
	ePLAYER_MOVE_R04,
	ePLAYER_MOVE_R05,
	ePLAYER_MOVE_R06,
	ePLAYER_MOVE_R07,
	ePLAYER_MOVE_R08,
	ePLAYER_MOVE_R09,
	ePLAYER_MOVE_R10,
	ePLAYER_MOVE_R11,
	ePLAYER_MOVE_R12,
	ePLAYER_MOVE_R_MAX,

	ePLAYER_ATTACK1_L01,
	ePLAYER_ATTACK1_L02,
	ePLAYER_ATTACK1_L03,
	ePLAYER_ATTACK1_L04,
	ePLAYER_ATTACK1_L_MAX,

	ePLAYER_ATTACK1_R01,
	ePLAYER_ATTACK1_R02,
	ePLAYER_ATTACK1_R03,
	ePLAYER_ATTACK1_R04,
	ePLAYER_ATTACK1_R_MAX,

	ePLAYER_ATTACK2_L01,
	ePLAYER_ATTACK2_L02,
	ePLAYER_ATTACK2_L03,
	ePLAYER_ATTACK2_L04,
	ePLAYER_ATTACK2_L_MAX,

	ePLAYER_ATTACK2_R01,
	ePLAYER_ATTACK2_R02,
	ePLAYER_ATTACK2_R03,
	ePLAYER_ATTACK2_R04,
	ePLAYER_ATTACK2_R_MAX,

	ePLAYER_ATTACK3_L01,
	ePLAYER_ATTACK3_L02,
	ePLAYER_ATTACK3_L03,
	ePLAYER_ATTACK3_L04,
	ePLAYER_ATTACK3_L05,
	ePLAYER_ATTACK3_L06,
	ePLAYER_ATTACK3_L_MAX,

	ePLAYER_ATTACK3_R01,
	ePLAYER_ATTACK3_R02,
	ePLAYER_ATTACK3_R03,
	ePLAYER_ATTACK3_R04,
	ePLAYER_ATTACK3_R05,
	ePLAYER_ATTACK3_R06,
	ePLAYER_ATTACK3_R_MAX,

	eEFFECT_SPARK_01,
	eEFFECT_SPARK_02,
	eEFFECT_SPARK_03,
	eEFFECT_SPARK_04,
	eEFFECT_SPARK_MAX,
	
	eGUAGE_HP,
	eSHADOW,
	eSPRITE_MAX
};
#endif//	!__COMMON__