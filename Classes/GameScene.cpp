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

	Vec2 vectors[] = {
		{ 0,0 },
		{ 0 , visibleSize.height / 5 },
		{ (visibleSize.width / 100) * 12, visibleSize.height / 3 },
		{ (visibleSize.width / 100) * 16, visibleSize.height / 4 },
		{ (visibleSize.width / 100) * 23, visibleSize.height / 9 },
		{ (visibleSize.width / 100) * 43, visibleSize.height / 3 },
		{ (visibleSize.width / 100) * 52, visibleSize.height / 4 },
		{ (visibleSize.width / 100) * 65, visibleSize.height / 6 },
		{ (visibleSize.width / 100) * 72, visibleSize.height / 8 },
		{ (visibleSize.width / 100) * 85, visibleSize.height / 12 },
		{ (visibleSize.width / 100) * 91, visibleSize.height / 7 },
		{ visibleSize.width ,visibleSize.height / 13 },
		{ visibleSize.width,0 },
		{ 0,0 }
	};

	auto terrain = DrawNode::create();
	terrain->drawSolidPoly(vectors, 13, Color4F::GREEN);
	

	auto terrainPb = PhysicsBody::createPolygon(vectors, 13);
	terrainPb->setDynamic(false);
	terrain->setPhysicsBody(terrainPb);

	this->addChild(player.getSprite());
	this->addChild(labelTouchInfo);
	this->addChild(terrain);
	
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