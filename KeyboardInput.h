#pragma once
#ifndef KEYBOARDINPUT_H
#define KEYBOARDINPUT_H

#include <WinUser.h>

class Keyboard 
{
public:
    virtual bool isKeyDown(int keyCode) { return (GetAsyncKeyState(keyCode) & 0x8000) ? 1 : 0; }
    virtual bool isKeyUp  (int keyCode) { return (GetAsyncKeyState(keyCode) & 0x8000) ? 0 : 1; }
    virtual ~Keyboard() {};
};
#endif