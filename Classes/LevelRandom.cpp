#include "Levels.h"
#include "StartScene.h"
#include "PhysicsCategories.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;

cocos2d::Scene * LevelRandom::createScene(bool musicOn) {
	auto scene = LevelRandom::create();
	scene->getPhysicsWorld()->setGravity(Vec2(0, -500));
	scene->setup(musicOn);
	return scene;
}

bool LevelRandom::init() {

	return GameScene::init();
}

void LevelRandom::setup(bool musicOn) {
	this->musicOn = musicOn;

	std::string entities[3] = { "Polizist","Moench", "Breze" };
	std::string skies[3] = { LEVEL1_SKY_TEX, LEVEL2_SKY_TEX, LEVEL3_SKY_TEX };
	std::string terrains[3] = { LEVEL1_TERRAIN_TEX, LEVEL2_TERRAIN_TEX, LEVEL3_TERRAIN_TEX };
	std::string musics[3] = { LEVEL1_BG_MUSIC, LEVEL2_BG_MUSIC, LEVEL3_BG_MUSIC };

	int random = CCRANDOM_0_1() * 3;
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(musics[random].c_str(), true);

	if (!musicOn)
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Texture2D::TexParams params;
	params.minFilter = GL_NEAREST;
	params.magFilter = GL_NEAREST;
	params.wrapS = GL_REPEAT;
	params.wrapT = GL_REPEAT;

	random = CCRANDOM_0_1() * 3;
	std::string sky = skies[random];
	Sprite *background = Sprite::create(sky);
	if (sky != LEVEL3_SKY_TEX)
		background->getTexture()->setTexParameters(params);
	background->setTextureRect(cocos2d::Rect(0, 0, totalSize.width, totalSize.height));

	background->setPosition(Vec2::ZERO);
	background->setAnchorPoint(Vec2::ZERO);

	this->timer = CCRANDOM_0_1() * 20 + 40;

	for (int i = 0; i < 5; i++) {
		random = CCRANDOM_0_1() * 3;
		enemies.push_back(BarrierEntity(entities[random], CCRANDOM_0_1() * 500 + 500));
	}

	// set terrain surface key-points
	std::vector<Vec2> keypoints{
		{ totalSize.width / 100 * 0 , totalSize.height / 10},
		{ totalSize.width / 100 * 10 , totalSize.height / 10}
	};

	for (int i = 20; i <= 100; i += 5) {
		keypoints.push_back(Vec2(totalSize.width / 100 * i, totalSize.height / (CCRANDOM_0_1() * 10 + 2)));
	}

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
	for (int i = 0; i < enemies.size(); i++) {
		int area = (totalSize.width - 300) / enemies.size() - 20;
		int posX = CCRANDOM_0_1() * area + area * (i + 1);
		enemies[i].moveToX(posX);
	}

	startGame();
}
