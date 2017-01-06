#pragma once
class CStage
{
public:
	CStage();
	virtual ~CStage();

	virtual void running() = 0;
};

