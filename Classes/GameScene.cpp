#include "GameScene.h"
#include "StartScene.h"

USING_NS_CC;

Scene* GameScene::createScene() {
	return GameScene::create();
}

bool GameScene::init()
{
	if (!Scene::init()) return false;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto touchListener = EventListenerTouchAllAtOnce::create();
	touchListener->onTouchesEnded = CC_CALLBACK_2(GameScene::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	return true;
}

void GameScene::onTouchesEnded(const std::vector<Touch*>& touches, Event* event) {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	for (const auto touch : touches) {
		auto loc = touch->getLocation();
		if (loc.x < (origin.x + visibleSize.width / 2))
			if (loc.y < (origin.y + visibleSize.height / 2)) action3();
			else action1();
		else
			if (loc.y < (origin.y + visibleSize.height / 2)) action4();
			else action2();
	}
}

void GameScene::onKeyPressed(const EventKeyboard::KeyCode keyCode, Event* event) {
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		Director::getInstance()->replaceScene(StartScene::createScene());
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_A:
		action1();
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_D:
		action2();
	}
}

void GameScene::action1() {
}

void GameScene::action2() {
}

void GameScene::action3() {
}

void GameScene::action4() {
}
