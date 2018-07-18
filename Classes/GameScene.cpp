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



	//Texture2D *texture = Director::getInstance()->getTextureCache()->addImage("background/sky.png");
	//auto textureSprite = Sprite::createWithTexture(texture, cocos2d::Rect(0, 0, visibleSize.width, visibleSize.height), false);

	Sprite *textureSprite = Sprite::create("background/sky4.png");
	Texture2D::TexParams params;
	params.minFilter = GL_NEAREST;
	params.magFilter = GL_NEAREST;
	params.wrapS = GL_REPEAT;
	params.wrapT = GL_REPEAT;
	//textureSprite->getTexture()->setTexParameters(params);
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

	labelTouchInfo = Label::createWithTTF("", "fonts/Marker Felt.ttf", 64);
	labelTouchInfo->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

	player = PlayerEntity();
	player.init();
	player.getSprite()->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

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
	terrainPb->setContactTestBitmask(0xFFFFFFFF);
	terrain->setPhysicsBody(terrainPb);

	auto sprite = Sprite::create("background/background3.png");
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
	this->addChild(surface);
	this->addChild(labelTouchInfo);
	this->addChild(player.getSprite());

	// For debugging
	//this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

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