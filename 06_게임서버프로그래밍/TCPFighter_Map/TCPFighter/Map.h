#pragma once

class CMap
{
public:
	CMap();
	~CMap();

	void	DrawTile(BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen = 100);
	void	SetDrawPos(int iPosX, int iPosY);

	// draw ÁÂÇ¥ ¾ò±â
	int		GetDrawPosX(void);
	int		GetDrawPosY(void);

private:
	int		m_iDrawPosX;
	int		m_iDrawPosY;
};