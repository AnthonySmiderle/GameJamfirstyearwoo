#pragma once

#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include "XinputManager.h"


class MainMenu : public cocos2d::Scene {
public:
	static cocos2d::Scene* createScene();
	
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	CREATE_FUNC(MainMenu);

	void initMenu();
	void initMouseListener();
	void initKeyboardListener();
	void initXinputListener();

private:
	SednaInput::XinputManager menuCManager;

	cocos2d::Director* director;
	
	cocos2d::EventListenerMouse* mouseListener;
	cocos2d::EventListenerKeyboard* keyboardListener;
	
	cocos2d::Vec2 mousePosition;
	
	cocos2d::Sprite* background;
	cocos2d::Label* title;
	cocos2d::Menu* menu;

};

#endif // __MAINMENU_SCENE_H__