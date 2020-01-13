#pragma once
class Actor
{
public:
	virtual int GetPos_X()                = 0;
	virtual int GetPos_Y()                = 0;
	virtual void SetPos_X(int)            = 0;
	virtual void SetPos_Y(int)            = 0;
	virtual char GetDirection()           = 0;
	virtual void SetDirection(char)       = 0;
	virtual bool CheckCollision(char dir) = 0;
	virtual void Dead()                   = 0;
};

