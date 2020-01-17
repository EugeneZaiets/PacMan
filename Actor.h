#pragma once
class iActor
{
public:
	virtual int getPos_X()                = 0;
	virtual int getPos_Y()                = 0;
	virtual void setPos_X(int)            = 0;
	virtual void setPos_Y(int)            = 0;
	virtual char getDirection()           = 0;
	virtual void setDirection(char)       = 0;
	virtual bool checkCollision(char dir) = 0;
	virtual void dead()                   = 0;
    virtual ~iActor(){};
};

