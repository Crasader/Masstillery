#include "Levels.h"
#include "StartScene.h"
#include "PhysicsCategories.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;

cocos2d::Scene * Level1::createScene(bool musicOn) {
	auto scene = Level1::create();
	scene->getPhysicsWorld()->setGravity(Vec2(0, -500));
	scene->setup(musicOn);
	return scene;
}

bool Level1::init() {
	return GameScene::init();
}

void Level1::setup(bool musicOn) {
	if (musicOn)
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(LEVEL1_BG_MUSIC, true);

	Texture2D::TexParams params;
	params.minFilter = GL_NEAREST;
	params.magFilter = GL_NEAREST;
	params.wrapS = GL_REPEAT;
	params.wrapT = GL_REPEAT;

	Sprite *background = Sprite::create(LEVEL1_SKY_TEX);
	background->getTexture()->setTexParameters(params);
	background->setTextureRect(cocos2d::Rect(0, 0, totalSize.width, totalSize.height));

	background->setPosition(Vec2::ZERO);
	background->setAnchorPoint(Vec2::ZERO);

	enemies.push_back(BarrierEntity::createMoench());
	enemies.push_back(BarrierEntity::createMoench());
	enemies.push_back(BarrierEntity::createPolizist());
	enemies.push_back(BarrierEntity::createPolizist());
	enemies.push_back(BarrierEntity::createFestzelt());
	enemies.push_back(BarrierEntity::createFestzelt());

	this->timer = enemies.size() * 10;

	// set terrain surface key-points
	std::vector<Vec2> keypoints{
	{ 0 ,					  totalSize.height * 0.1f },
	{ totalSize.width * 0.1f, totalSize.height * 0.1f },
	{ totalSize.width * 0.2f, totalSize.height * 0.2f },
	{ totalSize.width * 0.3f, totalSize.height * 0.5f },
	{ totalSize.width * 0.4f, totalSize.height * 0.3f },
	{ totalSize.width * 0.5f, totalSize.height * 0.6f },
	{ totalSize.width * 0.6f, totalSize.height * 0.4f },
	{ totalSize.width * 0.7f, totalSize.height * 0.2f },
	{ totalSize.width * 0.8f, totalSize.height * 0.2f },
	{ totalSize.width * 0.9f, totalSize.height * 0.2f },
	{ totalSize.width,		  totalSize.height * 0.2f },
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

	auto sprite = Sprite::create(LEVEL1_TERRAIN_TEX);
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

	GameScene::setup(musicOn);

	for (const auto& e : enemies) {
		foreground->addChild(e.sprite);
	}

	player.moveToX(totalSize.width / 100 * 5);
	enemies[0].moveToX(totalSize.width * 0.2f);
	enemies[1].moveToX(totalSize.width * 0.4f);
	enemies[2].moveToX(totalSize.width * 0.7f);
	enemies[3].moveToX(totalSize.width * 0.75f);
	enemies[4].moveToX(totalSize.width * 0.85f);
	enemies[5].moveToX(totalSize.width * 0.95f);

	startGame();
}