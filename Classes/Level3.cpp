#include "Levels.h"
#include "StartScene.h"
#include "PhysicsCategories.h"


USING_NS_CC;

cocos2d::Scene * Level3::createScene() {
	auto scene = Level3::create();
	scene->getPhysicsWorld()->setGravity(Vec2(0, -500));
	scene->setup();
	return scene;
}

bool Level3::init() {
	return GameScene::init();
}

void Level3::setup() {
	Texture2D::TexParams params;
	params.minFilter = GL_NEAREST;
	params.magFilter = GL_NEAREST;
	params.wrapS = GL_REPEAT;
	params.wrapT = GL_REPEAT;

	Sprite *background = Sprite::create(LEVEL3_SKY_TEX);
	background->setTextureRect(cocos2d::Rect(0, 0, totalSize.width, totalSize.height));

	background->setPosition(Vec2::ZERO);
	background->setAnchorPoint(Vec2::ZERO);

	this->timer = LEVEL3_TIME;

	enemies.push_back(BarrierEntity("Festzelt"));
	enemies.push_back(BarrierEntity("Polizist"));
	enemies.push_back(BarrierEntity("Moench"));

	// set terrain surface key-points
	std::vector<Vec2> keypoints{
	{ totalSize.width / 100 * 0 , totalSize.height / 10 },
	{ totalSize.width / 100 * 10 , totalSize.height / 10 },
	{ (totalSize.width / 100) * 16, totalSize.height / 4 },
	{ (totalSize.width / 100) * 23, totalSize.height / 9 },
	{ (totalSize.width / 100) * 43, totalSize.height / 3 },
	{ (totalSize.width / 100) * 52, totalSize.height / 4 },
	{ (totalSize.width / 100) * 65, totalSize.height / 6 },
	{ (totalSize.width / 100) * 72, totalSize.height / 8 },
	{ (totalSize.width / 100) * 85, totalSize.height / 12 },
	{ (totalSize.width / 100) * 91, totalSize.height / 7 },
	{ totalSize.width, totalSize.height / 13 },
	};

	std::vector<Vec2> v = calculateSurface(keypoints);

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
	sprite->setTextureRect(cocos2d::Rect(0, 0, totalSize.width, totalSize.height));
	sprite->setPosition(Vec2::ZERO);
	sprite->setAnchorPoint(Vec2::ZERO);

	ClippingNode * clipper = ClippingNode::create();
	clipper->setPosition(Vec2::ZERO);
	clipper->setStencil(terrain);
	clipper->setInverted(false);
	clipper->addChild(sprite);

	foreground = Node::create();
	foreground->addChild(terrain);
	foreground->addChild(clipper);
	foreground->addChild(surface);

	paraNode = ParallaxNode::create();
	paraNode->addChild(background, 1, Vec2(0.5f, 0.5f), Vec2::ZERO);
	paraNode->addChild(foreground, 2, Vec2(1.0f, 1.0f), Vec2::ZERO);

	this->addChild(paraNode);
	
	GameScene::setup();

	for (const auto& e : enemies) {
		foreground->addChild(e.sprite);
	}

	player.moveToX(totalSize.width / 100 * 5);
	enemies[0].moveToX(1300);
	enemies[1].moveToX(500);
	enemies[2].moveToX(800);

	startGame();
}
