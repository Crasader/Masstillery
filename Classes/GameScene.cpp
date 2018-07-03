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
	touchListener->onTouchesBegan = CC_CALLBACK_2(GameScene::onTouchesBegan, this);
	touchListener->onTouchesEnded = CC_CALLBACK_2(GameScene::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	labelTouchInfo = Label::createWithTTF("", "fonts/Marker Felt.ttf", 64);
	labelTouchInfo->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

	player = PlayerEntity();
	player.init();
	player.getSprite()->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

	this->addChild(player.getSprite());
	this->addChild(labelTouchInfo);

	return true;
}

void GameScene::onTouchesBegan(const std::vector<Touch*>& touches, Event* event) {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	for (const auto touch : touches) {
		auto loc = touch->getLocation();
		if (loc.x < (origin.x + visibleSize.width / 2))
			if (loc.y < (origin.y + visibleSize.height / 2)) player.moveLeft(true);
			else player.moveLeft(true);
		else
			if (loc.y < (origin.y + visibleSize.height / 2)) player.moveRight(true);
			else player.moveRight(true);
	}
}

void GameScene::onTouchesEnded(const std::vector<Touch*>& touches, Event* event) {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	for (const auto touch : touches) {
		auto loc = touch->getLocation();
		if (loc.x < (origin.x + visibleSize.width / 2))
			if (loc.y < (origin.y + visibleSize.height / 2)) player.moveLeft(false);
			else player.moveLeft(false);
		else
			if (loc.y < (origin.y + visibleSize.height / 2)) player.moveRight(false);
			else player.moveRight(false);
	}
}

void GameScene::onKeyPressed(const EventKeyboard::KeyCode keyCode, Event* event) {
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		Director::getInstance()->replaceScene(StartScene::createScene());
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_A:
		player.moveLeft(true);
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_D:
		player.moveRight(true);
		break;
	}
}

void GameScene::onKeyReleased(const EventKeyboard::KeyCode keyCode, Event* event) {
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		Director::getInstance()->replaceScene(StartScene::createScene());
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_A:
		player.moveLeft(false);
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_D:
		player.moveRight(false);
		break;
	}
}