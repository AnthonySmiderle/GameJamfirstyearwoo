#pragma once
#include "cocos2d.h"
struct Mouse {
	cocos2d::Vec2 position;
};

struct Keyboard {
	bool keyDown[256];
};