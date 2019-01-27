#include "GameScene.h"
#include "MainMenuScene.h"
#include <iostream>
#include "AudioEngine.h"

Scene* Level1::sceneHandle = nullptr;

Scene* Level1::createScene() {
	sceneHandle = Level1::create();
	return sceneHandle;
}

void Level1::onEnter() { Scene::onEnter(); }

bool Level1::init() {
	if(!Scene::init()) return false;

	srand(time(0));
	director = Director::getInstance();
	momTimer = rand() % 100;

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

void Level1::onExit() { Scene::onExit(); }

void Level1::initSprites() {
	Vec2 windowSize = director->getWinSizeInPixels();

	background = Sprite::create("backgrounds/MainMenuBGdark.png");
	background = Sprite::create("backgrounds/MainMenuBGdark.png");
	background->setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
	background->setAnchorPoint(Vec2(0.5f, 0.5f));

	playerHitBox = g3nts::PrimitiveRect(Vec2(200, 100), Vec2(250, 300), Color4F(1, 1, 1, 1));
	
	itemHitCircle1 = g3nts::PrimitiveCircle(Vec2(50, 200), 5, 5, 40, false, Color4F(1, 0, 1, 1));
	itemHitCircle2 = g3nts::PrimitiveCircle(Vec2(500, 200), 5, 5, 40, false, Color4F(1, 0, 1, 1));
	
	items.push_back(itemHitCircle1);
	items.push_back(itemHitCircle2);
	
	cabinet = g3nts::PrimitiveRect(Vec2(35, 210), Vec2(110, 350));
	momBox = g3nts::PrimitiveRect(Vec2(1000, 200), Vec2(1100, 500), Color4F(0, 1, 0, 1));
	microwave = g3nts::PrimitiveRect(Vec2(395, 370), Vec2(490, 435), Color4F(0, 1, 1, 1));

	scare = Sprite::create("mom.png");
	scare->setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	// Add everything to the scene
	this->addChild(background, -100);
	
	this->addChild(playerHitBox.getNode(), 10);
	for (g3nts::PrimitiveCircle item : items) this->addChild(item.getNode(), 5);

	this->addChild(cabinet.getNode(), 1);
	this->addChild(microwave.getNode(), 1);
	this->addChild(momBox.getNode(), 2);

	this->addChild(scare, 1000);
	
	scare->setVisible(false);
	momBox.getNode()->setVisible(false);
}

void Level1::initPauseMenu() {
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

void Level1::togglePause() {
	paused = !paused;

	if (paused) {
		playerStart = playerHitBox.getStartPosition();
		playerEnd = playerHitBox.getEndPosition();

		playerHitBox.getNode()->setVisible(false);
		HUD->setVisible(false);
		for (g3nts::PrimitiveCircle item : items) {
			item.getNode()->setVisible(false);
		}

		pauseMenu->setVisible(true);
	}
	else {
		pauseMenu->setVisible(false);
		HUD->setVisible(true);

		playerHitBox.setNewPositions(playerStart, playerEnd);
		playerHitBox.redraw();
		playerHitBox.getNode()->setVisible(true);
		for (g3nts::PrimitiveCircle item : items) {
			item.getNode()->setVisible(true);
		}
	}
}

void Level1::quitToMainMenu() {
	Scene* menuScene = MainMenu::createScene();
	director->replaceScene(TransitionFade::create(2.0f, menuScene));
}

void Level1::initHUD() {
	MenuItemImage* inventoryItem = MenuItemImage::create("square.png", "square.png");
	inventoryItem->setPosition(windowSize.x / 2.0f - inventoryItem->getContentSize().width, -windowSize.y / 2.0f +inventoryItem->getContentSize().height);

	HUD = Menu::create(inventoryItem, NULL);
	this->addChild(HUD, 99);
	HUD->setVisible(true);
}

void Level1::update(float dt) {

	//manager.update();
	
	momTimer++;

	for (int i = 0; i < items.size(); i++) {
		items[i].redraw();

		if (g3nts::isColliding(playerHitBox, items[i])) {
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

	if (playerHitBox.getCentrePosition().x > 35 && playerHitBox.getCentrePosition().x < 110 &&
		keyboard.keyDown[(int)EventKeyboard::KeyCode::KEY_SPACE])
		isHiding = true;
	else
		isHiding = false;

	if (momTimer == 700) {
		cocos2d::experimental::AudioEngine::play2d("mMusic.mp3", true);

	}
	if (momTimer == 1000 && !isHiding) {
		cocos2d::experimental::AudioEngine::play2d("mom_music.mp3");
		cocos2d::experimental::AudioEngine::play2d("ree.mp3");
		scare->setVisible(true);
		die = true;
	}
	else if (momTimer == 1000 && isHiding) {

		momBox.getNode()->setVisible(true);
		cocos2d::experimental::AudioEngine::play2d("mom_music.mp3");

		//play different audio file

	}
	if (momTimer == 1100 && die)
		exit(0);
	else if (momTimer == 1500 && !die) {
		momBox.getNode()->setVisible(false);
		momTimer = rand() % 100;
	}

	if (playerHitBox.getCentrePosition().y >= origin.y && playerHitBox.getCentrePosition().y <= topBoundary) {

		if (/*manager.getController(0)->isButtonPressed(SednaInput::DPAD_UP)
			|| */keyboard.keyDown[(int)EventKeyboard::KeyCode::KEY_W]) {
			playerHitBox.setNewPositions((playerHitBox.getStartPosition() + cocos2d::Vec2(0, 5)), playerHitBox.getEndPosition() + cocos2d::Vec2(0, 5));
			playerHitBox.redraw();
		}
		else if (/*manager.getController(0)->isButtonPressed(SednaInput::DPAD_DOWN)
			||*/ keyboard.keyDown[(int)EventKeyboard::KeyCode::KEY_S]) {
			playerHitBox.setNewPositions(playerHitBox.getStartPosition() + cocos2d::Vec2(0, -5), playerHitBox.getEndPosition() + cocos2d::Vec2(0, -5));
			playerHitBox.redraw();
		}

	}
	else {
		if (playerHitBox.getCentrePosition().y > topBoundary) {
			playerHitBox.setNewPositions(
				Vec2(playerHitBox.getStartPosition().x,
					origin.y + topBoundary + (playerHitBox.getEndPosition().y - playerHitBox.getStartPosition().y) / 2.0f),
				Vec2(playerHitBox.getEndPosition().x,
					origin.y + topBoundary - (playerHitBox.getEndPosition().y - playerHitBox.getStartPosition().y) / 2.0f)
			);
			playerHitBox.redraw();
		}

		else if (playerHitBox.getCentrePosition().y < origin.y) {
			playerHitBox.setNewPositions(
				Vec2(playerHitBox.getStartPosition().x,
					origin.y + (playerHitBox.getEndPosition().y - playerHitBox.getStartPosition().y) / 2.0f),
				Vec2(playerHitBox.getEndPosition().x,
					origin.y - (playerHitBox.getEndPosition().y - playerHitBox.getStartPosition().y) / 2.0f)
			);
			playerHitBox.redraw();
		}
	}


	if (playerHitBox.getCentrePosition().x >= origin.x && playerHitBox.getCentrePosition().x <= origin.x + windowSize.x) {
		if (/*manager.getController(0)->isButtonPressed(SednaInput::DPAD_LEFT)
		||*/ keyboard.keyDown[(int)EventKeyboard::KeyCode::KEY_A]) {
			playerHitBox.setNewPositions(playerHitBox.getStartPosition() + cocos2d::Vec2(-5, 0), playerHitBox.getEndPosition() + cocos2d::Vec2(-5, 0));
			playerHitBox.redraw();
		}
		else if (/*manager.getController(0)->isButtonPressed(SednaInput::DPAD_RIGHT)
			||*/ keyboard.keyDown[(int)EventKeyboard::KeyCode::KEY_D]) {
			playerHitBox.setNewPositions(playerHitBox.getStartPosition() + cocos2d::Vec2(5, 0), playerHitBox.getEndPosition() + cocos2d::Vec2(5, 0));
			playerHitBox.redraw();
		}
	}
	else {
		if (playerHitBox.getCentrePosition().x > origin.x + windowSize.x) {
			playerHitBox.setNewPositions(
				Vec2(origin.x + windowSize.x + (playerHitBox.getEndPosition().x - playerHitBox.getStartPosition().x) / 2.0f,
					 playerHitBox.getStartPosition().y),
				Vec2(origin.x + windowSize.x - (playerHitBox.getEndPosition().x - playerHitBox.getStartPosition().x) / 2.0f,
					playerHitBox.getEndPosition().y)
			);
			playerHitBox.redraw();
		}
		else if (playerHitBox.getCentrePosition().x < origin.x) {
			playerHitBox.setNewPositions(
				Vec2(origin.x + (playerHitBox.getEndPosition().x - playerHitBox.getStartPosition().x) / 2.0f,
					playerHitBox.getStartPosition().y),
				Vec2(origin.x - (playerHitBox.getEndPosition().x - playerHitBox.getStartPosition().x) / 2.0f,
					playerHitBox.getEndPosition().y)
			);
			playerHitBox.redraw();
		}
	}
}

void Level1::initMouseListener() {
	mouseListener = EventListenerMouse::create();

	mouseListener->onMouseDown = CC_CALLBACK_1(Level1::mouseDownCallBack, this);
	mouseListener->onMouseUp = CC_CALLBACK_1(Level1::mouseUpCallBack, this);
	mouseListener->onMouseMove = CC_CALLBACK_1(Level1::mouseMoveCallBack, this);
	mouseListener->onMouseScroll = CC_CALLBACK_1(Level1::mouseScrollCallBack, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void Level1::mouseDownCallBack(cocos2d::Event* mEvent) {
	EventMouse* mouseEvent = dynamic_cast<EventMouse*>(mEvent);

	auto mouseClickPosition = mouseEvent->getLocationInView();
	mouseClickPosition.y += 540;

	auto mouseButton = mouseEvent->getMouseButton();
	mouseDown = true;
}

void Level1::mouseUpCallBack(cocos2d::Event * mEvent) {
	EventMouse* mouseEvent = dynamic_cast<EventMouse*>(mEvent);

	auto mouseUpPosition = mouseEvent->getLocationInView();
	mouseUpPosition.y += 540;

	auto mouseButton = mouseEvent->getMouseButton();
	mouseDown = false;
}

void Level1::mouseMoveCallBack(cocos2d::Event * mEvent) {
	EventMouse* mouseEvent = dynamic_cast<EventMouse*>(mEvent);

	auto mouseEventPos = mouseEvent->getLocationInView();
	this->mouse.position = Vec2(mouseEventPos.x, 540 + mouseEventPos.y);

	mousePosition = mouse.position;
}

void Level1::mouseScrollCallBack(cocos2d::Event * mEvent) {}

void Level1::initKeyboardListener() {
	keyboardListener = EventListenerKeyboard::create();

	keyboardListener->onKeyPressed = CC_CALLBACK_2(Level1::keyDownCallback, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(Level1::keyUpCallback, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void Level1::keyDownCallback(EventKeyboard::KeyCode keyCode, Event* kEvent) {
	keyboard.keyDown[(int)keyCode] = true;
}

void Level1::keyUpCallback(EventKeyboard::KeyCode keyCode, Event* kEvent) {
	keyboard.keyDown[(int)keyCode] = false;
	
	typedef EventKeyboard::KeyCode key;
	switch (keyCode) {
	case key::KEY_ESCAPE:
		togglePause();
		break;
	
	case key::KEY_E:
		if (inventory.size() > 0) {
			inventory[0].getNode()->setLocalZOrder(5);
			inventory[0].setPosition(Vec2(origin.x + playerHitBox.getEndPosition().x + 20,
										  origin.y + playerHitBox.getStartPosition().y - 20));
			inventory[0].redraw();

			items.push_back(inventory[0]);
			inventory.erase(inventory.begin());
		}

		break;
	}

}


//void Level1::initControllerListener() {
//	controllerListener = EventListenerController::create();
//
//	controllerListener->onAxisEvent = CC_CALLBACK_3(Level1::cAxisEventCallBack, this);
//	controllerListener->onKeyDown = CC_CALLBACK_3(Level1::cKeyDownCallBack, this);
//	controllerListener->onKeyUp = CC_CALLBACK_3(Level1::cKeyUpCallBack, this);
//
//	_eventDispatcher->addEventListenerWithSceneGraphPriority(controllerListener, this);
//}
//
//void Level1::cKeyDownCallBack(Controller* controller, int keyCode, Event* cEvent) {
//	gamepad.deviceID = controller->getDeviceId();
//	gamepad.keyDown[keyCode] = true;
//}
//
//void Level1::cKeyUpCallBack(Controller* controller, int keyCode, Event* cEvent) {
//	gamepad.deviceID = controller->getDeviceId();
//	gamepad.keyDown[keyCode] = false;
//
//	cocos2d::log("Keycode: %i", keyCode);
//}
//
//void Level1::cAxisEventCallBack(Controller* controller, int keyCode, Event* cEvent) {
//	gamepad.deviceID = controller->getDeviceId();
//}


//void Level1::checkUp() {
//	
//}
//
//void Level1::checkDown() {
//	
//}
//
//void Level1::checkLeft() {
//	
//}
//
//void Level1::checkRight() {
//	
//}

//void Level1::checkStart() {
//	if (manager.getController(0)->isButtonPressed(SednaInput::START)) {
//		this->unscheduleUpdate();
//		
//		if (manager.getController(0)->isButtonReleased(SednaInput::START)) togglePause();
//		else checkStart();
//		
//		this->scheduleUpdate();
//	}
//}
