#pragma once

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Primitives.h"
#include <cstdio>
#include <random>
#include "HidingPlace.h"
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
	
	void checkUp();
	void checkDown();
	void checkLeft();
	void checkRight();
	

	//void checkStart();



private:

	Label* scoreLabel;
	Label* scoreLabelInt;
	int x;
	int score = 0;
	bool die = false;
	bool isInRoom = false;
	bool isHiding = false;
	bool microwaving = false;
	g3nts::PrimitiveRect cabnet{ cocos2d::Vec2(35,210),cocos2d::Vec2(110,350) };
	g3nts::PrimitiveRect momBox{ cocos2d::Vec2(1000,200),cocos2d::Vec2(1100,500),cocos2d::Color4F(0,1,0,1) };
	Label* hide;
	HidingPlace microBox{ cocos2d::Vec2(390,400),cocos2d::Vec2(520,430) };

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

	Sprite* scare;
	Sprite* stupidMicrowave;
	Sprite* background;
	Sprite* cameraTarget;

	bool paused = false;
	Menu* pauseMenu;
	Menu* HUD;

	g3nts::PrimitiveCircle playerHitCircle;
	Vec2 playerPos;

	std::vector<g3nts::PrimitiveCircle> inventory;

	std::vector<g3nts::PrimitiveCircle> items;
	
	g3nts::PrimitiveCircle itemHitCircle1;
	g3nts::PrimitiveCircle itemHitCircle2;

	Vec2 windowSize;
	Vec2 origin;
	
	/*SednaInput::XinputController* p1;
	SednaInput::XinputManager manager;
	SednaInput::Stick sticks[2];*/
};

#endif // __GAME_SCENE_H__