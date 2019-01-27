#include "GameScene.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

cocos2d::Scene* MainMenu::createScene() { return MainMenu::create(); }

//////////// ENTER SCENE ////////////////////////////////////////////
void MainMenu::onEnter() { Scene::onEnter(); }

//////////// INIT MENU //////////////////////////////////////////////
bool MainMenu::init() {
	
	if (!Scene::init()) return false;

	director = Director::getInstance();

	initMenu();
	initMouseListener();
	initKeyboardListener();

	auto windowSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	

	return true;
}

void MainMenu::initMenu() {
	Vec2 windowSize = director->getWinSizeInPixels();

	background = Sprite::create("backgrounds/MainMenuBGdark.png");
	background->setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
	background->setAnchorPoint(Vec2(0.5f, 0.5f));

	title = Label::create("3AM Cooking Sim!", "fonts/Roboto/Roboto-Regular.ttf", 72);
	title->setAnchorPoint(Vec2(0.5f, 0.5f));
	title->setPosition(Vec2(windowSize.x / 2.0f, windowSize.y - title->getContentSize().height - 200));
	title->enableShadow();

	auto playLabel = Label::create("Start Game", "fonts/Roboto/Roboto-Regular.ttf", 24);
	auto exitLabel = Label::create("Exit Game", "fonts/Roboto/Roboto-Regular.ttf", 24);
	exitLabel->enableShadow();
	playLabel->enableShadow();

	auto playButton = MenuItemLabel::create(playLabel, [&](Ref* sender) {
		//auto helloWorldScene = HelloWorld::createScene();
		auto gameScene = Gameplay::createScene();
		director->replaceScene(TransitionFade::create(2.0f, gameScene));
	});
	auto exitButton = MenuItemLabel::create(exitLabel, [&](Ref* sender) {
		director->end();
	});

	playButton->setPosition(0, -200);
	exitButton->setPosition(0, -250);

	menu = Menu::create(playButton, exitButton, NULL);

	this->addChild(background, -100);
	this->addChild(menu, 99);
	this->addChild(title, 100);
}

//////////// EXIT SCENE ////////////////////////////////////////////
void MainMenu::onExit() { Scene::onExit(); }


//////////// MOUSE LISTENER ////////////////////////////////////////
void MainMenu::initMouseListener() {
	mouseListener = EventListenerMouse::create();

	// Mouse down
	mouseListener->onMouseDown = [&](Event* mEvent) {
		EventMouse* mouseEvent = dynamic_cast<EventMouse*>(mEvent);

		auto mouseClickPos = mouseEvent->getLocationInView();
		mouseClickPos.y += 540;

		auto mouseButton = mouseEvent->getMouseButton();
	};

	// Mouse up
	mouseListener->onMouseUp = [&](Event* mEvent) {
		EventMouse* mouseEvent = dynamic_cast<EventMouse*>(mEvent);

		auto mouseUpPos = mouseEvent->getLocationInView();
		mouseUpPos.y += 540;
	};

	// Mouse move
	mouseListener->onMouseMove = [&](Event* mEvent) {
		EventMouse* mouseEvent = dynamic_cast<EventMouse*>(mEvent);

		auto mouseEventPos = mouseEvent->getLocationInView();
		mouseEventPos.y += 540;

		mousePosition = Vec2(mouseEventPos.x, mouseEventPos.y + 540);
	};

	// Mouse scroll
	mouseListener->onMouseScroll = [&](Event* mEvent) {};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

//////////// KEYBOARD LISTENER ////////////////////////////////////////
void MainMenu::initKeyboardListener() {
	keyboardListener = EventListenerKeyboard::create();

	keyboardListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* kEvent) {};
	keyboardListener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* kEvent) {};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}
