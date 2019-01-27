#pragma once

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Primitives.h"
#include <cstdio>
#include <random>
//#include "XinputManager.h"

USING_NS_CC;

struct Mouse {
	cocos2d::Vec2 position;
};

struct Keyboard {
	bool keyDown[256];
};

//struct Gamepad {
//	int deviceID;
//	bool keyDown[16];
//};

class Gameplay : public cocos2d::Scene {
public:
	static cocos2d::Scene* createScene();
	
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	CREATE_FUNC(Gameplay);

	void initSprites();
	void initHUD();
	void initPauseMenu();

	void togglePause();
	void quitToMainMenu();
	
	void update(float dt);

	void initMouseListener();
	void mouseDownCallBack(Event* mEvent);
	void mouseUpCallBack(Event* mEvent);
	void mouseMoveCallBack(Event* mEvent);
	void mouseScrollCallBack(Event* mEvent);

	void initKeyboardListener();
	void keyDownCallback(EventKeyboard::KeyCode keyCode, Event* kEvent);
	void keyUpCallback(EventKeyboard::KeyCode keyCode, Event* kEvent);

	//void initControllerListener();
	//void cKeyDownCallBack(Controller* controller, int keyCode, Event* cEvent);
	//void cKeyUpCallBack(Controller* controller, int keyCode, Event* cEvent);
	//void cAxisEventCallBack(Controller* controller, int keyCode, Event* cEvent);
	
	/*void checkUp();
	void checkDown();
	void checkLeft();
	void checkRight();*/

	//void checkStart();

private:

	Director* director;
	static cocos2d::Scene* sceneHandle;

	EventListenerMouse* mouseListener;
	Vec2 mousePosition;
	Mouse mouse;
	bool mouseDown;
	
	EventListenerKeyboard* keyboardListener;
	Keyboard keyboard;

	/*EventListenerController* controllerListener;
	Gamepad gamepad;*/
	
	/*SednaInput::XinputManager manager;
	SednaInput::XinputController* p1;
	SednaInput::Stick sticks[2];*/

	Sprite* background;
	Sprite* scare;

	bool paused = false;
	Menu* pauseMenu;
	Menu* HUD;

	g3nts::PrimitiveRect playerHitBox;
	Vec2 playerStart, playerEnd;

	std::vector<g3nts::PrimitiveCircle> inventory;

	std::vector<g3nts::PrimitiveCircle> items;
	
	g3nts::PrimitiveCircle itemHitCircle1;
	g3nts::PrimitiveCircle itemHitCircle2;
	
	g3nts::PrimitiveRect microwave;
	g3nts::PrimitiveRect cabinet;
	g3nts::PrimitiveRect momBox;
	
	int momTimer;
	bool die = false;
	bool isInRoom = false;
	bool isHiding = false;
	
	Vec2 windowSize;
	Vec2 origin;
	float topBoundary = 280.0f;
};

#endif // __GAME_SCENE_H__