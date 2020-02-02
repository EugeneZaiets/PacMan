#ifndef KEYBOARDINPUT_H
#define KEYBOARDINPUT_H

#include <WinUser.h>

class Keyboard 
{
public:
    virtual bool isKeyDown          (int keyCode) { return (GetAsyncKeyState(keyCode) & 0x8000) ? 1 : 0; }
    virtual bool isKeyOncePressed   (int keyCode) { return (GetKeyState     (keyCode) & 0x0001) ? 1 : 0; }
    virtual bool isKeyPressed       (int keyCode) { return (GetAsyncKeyState(keyCode) & 0x0001) ? 1 : 0; }
    virtual bool isKeyUp            (int keyCode) { return (GetAsyncKeyState(keyCode) & 0x8000) ? 0 : 1; }
    virtual void resetAsyncKeyState (int keyCode) { GetAsyncKeyState(keyCode) & 0x0000;                   }
    virtual ~Keyboard() {};
};
#endif