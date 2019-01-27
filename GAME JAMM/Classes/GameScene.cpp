#include "GameScene.h"
#include "MainMenuScene.h"
#include <iostream>
#include "LevelTwo.h"
#include "AudioEngine.h"
#include "HidingPlace.h"

Scene* Gameplay::sceneHandle = nullptr;

Scene* Gameplay::createScene() {
	sceneHandle = Gameplay::create();
	return sceneHandle;
}

void Gameplay::onEnter() { Scene::onEnter(); }

bool Gameplay::init() {
	if (!Scene::init()) return false;

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

void Gameplay::onExit() { Scene::onExit(); }

void Gameplay::initSprites() {
	cocos2d::experimental::AudioEngine::play2d("mMusic.mp3", true);

	background = Sprite::create("backgrounds/MainMenuBGdark.png");
	background->setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
	background->setAnchorPoint(Vec2(0.5f, 0.5f));

	stupidMicrowave = Sprite::create("backgrounds/microwaving.png");
	stupidMicrowave->setPosition(cocos2d::Vec2(windowSize.x / 2.0f, windowSize.y / 2.0f));
	
	scare = Sprite::create("mom.png");
	scare->setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	promptNextLevel = Label::create("The Microwave Broke. Go Find the Screwdriver in the Garage. Space to continue.", "fonts/Roboto/Roboto-Regular.ttf", 30, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	promptNextLevel->setPosition(cocos2d::Vec2(windowSize.x / 2.0f, windowSize.y / 2.0f));

	cabinet = g3nts::PrimitiveRect(Vec2(35, 210), Vec2(110, 350));
	momBox = g3nts::PrimitiveRect(Vec2(1000, 200), Vec2(1100, 500), Color4F(0, 1, 0, 1));
	
	microBox = HidingPlace(Vec2(390, 400), Vec2(520, 430));
	microBox.getPlaceToHide().getNode()->setVisible(false);
	
	scoreLabel = Label::create("Time:", "fonts/Roboto/Roboto-Regular.ttf", 48, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	scoreLabel->setPosition(cocos2d::Vec2(windowSize.x - scoreLabel->getContentSize().width, windowSize.y - scoreLabel->getContentSize().height));

	scoreLabelInt = Label::create(std::to_string(score), "fonts/Roboto/Roboto-Regular.ttf", 48, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	scoreLabelInt->setPosition(cocos2d::Vec2(windowSize.x - scoreLabel->getContentSize().width, windowSize.y - scoreLabel->getContentSize().height - 50));
	
	hide = Label::create("Hold Space to Hide", "fonts/Roboto/Roboto-Regular.ttf", 30, Size::ZERO);
	hide->setVisible(false);
	hide->enableShadow();
	hide->setPosition(cocos2d::Vec2(windowSize.x / 2.0f, windowSize.y / 2.0f));

	playerHitBox = g3nts::PrimitiveRect(Vec2(200, 100), Vec2(250, 300), Color4F(1, 1, 1, 1));

	itemHitCircle1 = g3nts::PrimitiveCircle(Vec2(50, 200), 5, 5, 40, false, Color4F(1, 0, 1, 1));
	itemHitCircle2 = g3nts::PrimitiveCircle(Vec2(500, 200), 5, 5, 40, false, Color4F(1, 0, 1, 1));

	items.push_back(itemHitCircle1);
	items.push_back(itemHitCircle2);
	

	this->addChild(background, -100);
	this->addChild(stupidMicrowave, -100);
	this->addChild(scare, 1000);
	this->addChild(promptNextLevel, 2);
	
	this->addChild(cabinet.getNode(), 1);
	this->addChild(momBox.getNode(), 2);
	this->addChild(microBox.getPlaceToHide().getNode(), 1);
	
	this->addChild(scoreLabel, 100);
	this->addChild(scoreLabelInt, 100);
	this->addChild(hide);
	
	for (g3nts::PrimitiveCircle item : items) this->addChild(item.getNode());
	this->addChild(playerHitBox.getNode(), 1);

	stupidMicrowave->setVisible(false);
	scare->setVisible(false);
	promptNextLevel->setVisible(false);
	cabinet.getNode()->setVisible(false);
	momBox.getNode()->setVisible(false);
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
		playerStart = playerHitBox.getStartPosition();
		playerEnd = playerHitBox.getEndPosition();

		playerHitBox.getNode()->setVisible(false);
		HUD->setVisible(false);
		for (g3nts::PrimitiveCircle item : items) item.getNode()->setVisible(false);
		for (g3nts::PrimitiveCircle item : inventory) item.getNode()->setVisible(false);

		pauseMenu->setVisible(true);
	}
	else {
		pauseMenu->setVisible(false);
		
		playerHitBox.setNewPositions(playerStart, playerEnd);
		playerHitBox.redraw();

		HUD->setVisible(true);
		playerHitBox.getNode()->setVisible(true);
		for (g3nts::PrimitiveCircle item : items) item.getNode()->setVisible(true);
		for (g3nts::PrimitiveCircle item : inventory) item.getNode()->setVisible(true);
	}
}

void Gameplay::quitToMainMenu() {
	Scene* menuScene = MainMenu::createScene();
	director->replaceScene(TransitionFade::create(2.0f, menuScene));
}

void Gameplay::initHUD() {
	MenuItemImage* inventoryItem = MenuItemImage::create("square.png", "square.png");
	inventoryItem->setPosition(windowSize.x / 2.0f - inventoryItem->getContentSize().width, -windowSize.y / 2.0f + inventoryItem->getContentSize().height);

	HUD = Menu::create(inventoryItem, NULL);
	this->addChild(HUD, 99);
	HUD->setVisible(true);
}

void Gameplay::update(float dt) {

	//manager.update();
	if (score == 1000) {
		promptNextLevel->setVisible(true);

		if (keyboard.keyDown[(int)EventKeyboard::KeyCode::KEY_SPACE]) {
			Scene* levelTwo = LevelTwo::createScene();
			director->replaceScene(TransitionFade::create(2.0f, levelTwo));
		}
	}
	else {

		if (!paused) {
			momTimer++;

			for (int i = 0; i < items.size(); i++) {
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

			//checkUp();
			//checkDown();
			//checkLeft();
			//checkRight();
			//

			//checkStart();

		//	if(playerHitCircle.getPosition().momTimer > 110 && playerHitCircle.getPosition().momTimer < 200)

			if (playerHitBox.getCentrePosition().x >= microBox.getPlaceToHide().getStartPosition().x &&
				playerHitBox.getCentrePosition().x <= microBox.getPlaceToHide().getEndPosition().x &&
				playerHitBox.getCentrePosition().y >= 150) {

				microwaving = true;
				background->setVisible(false);
				stupidMicrowave->setVisible(true);
				scoreLabelInt->setString(std::to_string(score));

			}
			else {
				microwaving = false;
				background->setVisible(true);
				stupidMicrowave->setVisible(false);

			}
			if (microwaving)
				score++;

			if (playerHitBox.getCentrePosition().x >= cabinet.getStartPosition().x &&
				playerHitBox.getCentrePosition().x <= cabinet.getEndPosition().x &&
				playerHitBox.getCentrePosition().y >= 100 &&
				!keyboard.keyDown[(int)EventKeyboard::KeyCode::KEY_SPACE]) {

				hide->setVisible(true);
			}
			else
				hide->setVisible(false);

			if (playerHitBox.getCentrePosition().x > 35 && playerHitBox.getCentrePosition().x < 110 && keyboard.keyDown[(int)EventKeyboard::KeyCode::KEY_SPACE]) {

				isHiding = true;
				playerHitBox.getNode()->setVisible(false);
			}
			else {
				isHiding = false;
				playerHitBox.getNode()->setVisible(true);

			}
			if (momTimer == 700) {
				cocos2d::experimental::AudioEngine::stop(1);
				cocos2d::experimental::AudioEngine::play2d("superMomMusic.mp3");

			}
			if (momTimer == 1000 && !isHiding) {
				cocos2d::experimental::AudioEngine::play2d("superMomMusic.mp3");
				cocos2d::experimental::AudioEngine::play2d("ree.mp3");
				scare->setVisible(true);
				die = true;
			}
			else if (momTimer == 1000 && isHiding) {

				momBox.getNode()->setVisible(true);
				cocos2d::experimental::AudioEngine::play2d("mom_music.mp3");

				//play different audio file

			}
			if (momTimer == 1000 && !isHiding) {
				cocos2d::experimental::AudioEngine::stopAll();

				cocos2d::experimental::AudioEngine::play2d("ree.mp3");
				scare->setVisible(true);
				die = true;
			}
			if (momTimer == 1500 && die || momTimer == 2000 && die)
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

	}
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

//
//void Gameplay::checkUp() {
//	if (/*manager.getController(0)->isButtonPressed(SednaInput::DPAD_UP)
//		|| */keyboard.keyDown[(int)EventKeyboard::KeyCode::KEY_W]) {
//		playerHitCircle.setPosition(playerHitCircle.getPosition() + cocos2d::Vec2(0, 5));
//		playerHitCircle.redraw();
//	}
//}
//
//void Gameplay::checkDown() {
//	if (/*manager.getController(0)->isButtonPressed(SednaInput::DPAD_DOWN)
//		||*/ keyboard.keyDown[(int)EventKeyboard::KeyCode::KEY_S]) {
//		playerHitCircle.setPosition(playerHitCircle.getPosition() + cocos2d::Vec2(0, -5));
//		playerHitCircle.redraw();
//	}
//}
//
//void Gameplay::checkLeft() {
//	if (/*manager.getController(0)->isButtonPressed(SednaInput::DPAD_LEFT)
//		||*/ keyboard.keyDown[(int)EventKeyboard::KeyCode::KEY_A]) {
//		playerHitCircle.setPosition(playerHitCircle.getPosition() + cocos2d::Vec2(-5, 0));
//		playerHitCircle.redraw();
//	}
//}
//
//void Gameplay::checkRight() {
//	if (/*manager.getController(0)->isButtonPressed(SednaInput::DPAD_RIGHT)
//		||*/ keyboard.keyDown[(int)EventKeyboard::KeyCode::KEY_D]) {
//		playerHitCircle.setPosition(playerHitCircle.getPosition() + cocos2d::Vec2(5, 0));
//		playerHitCircle.redraw();
//	}
//}





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
