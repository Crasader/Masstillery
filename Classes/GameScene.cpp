#include "GameScene.h"
#include "StartScene.h"
#include "PhysicsCategories.h"


USING_NS_CC;

void GameScene::setup() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	this->labelTimeInfo = Label::createWithTTF(std::string("Time: ") + std::string(std::to_string(timer)), "fonts/Marker Felt.ttf", 48);
	this->labelTimeInfo->setPosition(Vec2(labelTimeInfo->getContentSize().width, visibleSize.height - 32));
	this->schedule(schedule_selector(GameScene::count), 1.0);

	player = PlayerEntity();
	player.init("Mass");

	auto body = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	auto edgeNode = Node::create();
	edgeNode->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	edgeNode->setPhysicsBody(body);

	this->addChild(edgeNode);
	this->addChild(labelTimeInfo);
	this->addChild(player.sprite);
	isGameRunning = true;
}

bool GameScene::init() {
	if (!Scene::initWithPhysics()) return false;

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	return true;
}

void GameScene::count(float dt) {
	timer--;
	labelTimeInfo->setString(std::string("Time: ") + std::string(std::to_string(timer)));
	checkFinish();
}

bool GameScene::onTouchBegan(Touch* touch, Event* event) {
	onTouchHandle(touch, true);
	return true;
}

void GameScene::onTouchEnded(Touch* touch, Event* event) {
	onTouchHandle(touch, false);
}

void GameScene::onTouchHandle(Touch* touch, bool started) {
	if (!isGameRunning) return;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto loc = touch->getLocation();
	int x = 0;
	int y = 0;

	if (loc.x < (origin.x + visibleSize.width * 0.33))		x = 1;
	else if (loc.x < (origin.x + visibleSize.width * 0.66)) x = 2;
	if (loc.y < (origin.y + visibleSize.height * 0.33))		 y = 1;
	else if (loc.y < (origin.y + visibleSize.height * 0.66)) y = 2;

	if (x == 1) {
		if (y == 1)			player.moveLeft(started);
		else if (y == 2)	player.decreaseAccel(started);
		else				player.moveShootLeft(started);
	}
	else if (x == 2) {
		if (y == 1) {}
		else if (y == 2)	if (started) player.shoot();
		else {}
	}
	else {
		if (y == 1) 		player.moveRight(started);
		else if (y == 2) 	player.increaseAccel(started);
		else 				player.moveShootRight(started);
	}
}

void GameScene::onKeyPressed(const EventKeyboard::KeyCode keyCode, Event* event) {
	if (!isGameRunning) return;

	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_ENTER:
		player.shoot();	break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		player.moveShootLeft(true); break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		player.moveShootRight(true); break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_A:
		player.moveLeft(true); break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_D:
		player.moveRight(true); break;
	case EventKeyboard::KeyCode(121): // Ger KB: plus
	case EventKeyboard::KeyCode::KEY_KP_PLUS:
		player.increaseAccel(true); break;
	case EventKeyboard::KeyCode(75): // Ger KB: minus
	case EventKeyboard::KeyCode::KEY_KP_MINUS:
		player.decreaseAccel(true); break;
	}
}

void GameScene::onKeyReleased(const EventKeyboard::KeyCode keyCode, Event* event) {
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		Director::getInstance()->replaceScene(StartScene::createScene()); break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		player.moveShootLeft(false); break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		player.moveShootRight(false); break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_A:
		player.moveLeft(false);	break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_D:
		player.moveRight(false); break;
	case EventKeyboard::KeyCode(121): // Ger KB: plus
	case EventKeyboard::KeyCode::KEY_KP_PLUS:
		player.increaseAccel(false); break;
	case EventKeyboard::KeyCode(75): // Ger KB: minus
	case EventKeyboard::KeyCode::KEY_KP_MINUS:
		player.decreaseAccel(false); break;
	}
}

bool GameScene::onContactBegin(cocos2d::PhysicsContact& contact) {
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	if (!nodeA || !nodeB) return false;

	if (nodeA->getTag() == SHOT_TAG) nodeA->removeFromParentAndCleanup(true);
	if (nodeB->getTag() == SHOT_TAG) nodeB->removeFromParentAndCleanup(true);

	for (auto& e : enemies) {
		e.handleContact(contact);
	}

	checkFinish();

	return true;
}

bool GameScene::allDestroyed() {
	bool allDestroyed = true;

	for (const auto& e : enemies) {
		allDestroyed &= e.isDestroyed();
	}

	return allDestroyed;
}

void GameScene::checkFinish() {
	if (!isGameRunning) return;

	if (timer <= 0) endGame(false);

	if (allDestroyed()) endGame(true);
}

void GameScene::endGame(bool victory) {
	this->cleanup();
	isGameRunning = false;
	auto visibleSize = Director::getInstance()->getVisibleSize();

	std::string strVictory{};

	if (victory) strVictory = "Victory!";
	else strVictory = "Defeated!";

	auto labelVictory = Label::createWithTTF(strVictory, "fonts/Marker Felt.ttf", 128);
	labelVictory->setPosition(visibleSize / 2);

	this->addChild(labelVictory);

}
