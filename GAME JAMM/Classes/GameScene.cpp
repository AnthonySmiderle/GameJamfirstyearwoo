#include "GameScene.h"
#include "MainMenuScene.h"
#include <iostream>

Scene* Gameplay::sceneHandle = nullptr;

Scene* Gameplay::createScene() {
	sceneHandle = Gameplay::create();
	return sceneHandle;
}

void Gameplay::onEnter() { Scene::onEnter(); }

bool Gameplay::init() {
	if(!Scene::init()) return false;

	director = Director::getInstance();

	windowSize = director->getWinSizeInPixels();
	origin = director->getVisibleOrigin();

	initSprites();
	initPauseMenu();
	initHUD();

	initMouseListener();
	initKeyboardListener();

	this->scheduleUpdate();

	return true;
}

void Gameplay::onExit() { Scene::onExit(); }

void Gameplay::initSprites() {

	itemHitCircle1 = g3nts::PrimitiveCircle(Vec2(50,200), 5, 5, 40, false, Color4F(1.0f, 0.0f, 0.0f, 1.0f));
	itemHitCircle2 = g3nts::PrimitiveCircle(Vec2(500,200), 5, 5, 40, false, Color4F(1.0f, 0.0f, 0.0f, 1.0f));
	playerHitCircle = g3nts::PrimitiveCircle(Vec2(200, 200), 10, 5, 40, false, Color4F(1.0f, 0.0f, 0.0f, 1.0f));

	background = Sprite::create("backgrounds/MainMenuBG.png");
	background->setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
	background->setAnchorPoint(Vec2(0.5f, 0.5f));

	cameraTarget = Sprite::create();

	items.push_back(itemHitCircle1);
	items.push_back(itemHitCircle2);

	for (g3nts::PrimitiveCircle item : items) this->addChild(item.getNode());
	this->addChild(playerHitCircle.getNode(), 1);

	this->addChild(background, -100);
	this->addChild(cameraTarget);
}

void Gameplay::initPauseMenu() {
	Label* pausedLabel = Label::create("PAUSED", "fonts/Roboto/Roboto-Regular.ttf", 48, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	pausedLabel->enableShadow();

	Label* resumeLabel = Label::create("Resume Game", "fonts/Roboto/Roboto-Regular.ttf", 24, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::BOTTOM);
	resumeLabel->enableShadow();

	Label* mainMenuLabel = Label::create("Back to Main Menu", "fonts/Roboto/Roboto-Regular.ttf", 24, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::BOTTOM);
	mainMenuLabel->enableShadow();

	MenuItem* pausedItem = MenuItemLabel::create(pausedLabel, NULL);
	MenuItem* resumeButton = MenuItemLabel::create(resumeLabel, [&](Ref* sender) { togglePause(); });
	MenuItem* mainMenuButton = MenuItemLabel::create(mainMenuLabel, [&](Ref* sender) { quitToMainMenu(); });

	pausedItem->setPosition(0, 100);
	resumeButton->setPosition(0, -200);
	mainMenuButton->setPosition(0, -250);
	
	pauseMenu = Menu::create(pausedItem, resumeButton, mainMenuButton, NULL);

	this->addChild(pauseMenu, 100);
	pauseMenu->setVisible(false);
}

void Gameplay::togglePause() {
	paused = !paused;

	if (paused) {
		playerPos = playerHitCircle.getPosition();

		playerHitCircle.getNode()->setVisible(false);
		HUD->setVisible(false);
		for (g3nts::PrimitiveCircle item : items) {
			item.getNode()->setVisible(false);
		}

		pauseMenu->setVisible(true);
	}
	else {
		pauseMenu->setVisible(false);
		HUD->setVisible(true);

		playerHitCircle.setPosition(playerPos);
		playerHitCircle.redraw();
		playerHitCircle.getNode()->setVisible(true);
		for (g3nts::PrimitiveCircle item : items) {
			item.getNode()->setVisible(true);
		}
	}
}

void Gameplay::quitToMainMenu() {
	Scene* menuScene = MainMenu::createScene();
	director->replaceScene(TransitionFade::create(2.0f, menuScene));
}

void Gameplay::initHUD() {
	MenuItemImage* inventoryItem = MenuItemImage::create("square.png", "square.png");
	inventoryItem->setPosition(windowSize.x / 2.0f - inventoryItem->getContentSize().width, -windowSize.y / 2.0f +inventoryItem->getContentSize().height);

	HUD = Menu::create(inventoryItem, NULL);
	this->addChild(HUD, 99);
	HUD->setVisible(true);
}

void Gameplay::update(float dt) {

	//manager.update();

	for (int i = 0; i < items.size(); i++) {
		if (g3nts::isColliding(playerHitCircle, items[i])) {

			if (inventory.size() == 0) {
				items[i].getNode()->setVisible(false);

				inventory.push_back(items[i]);
				items.erase(items.begin() + i);
				i--;

			}
		}
	}

	for (g3nts::PrimitiveCircle item : inventory) {
		item.getNode()->setAnchorPoint(Vec2(0.5f, 0.5f));
		item.getNode()->setLocalZOrder(150);
		item.setPosition(Vec2(origin.x + windowSize.x - 125,
							  origin.y + 125));
		item.getNode()->setVisible(true);
		item.redraw();
	}

	checkUp();
	checkDown();
	checkLeft();
	checkRight();



	//checkStart();








	
}

void Gameplay::initMouseListener() {
	mouseListener = EventListenerMouse::create();

	mouseListener->onMouseDown = CC_CALLBACK_1(Gameplay::mouseDownCallBack, this);
	mouseListener->onMouseUp = CC_CALLBACK_1(Gameplay::mouseUpCallBack, this);
	mouseListener->onMouseMove = CC_CALLBACK_1(Gameplay::mouseMoveCallBack, this);
	mouseListener->onMouseScroll = CC_CALLBACK_1(Gameplay::mouseScrollCallBack, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void Gameplay::mouseDownCallBack(cocos2d::Event* mEvent) {
	EventMouse* mouseEvent = dynamic_cast<EventMouse*>(mEvent);

	auto mouseClickPosition = mouseEvent->getLocationInView();
	mouseClickPosition.y += 540;

	auto mouseButton = mouseEvent->getMouseButton();
	mouseDown = true;
}

void Gameplay::mouseUpCallBack(cocos2d::Event * mEvent) {
	EventMouse* mouseEvent = dynamic_cast<EventMouse*>(mEvent);

	auto mouseUpPosition = mouseEvent->getLocationInView();
	mouseUpPosition.y += 540;

	auto mouseButton = mouseEvent->getMouseButton();
	mouseDown = false;
}

void Gameplay::mouseMoveCallBack(cocos2d::Event * mEvent) {
	EventMouse* mouseEvent = dynamic_cast<EventMouse*>(mEvent);

	auto mouseEventPos = mouseEvent->getLocationInView();
	this->mouse.position = Vec2(mouseEventPos.x, 540 + mouseEventPos.y);

	mousePosition = mouse.position;
}

void Gameplay::mouseScrollCallBack(cocos2d::Event * mEvent) {}

void Gameplay::initKeyboardListener() {
	keyboardListener = EventListenerKeyboard::create();

	keyboardListener->onKeyPressed = CC_CALLBACK_2(Gameplay::keyDownCallback, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(Gameplay::keyUpCallback, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void Gameplay::keyDownCallback(EventKeyboard::KeyCode keyCode, Event* kEvent) {
	keyboard.keyDown[(int)keyCode] = true;
}

void Gameplay::keyUpCallback(EventKeyboard::KeyCode keyCode, Event* kEvent) {
	keyboard.keyDown[(int)keyCode] = false;
	
	typedef EventKeyboard::KeyCode key;
	switch (keyCode) {
	case key::KEY_ESCAPE:
		togglePause();
		break;
	
	case key::KEY_SPACE:
		if (inventory.size() > 0) {
			inventory[0].getNode()->setLocalZOrder(0);
			inventory[0].setPosition(Vec2(origin.x + playerHitCircle.getPosition().x + playerHitCircle.getRadius() + 5,
										  origin.y + playerHitCircle.getPosition().y - playerHitCircle.getRadius() - 10));
			inventory[0].redraw();

			items.push_back(inventory[0]);
			inventory.erase(inventory.begin());
		}
	}

}


//void Gameplay::initControllerListener() {
//	controllerListener = EventListenerController::create();
//
//	controllerListener->onAxisEvent = CC_CALLBACK_3(Gameplay::cAxisEventCallBack, this);
//	controllerListener->onKeyDown = CC_CALLBACK_3(Gameplay::cKeyDownCallBack, this);
//	controllerListener->onKeyUp = CC_CALLBACK_3(Gameplay::cKeyUpCallBack, this);
//
//	_eventDispatcher->addEventListenerWithSceneGraphPriority(controllerListener, this);
//}
//
//void Gameplay::cKeyDownCallBack(Controller* controller, int keyCode, Event* cEvent) {
//	gamepad.deviceID = controller->getDeviceId();
//	gamepad.keyDown[keyCode] = true;
//}
//
//void Gameplay::cKeyUpCallBack(Controller* controller, int keyCode, Event* cEvent) {
//	gamepad.deviceID = controller->getDeviceId();
//	gamepad.keyDown[keyCode] = false;
//
//	cocos2d::log("Keycode: %i", keyCode);
//}
//
//void Gameplay::cAxisEventCallBack(Controller* controller, int keyCode, Event* cEvent) {
//	gamepad.deviceID = controller->getDeviceId();
//}


void Gameplay::checkUp() {
	if (/*manager.getController(0)->isButtonPressed(SednaInput::DPAD_UP)
		|| */keyboard.keyDown[(int)EventKeyboard::KeyCode::KEY_W]) {
		playerHitCircle.setPosition(playerHitCircle.getPosition() + cocos2d::Vec2(0, 5));
		playerHitCircle.redraw();
	}
}

void Gameplay::checkDown() {
	if (/*manager.getController(0)->isButtonPressed(SednaInput::DPAD_DOWN)
		||*/ keyboard.keyDown[(int)EventKeyboard::KeyCode::KEY_S]) {
		playerHitCircle.setPosition(playerHitCircle.getPosition() + cocos2d::Vec2(0, -5));
		playerHitCircle.redraw();
	}
}

void Gameplay::checkLeft() {
	if (/*manager.getController(0)->isButtonPressed(SednaInput::DPAD_LEFT)
		||*/ keyboard.keyDown[(int)EventKeyboard::KeyCode::KEY_A]) {
		playerHitCircle.setPosition(playerHitCircle.getPosition() + cocos2d::Vec2(-5, 0));
		playerHitCircle.redraw();
	}
}

void Gameplay::checkRight() {
	if (/*manager.getController(0)->isButtonPressed(SednaInput::DPAD_RIGHT)
		||*/ keyboard.keyDown[(int)EventKeyboard::KeyCode::KEY_D]) {
		playerHitCircle.setPosition(playerHitCircle.getPosition() + cocos2d::Vec2(5, 0));
		playerHitCircle.redraw();
	}
}

//void Gameplay::checkStart() {
//	if (manager.getController(0)->isButtonPressed(SednaInput::START)) {
//		this->unscheduleUpdate();
//		
//		if (manager.getController(0)->isButtonReleased(SednaInput::START)) togglePause();
//		else checkStart();
//		
//		this->scheduleUpdate();
//	}
//}
