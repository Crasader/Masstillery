#include "GameScene.h"
#include "StartScene.h"
#include "PhysicsCategories.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;

void GameScene::setup(bool musicOn) {
	this->musicOn = musicOn;

	auto visibleSize = Director::getInstance()->getVisibleSize();

	this->labelTimeInfo = Label::createWithTTF(std::string("Time: ") + std::string(std::to_string(timer)), "fonts/Marker Felt.ttf", 48);
	this->labelTimeInfo->setPosition(Vec2(labelTimeInfo->getContentSize().width, visibleSize.height - 32));
	this->schedule(schedule_selector(GameScene::count), 1.0);

	player = PlayerEntity();
	player.init("Mass");

	auto body = PhysicsBody::createEdgeBox(totalSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	auto edgeNode = Node::create();
	edgeNode->setPosition(totalSize / 2);
	edgeNode->setPhysicsBody(body);

	foreground->addChild(edgeNode);
	this->addChild(labelTimeInfo);
	foreground->addChild(player.sprite);
}

bool GameScene::init() {
	if (!Scene::initWithPhysics()) return false;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	totalSize = Size(visibleSize.width * 1.5, visibleSize.height);

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

	//this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	return true;
}

std::vector<Vec2> GameScene::calculateSurface(std::vector<Vec2>& keypoints) {
#define SEGMENTS 10
	std::vector<Vec2> v{};

	// calculate surface points
	for (int i = 0; i < keypoints.size() - 1; i++) {
		auto p0 = keypoints[i];
		auto p1 = keypoints[i + 1];

		int hSegments = floorf((p1.x - p0.x) / SEGMENTS);
		float dx = (p1.x - p0.x) / hSegments;
		float da = M_PI / hSegments;
		float ymid = (p0.y + p1.y) / 2;
		float ampl = (p0.y - p1.y) / 2;

		Vec2 pt0, pt1;
		pt0 = p0;
		for (int j = 0; j < hSegments + 2; ++j) {

			pt1.x = p0.x + j * dx;
			pt1.y = ymid + ampl * cosf(da*j);

			v.push_back(pt0);

			pt0 = pt1;
		}
	}
	return v;
}

void GameScene::startGame() {
	auto moveBy = totalSize.width - Director::getInstance()->getVisibleSize().width;
	
	paraNode->runAction(Sequence::create(
		DelayTime::create(1),
		MoveBy::create(2, Vec2(-moveBy, 0)),
		DelayTime::create(1),
		MoveBy::create(2, Vec2(moveBy, 0)),
		CallFunc::create([this]() {
			auto labelGo = Label::createWithTTF("GO!", "fonts/Marker Felt.ttf", 128);
			labelGo->setPosition(Director::getInstance()->getVisibleSize() / 2);
			labelGo->setTag(11);
			this->addChild(labelGo);
		}),
		DelayTime::create(1),
		CallFunc::create([this]() {
			this->removeChildByTag(11); 
			isGameRunning = true; 
		}), nullptr));
}

void GameScene::count(float dt) {
	if (!isGameRunning) return;

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

	if (!started)
		paraNode->stopAllActions();

	if (loc.x < (origin.x + visibleSize.width * 0.33))		x = 1;
	else if (loc.x < (origin.x + visibleSize.width * 0.66)) x = 2;
	if (loc.y < (origin.y + visibleSize.height * 0.33))		 y = 1;
	else if (loc.y < (origin.y + visibleSize.height * 0.66)) y = 2;

	if (x == 1) {
		if (y == 1) {
			player.moveLeft(started);
			paraNode->runAction(RepeatForever::create(Spawn::create(CallFunc::create(CC_CALLBACK_0(GameScene::handleMove, this)), nullptr)));
		}
		else if (y == 2)	player.decreaseAccel(started);
		else				player.moveShootLeft(started);
	}
	else if (x == 2) {
		if (y == 1) {}
		else if (y == 2)	if (started) player.shoot();
		else {}
	}
	else {
		if (y == 1) { 
			player.moveRight(started); 
			paraNode->runAction(RepeatForever::create(Spawn::create(CallFunc::create(CC_CALLBACK_0(GameScene::handleMove, this)), nullptr)));
		}
		else if (y == 2) 	player.increaseAccel(started);
		else 				player.moveShootRight(started);
	}
}

void GameScene::onKeyPressed(const EventKeyboard::KeyCode keyCode, Event* event) {
	if (!isGameRunning) return;

	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_ENTER:
		player.shoot(); break;
	case EventKeyboard::KeyCode::KEY_SPACE:
		player.shoot(); break;

	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		player.increaseAccel(true); break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		player.decreaseAccel(true); break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		player.moveLeft(true); break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		player.moveRight(true); break;
	case EventKeyboard::KeyCode::KEY_DELETE:
		player.moveShootLeft(true); break;
	case EventKeyboard::KeyCode::KEY_PG_DOWN:
		player.moveShootRight(true); break;

	case EventKeyboard::KeyCode::KEY_W:
		player.increaseAccel(true); break;
	case EventKeyboard::KeyCode::KEY_S:
		player.decreaseAccel(true); break;
	case EventKeyboard::KeyCode::KEY_A:
		player.moveLeft(true); break;
	case EventKeyboard::KeyCode::KEY_D:
		player.moveRight(true); break;
	case EventKeyboard::KeyCode::KEY_Q:
		player.moveShootLeft(true); break;
	case EventKeyboard::KeyCode::KEY_E:
		player.moveShootRight(true); break;
	}
}

void GameScene::onKeyReleased(const EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) Director::getInstance()->replaceScene(StartScene::createScene(musicOn));
	if (!isGameRunning) return;

	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		player.increaseAccel(false); break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		player.decreaseAccel(false); break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		player.moveLeft(false); break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		player.moveRight(false); break;
	case EventKeyboard::KeyCode::KEY_DELETE:
		player.increaseAccel(false); break;
	case EventKeyboard::KeyCode::KEY_PG_DOWN:
		player.decreaseAccel(false); break;

	case EventKeyboard::KeyCode::KEY_W:
		player.increaseAccel(false); break;
	case EventKeyboard::KeyCode::KEY_S:
		player.decreaseAccel(false); break;
	case EventKeyboard::KeyCode::KEY_A:
		player.moveLeft(false); break;
	case EventKeyboard::KeyCode::KEY_D:
		player.moveRight(false); break;
	case EventKeyboard::KeyCode::KEY_Q:
		player.moveShootLeft(false); break;
	case EventKeyboard::KeyCode::KEY_E:
		player.moveShootRight(false); break;
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

void GameScene::handleMove() {
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto pos = paraNode->convertToWorldSpace(player.sprite->getPosition());

	if (pos.x < visibleSize.width / 2 || pos.x > visibleSize.width / 2)
		paraNode->setPositionX((player.sprite->getPositionX() - visibleSize.width / 2) * -1);

	if (paraNode->getPositionX() > 0)
		paraNode->setPositionX(0);

	if (totalSize.width + paraNode->getPositionX() - visibleSize.width < 0)
		paraNode->setPositionX(visibleSize.width - totalSize.width);
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

	if (victory) {
		strVictory = "Victory!";
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/jeah.mp3");
	}
	else {
		strVictory = "Defeated!";
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/ooh.mp3");
	}

	auto labelVictory = Label::createWithTTF(strVictory, "fonts/Marker Felt.ttf", 128);
	labelVictory->setPosition(visibleSize / 2);

	this->addChild(labelVictory);

}
