#include "GameScene.h"
#include "StartScene.h"
#include "PhysicsCategories.h"


USING_NS_CC;

Scene* GameScene::createScene() {
	auto scene = GameScene::create();
	scene->getPhysicsWorld()->setGravity(Vec2(0, -500));
	return scene;
}

bool GameScene::init()
{
	if (!Scene::initWithPhysics()) return false;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();



	//Texture2D *texture = Director::getInstance()->getTextureCache()->addImage("background/sky.png");
	//auto textureSprite = Sprite::createWithTexture(texture, cocos2d::Rect(0, 0, visibleSize.width, visibleSize.height), false);

	Sprite *textureSprite = Sprite::create("background/sky.png");
	Texture2D::TexParams params;
	params.minFilter = GL_NEAREST;
	params.magFilter = GL_NEAREST;
	params.wrapS = GL_REPEAT;
	params.wrapT = GL_REPEAT;
	textureSprite->getTexture()->setTexParameters(params);
	textureSprite->setTextureRect(cocos2d::Rect(0, 0, visibleSize.width, visibleSize.height));

	textureSprite->setPosition(Vec2::ZERO);
	textureSprite->setAnchorPoint(Vec2::ZERO);

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

	labelTouchInfo = Label::createWithTTF("", "fonts/Marker Felt.ttf", 64);
	labelTouchInfo->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

	player = PlayerEntity();
	player.init("entities/Mass.png");

	festzelt = BarrierEntity();
	festzelt.init("entities/Festzelt.png");

	polizist = BarrierEntity();
	polizist.init("entities/Polizist.png");

	moench = BarrierEntity();
	moench.init("entities/Moench.png");

	// set terrain surface key-points
	Vec2 vector[] = {
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
		{ visibleSize.width, visibleSize.height / 13 },
	};

#define KEY_POINTS 11
#define SEGMENTS 10
	std::vector<Vec2> v{};

	// calculate surface points
	for (int i = 0; i < KEY_POINTS - 1; i++) {
		auto p0 = vector[i];
		auto p1 = vector[i + 1];

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

	// node to draw onto
	auto terrain = DrawNode::create();

	for (int i = 0; i < v.size() - 1; i++) {
		auto p0 = v[i];
		auto p1 = v[i + 1];
		auto y0 = Vec2(p0);
		y0.y = 0;
		auto y1 = Vec2(p1);
		y1.y = 0;

		terrain->drawTriangle(p0, y1, y0, Color4F::GREEN);
		terrain->drawTriangle(p0, p1, y1, Color4F::GREEN);
	}

	// physics body for collision
	auto terrainPb = PhysicsBody::createEdgeChain(&v[0], v.size());
	terrainPb->setDynamic(false);
	terrainPb->setCategoryBitmask(TERRAIN_TAG);
	terrainPb->setContactTestBitmask(SHOT_TAG | BARRIER_TAG);
	terrain->setPhysicsBody(terrainPb);
	terrain->setTag(TERRAIN_TAG);

	auto sprite = Sprite::create("background/background1.png");
	sprite->getTexture()->setTexParameters(params);
	sprite->setTextureRect(cocos2d::Rect(0, 0, visibleSize.width, visibleSize.height));
	sprite->setPosition(Vec2::ZERO);
	sprite->setAnchorPoint(Vec2::ZERO);

	ClippingNode * clipper = ClippingNode::create();
	clipper->setPosition(Vec2::ZERO);
	clipper->setStencil(terrain);
	clipper->setInverted(false);
	clipper->addChild(sprite);

	// add all nodes
	this->addChild(textureSprite);
	this->addChild(terrain);
	this->addChild(clipper);
	this->addChild(labelTouchInfo);
	this->addChild(festzelt.sprite);
	this->addChild(polizist.sprite);
	this->addChild(moench.sprite);
	this->addChild(player.sprite);

	// For debugging
	this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	player.moveToX(300);
	festzelt.moveToX(1300);
	polizist.moveToX(500);
	moench.moveToX(800);

	return true;
}

bool GameScene::onTouchBegan(Touch* touch, Event* event) {
	onTouchHandle(touch, true);
	return true;
}

void GameScene::onTouchEnded(Touch* touch, Event* event) {
	onTouchHandle(touch, false);
}

void GameScene::onTouchHandle(Touch* touch, bool started) {
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
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		Director::getInstance()->replaceScene(StartScene::createScene()); break;
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

	if (nodeA && nodeB) {
		if (nodeA->getTag() == SHOT_TAG) nodeA->removeFromParentAndCleanup(true);
		if (nodeB->getTag() == SHOT_TAG) nodeB->removeFromParentAndCleanup(true);
	}
	return true;
}
