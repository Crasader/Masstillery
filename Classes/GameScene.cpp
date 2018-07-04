#include "GameScene.h"
#include "StartScene.h"

USING_NS_CC;

Scene* GameScene::createScene() {
	auto scene = GameScene::create();

	return scene;
}

bool GameScene::init()
{
	if (!Scene::initWithPhysics()) return false;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
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

	Vec2 vertices[] = {
		{0,0},
		{0,200},
		{ visibleSize.width, 200 },
		{ visibleSize.width, 0 },
	};

	auto floor = DrawNode::create(5);
	floor->drawLine(Vec2(0, 200), Vec2(visibleSize.width, 200), Color4F::WHITE);
	auto floorPb = PhysicsBody::createPolygon(vertices, 4);
	floorPb->setDynamic(false);
	floor->setPhysicsBody(floorPb);

	this->addChild(floor);

	this->addChild(player.getSprite());
	this->addChild(labelTouchInfo);

	// For debugging
	this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	return true;
}

bool GameScene::onTouchBegan(Touch* touch, Event* event) {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto loc = touch->getLocation();
	if (loc.x < (origin.x + visibleSize.width / 2))
		if (loc.y < (origin.y + visibleSize.height / 2)) player.moveLeft(true);
		else player.moveLeft(true);
	else
		if (loc.y < (origin.y + visibleSize.height / 2)) player.moveRight(true);
		else player.moveRight(true);

		return true;
}

void GameScene::onTouchEnded(Touch* touch, Event* event) {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	auto loc = touch->getLocation();
	if (loc.x < (origin.x + visibleSize.width / 2))
		if (loc.y < (origin.y + visibleSize.height / 2)) player.moveLeft(false);
		else player.moveLeft(false);
	else
		if (loc.y < (origin.y + visibleSize.height / 2)) player.moveRight(false);
		else player.moveRight(false);

}

void GameScene::onKeyPressed(const EventKeyboard::KeyCode keyCode, Event* event) {
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		Director::getInstance()->replaceScene(StartScene::createScene());
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		player.jump();
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