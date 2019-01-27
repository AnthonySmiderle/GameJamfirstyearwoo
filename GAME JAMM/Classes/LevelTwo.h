#pragma once
#include "cocos2d.h"
#include "Primitives.h"
#include <cstdio>
#include <random>
#include "STRUCTS.h"
#include "HidingPlace.h"
//#include "XinputManager.h"

USING_NS_CC;



//struct Gamepad {
//	int deviceID;
//	bool keyDown[16];
//};

class LevelTwo : public cocos2d::Scene {
public:
	static cocos2d::Scene* createScene();
	
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	CREATE_FUNC(LevelTwo);

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

	bool isInHidingPlace(g3nts::PrimitiveCircle c, HidingPlace h);
	bool isInHidingPlace(g3nts::PrimitiveRect r, HidingPlace h);

private:



	/////////////////

	HidingPlace cabinet2;



	Sprite* dining;
	Sprite* hallway;
	Sprite* stair;
	Sprite* garage;


	bool isInKitchen = true;
	bool isInDining = false;
	bool isInHallway = false;
	bool isInStair = false;
	bool isInGarage = false;

	/////////////////
	int microwaveSound;
	Label* scoreLabel;
	Label* scoreLabelInt;
	int x;
	int score = 0;
	bool die = false;
	bool isInRoom = false;
	bool isHiding = false;
	bool microwaving = false;
	g3nts::PrimitiveRect cabinet;
	g3nts::PrimitiveRect momBox;
	Label* hide;
	HidingPlace microBox;

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

	bool paused = false;
	Menu* pauseMenu;
	Menu* HUD;

	g3nts::PrimitiveRect playerHitBox;
	Vec2 playerStart, playerEnd;

	std::vector<g3nts::PrimitiveCircle> inventory;

	std::vector<g3nts::PrimitiveCircle> items;
	
	g3nts::PrimitiveCircle itemHitCircle1;
	g3nts::PrimitiveCircle itemHitCircle2;

	Vec2 windowSize;
	Vec2 origin;
	float topBoundary = 280.0f;
	
	/*SednaInput::XinputController* p1;
	SednaInput::XinputManager manager;
	SednaInput::Stick sticks[2];*/
};

