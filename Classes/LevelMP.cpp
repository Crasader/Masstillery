#include "Levels.h"
#include "StartScene.h"
#include "PhysicsCategories.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;

cocos2d::Scene * LevelMP::createScene(bool musicOn) {
	auto scene = LevelMP::create();
	scene->getPhysicsWorld()->setGravity(Vec2(0, -500));
	scene->setup(musicOn);
	return scene;
}

bool LevelMP::init() {
	if (!Scene::initWithPhysics()) return false;

	auto totalSize = Director::getInstance()->getVisibleSize();

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(LevelMP::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(LevelMP::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(LevelMP::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	//this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	return true;
}

void LevelMP::setup(bool musicOn) {
	auto totalSize = Director::getInstance()->getVisibleSize();

	std::string entities[4] = { "Festzelt","Polizist","Moench", "Breze" };
	std::string skies[3] = { LEVEL1_SKY_TEX, LEVEL2_SKY_TEX, LEVEL3_SKY_TEX };
	std::string terrains[3] = { LEVEL1_TERRAIN_TEX, LEVEL2_TERRAIN_TEX, LEVEL3_TERRAIN_TEX };
	std::string musics[3] = { LEVEL1_BG_MUSIC, LEVEL2_BG_MUSIC, LEVEL3_BG_MUSIC };

	if (musicOn) {
		int random = CCRANDOM_0_1() * 3;
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(musics[random].c_str(), true);
	}

	Texture2D::TexParams params;
	params.minFilter = GL_NEAREST;
	params.magFilter = GL_NEAREST;
	params.wrapS = GL_REPEAT;
	params.wrapT = GL_REPEAT;

	int random = CCRANDOM_0_1() * 3;

	std::string sky = skies[random];
	Sprite *background = Sprite::create(sky);
	if (sky != LEVEL3_SKY_TEX)
		background->getTexture()->setTexParameters(params);
	background->setTextureRect(cocos2d::Rect(0, 0, totalSize.width, totalSize.height));

	background->setPosition(Vec2::ZERO);
	background->setAnchorPoint(Vec2::ZERO);

	for (int i = 0; i < 5; i++) {
		random = CCRANDOM_0_1() * 4;
		enemies.push_back(BarrierEntity(entities[random]));
	}

	// set terrain surface key-points
	std::vector<Vec2> keypoints{
		{ totalSize.width / 100 * 0 , totalSize.height / 10 },
		{ totalSize.width / 100 * 10 , totalSize.height / 10 }
	};

	for (int i = 20; i <= 80; i += 5) {
		keypoints.push_back({ totalSize.width / 100 * i, totalSize.height / (CCRANDOM_0_1() * 10 + 2) });
	}

	keypoints.push_back({ totalSize.width / 100 * 90 , totalSize.height / 10 });
	keypoints.push_back({ totalSize.width / 100 * 100 , totalSize.height / 10 });

	std::vector<Vec2> v = GameScene::calculateSurface(keypoints);

	// node to draw onto
	auto terrain = DrawNode::create();
	auto surface = DrawNode::create();

	for (int i = 0; i < v.size() - 1; i++) {
		auto p0 = v[i];
		auto p1 = v[i + 1];
		auto y0 = Vec2(p0);
		y0.y = 0;
		auto y1 = Vec2(p1);
		y1.y = 0;

		terrain->drawTriangle(p0, y1, y0, Color4F::GREEN);
		terrain->drawTriangle(p0, p1, y1, Color4F::GREEN);
		surface->drawSegment(p0, p1, 5, Color4F::BLACK);
	}

	// physics body for collision
	auto terrainPb = PhysicsBody::createEdgeChain(&v[0], v.size());
	terrainPb->setDynamic(false);
	terrainPb->setCategoryBitmask(TERRAIN_TAG);
	terrainPb->setContactTestBitmask(SHOT_TAG | SHOT2_TAG | BARRIER_TAG);
	terrain->setPhysicsBody(terrainPb);
	terrain->setTag(TERRAIN_TAG);

	random = CCRANDOM_0_1() * 3;
	auto sprite = Sprite::create(terrains[random]);
	sprite->getTexture()->setTexParameters(params);
	sprite->setTextureRect(cocos2d::Rect(0, 0, totalSize.width, totalSize.height));
	sprite->setPosition(Vec2::ZERO);
	sprite->setAnchorPoint(Vec2::ZERO);

	ClippingNode * clipper = ClippingNode::create();
	clipper->setPosition(Vec2::ZERO);
	clipper->setStencil(terrain);
	clipper->setInverted(false);
	clipper->addChild(sprite);


	this->addChild(background);
	this->addChild(terrain);
	this->addChild(clipper);
	this->addChild(surface);

	player1 = PlayerEntity();
	player1.init("Mass");

	player2 = PlayerEntity();
	player2.init("GoassMass");

	auto body = PhysicsBody::createEdgeBox(totalSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	auto edgeNode = Node::create();
	edgeNode->setPosition(totalSize / 2);
	edgeNode->setPhysicsBody(body);

	this->addChild(edgeNode);
	this->addChild(player1.sprite);
	this->addChild(player2.sprite);

	for (const auto& e : enemies) {
		this->addChild(e.sprite);
	}

	player1.moveToX(totalSize.width / 100 * 5);
	player2.moveToX(totalSize.width / 100 * 95);
	for (int i = 0; i < enemies.size(); i++) {
		int area = (totalSize.width - 300) / enemies.size() - 20;
		int posX = CCRANDOM_0_1() * area + area * (i + 1);
		enemies[i].moveToX(posX);
	}

	this->runAction(Sequence::create(
		CallFunc::create([this]() {
		auto labelGo = Label::createWithTTF("GO!", "fonts/Marker Felt.ttf", 128);
		labelGo->setPosition(Director::getInstance()->getVisibleSize() / 2);
		labelGo->setTag(11);
		this->addChild(labelGo);
	}),
		DelayTime::create(1),
		CallFunc::create([this]() {
		this->removeChildByTag(11);
		this->isGameRunning = true;
	}), nullptr));
}

void LevelMP::onKeyPressed(const EventKeyboard::KeyCode keyCode, Event* event) {
	if (!isGameRunning) return;

	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_ENTER:
		player2.shoot(Color3B(140, 50, 15), SHOT2_TAG, PLAYER_TAG); break;

	case EventKeyboard::KeyCode::KEY_SPACE:
		player1.shoot(Color3B(255, 214, 49), SHOT_TAG, PLAYER2_TAG); break;

	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		player2.increaseAccel(true); break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		player2.decreaseAccel(true); break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		player2.moveLeft(true); break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		player2.moveRight(true); break;
	case EventKeyboard::KeyCode::KEY_DELETE:
		player2.moveShootLeft(true); break;
	case EventKeyboard::KeyCode::KEY_PG_DOWN:
		player2.moveShootRight(true); break;

	case EventKeyboard::KeyCode::KEY_W:
		player1.increaseAccel(true); break;
	case EventKeyboard::KeyCode::KEY_S:
		player1.decreaseAccel(true); break;
	case EventKeyboard::KeyCode::KEY_A:
		player1.moveLeft(true); break;
	case EventKeyboard::KeyCode::KEY_D:
		player1.moveRight(true); break;
	case EventKeyboard::KeyCode::KEY_Q:
		player1.moveShootLeft(true); break;
	case EventKeyboard::KeyCode::KEY_E:
		player1.moveShootRight(true); break;
	}
}

void LevelMP::onKeyReleased(const EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) Director::getInstance()->replaceScene(StartScene::createScene(musicOn));
	if (!isGameRunning) return;

	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		player2.increaseAccel(false); break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		player2.decreaseAccel(false); break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		player2.moveLeft(false); break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		player2.moveRight(false); break;
	case EventKeyboard::KeyCode::KEY_DELETE:
		player2.increaseAccel(false); break;
	case EventKeyboard::KeyCode::KEY_PG_DOWN:
		player2.decreaseAccel(false); break;

	case EventKeyboard::KeyCode::KEY_W:
		player1.increaseAccel(false); break;
	case EventKeyboard::KeyCode::KEY_S:
		player1.decreaseAccel(false); break;
	case EventKeyboard::KeyCode::KEY_A:
		player1.moveLeft(false); break;
	case EventKeyboard::KeyCode::KEY_D:
		player1.moveRight(false); break;
	case EventKeyboard::KeyCode::KEY_Q:
		player1.moveShootLeft(false); break;
	case EventKeyboard::KeyCode::KEY_E:
		player1.moveShootRight(false); break;
	}
}

bool LevelMP::onContactBegin(cocos2d::PhysicsContact& contact) {
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	if (!nodeA || !nodeB) return false;

	if (nodeA->getTag() == SHOT_TAG || nodeA->getTag() == SHOT2_TAG) nodeA->removeFromParentAndCleanup(true);
	if (nodeB->getTag() == SHOT_TAG || nodeB->getTag() == SHOT2_TAG) nodeB->removeFromParentAndCleanup(true);

	for (auto& e : enemies) {
		e.handleContact(contact);
	}

	player1.handleContact(contact);
	player2.handleContact(contact);

	checkFinish();

	return true;
}

void LevelMP::checkFinish() {
	if (!isGameRunning) return;

	if (player1.isDestroyed()) endGame(1);

	if (player2.isDestroyed()) endGame(2);
}

void LevelMP::endGame(int winner) {
	this->cleanup();
	isGameRunning = false;
	auto visibleSize = Director::getInstance()->getVisibleSize();

	std::string strWinner{ "Player " };
	strWinner.append(std::to_string(winner));
	strWinner.append(" Won The Game!");

	auto labelVictory = Label::createWithTTF(strWinner, "fonts/Marker Felt.ttf", 128);
	labelVictory->setPosition(visibleSize / 2);

	this->addChild(labelVictory);

}
