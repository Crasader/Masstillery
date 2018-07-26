#include "Level3.h"
#include "StartScene.h"
#include "PhysicsCategories.h"


USING_NS_CC;

cocos2d::Scene * Level3::createScene() {
	auto scene = Level3::create();
	scene->setup();
	return scene;
}

bool Level3::init() {
	return GameScene::init();
}

void Level3::setup() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Texture2D::TexParams params;
	params.minFilter = GL_NEAREST;
	params.magFilter = GL_NEAREST;
	params.wrapS = GL_REPEAT;
	params.wrapT = GL_REPEAT;

	Sprite *background = Sprite::create(LEVEL3_SKY_TEX);
	if (LEVEL3_TILED)
		background->getTexture()->setTexParameters(params);
	background->setTextureRect(cocos2d::Rect(0, 0, visibleSize.width, visibleSize.height));

	background->setPosition(Vec2::ZERO);
	background->setAnchorPoint(Vec2::ZERO);

	this->timer = LEVEL3_TIME;

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
	terrainPb->setContactTestBitmask(SHOT_TAG | BARRIER_TAG);
	terrain->setPhysicsBody(terrainPb);
	terrain->setTag(TERRAIN_TAG);

	auto sprite = Sprite::create(LEVEL3_TERRAIN_TEX);
	sprite->getTexture()->setTexParameters(params);
	sprite->setTextureRect(cocos2d::Rect(0, 0, visibleSize.width, visibleSize.height));
	sprite->setPosition(Vec2::ZERO);
	sprite->setAnchorPoint(Vec2::ZERO);

	ClippingNode * clipper = ClippingNode::create();
	clipper->setPosition(Vec2::ZERO);
	clipper->setStencil(terrain);
	clipper->setInverted(false);
	clipper->addChild(sprite);

	// add nodes
	this->addChild(background);
	this->addChild(terrain);
	this->addChild(clipper);
	this->addChild(surface);
	
	GameScene::setup();

	this->addChild(festzelt.sprite);
	this->addChild(polizist.sprite);
	this->addChild(moench.sprite);

	// For debugging
	//this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	player.moveToX(300);
	festzelt.moveToX(1300);
	polizist.moveToX(500);
	moench.moveToX(800);
}

void Level3::handleContact(cocos2d::PhysicsContact& contact) {
	festzelt.handleContact(contact);
	polizist.handleContact(contact);
	moench.handleContact(contact);
}

bool Level3::allDestroyed() {
	bool allDestroyed = true;

	allDestroyed &= festzelt.isDestroyed();
	allDestroyed &= polizist.isDestroyed();
	allDestroyed &= moench.isDestroyed();

	return allDestroyed;
}
