#include "GameScene.h"
#include "MainMenuScene.h"
#include <iostream>
#include "AudioEngine.h"

Scene* Gameplay::sceneHandle = nullptr;

Scene* Gameplay::createScene() {
	sceneHandle = Gameplay::create();
	return sceneHandle;
}

void Gameplay::onEnter() { Scene::onEnter(); }

bool Gameplay::init() {
	if(!Scene::init()) return false;

	srand(time(0));
	director = Director::getInstance();
	x = rand() % 100;
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
	cocos2d::experimental::AudioEngine::play2d("mMusic.mp3",true);


	Vec2 windowSize = director->getWinSizeInPixels();

	itemHitCircle = g3nts::PrimitiveCircle(cocos2d::Vec2(50,200), 5, 5, 40, false, Color4F(1.0f, 0.0f, 0.0f, 1.0f));
	playerHitCircle = g3nts::PrimitiveCircle(cocos2d::Vec2(200, 200), 10, 5, 40, false, Color4F(1.0f, 0.0f, 0.0f, 1.0f));

	scare = Sprite::create("devito.jpg");
	scare->setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
	background = Sprite::create("backgrounds/MainMenuBGdark.png");
	background->setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
	background->setAnchorPoint(Vec2(0.5f, 0.5f));

	cameraTarget = Sprite::create();

	items.push_back(itemHitCircle);

	this->addChild(cabnet.getNode(), 1);
	this->addChild(momBox.getNode(), 2);
	momBox.getNode()->setVisible(false);

	this->addChild(itemHitCircle.getNode(), 1);
	this->addChild(playerHitCircle.getNode(), 1);

	this->addChild(background, -100);
	this->addChild(scare, 1000);
	scare->setVisible(false);
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
	Vec2 windowSize = director->getWinSizeInPixels();

	MenuItemImage* inventoryItem = MenuItemImage::create("square.png", "square.png");
	inventoryItem->setPosition(windowSize.x / 2.0f - inventoryItem->getContentSize().width, -windowSize.y / 2.0f +inventoryItem->getContentSize().height);

	HUD = Menu::create(inventoryItem, NULL);
	this->addChild(HUD, 100);
	HUD->setVisible(true);
}

void Gameplay::update(float dt) {

	//manager.update();


	x++;

	for (int i = 0; i < items.size(); i++) {
		if (g3nts::isColliding(playerHitCircle, items[i])) {
			removeChild(items[i].getNode(), 1);
			items[i].getNode()->clear();
			items.erase(items.begin() + i);
			i--;
		}
	}


	if (playerHitCircle.getPosition().x > 35 && playerHitCircle.getPosition().x < 110 && keyboard.keyDown[(int)EventKeyboard::KeyCode::KEY_SPACE]) {

		isHiding = true;
	}
	else
		isHiding = false;

	if (x == 500 && !isHiding) {
		cocos2d::experimental::AudioEngine::play2d("ree.mp3");
		scare->setVisible(true);
		die = true;
	}
	else if (x == 500 && isHiding) {

		momBox.getNode()->setVisible(true);
		cocos2d::experimental::AudioEngine::play2d("mom_music.mp3");

		//play different audio file

	}
	if (x == 700 && die)
		exit(0);
	else if (x == 1000 && !die) {
		momBox.getNode()->setVisible(false);
		x = rand() % 100;
	}

	//checkUp();
	//checkDown();
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

	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) togglePause();
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
