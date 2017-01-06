#ifndef Object__H__
#define Object__H__


class My_Object
{
public:
	// 생성자
	My_Object();

	// 파괴자
	//virtual ~My_Object();
	virtual ~My_Object();

	virtual void Print(void) = 0;	// 출력
	virtual void InputData(void);
	bool CheckPos(int x, int y);

protected:
	int		_x;
	int		_y;
	char	_name[16];
};



#endif	//Object__H__