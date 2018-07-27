#ifndef __LEVELS_H__
#define __LEVELS_H__

#include "cocos2d.h"
#include "GameScene.h"
#include "PlayerEntity.h"
#include "BarrierEntity.h"

#define LEVEL1_TIME			30
#define LEVEL1_SKY_TEX		"background/sky.png"
#define LEVEL1_TERRAIN_TEX	"background/background1.png"

#define LEVEL2_TIME			20
#define LEVEL2_SKY_TEX		"background/sky2.png"
#define LEVEL2_TERRAIN_TEX	"background/background3.png"

#define LEVEL3_TIME			30
#define LEVEL3_SKY_TEX		"background/sky4.png"
#define LEVEL3_TERRAIN_TEX	"background/background6.png"

class Level1 : public GameScene
{
public:
	static cocos2d::Scene* createScene();

	void setup();

	virtual bool init();

	CREATE_FUNC(Level1);

};

class Level2 : public GameScene
{
public:
	static cocos2d::Scene* createScene();

	void setup();

	virtual bool init();

	CREATE_FUNC(Level2);
};

class Level3 : public GameScene
{
public:
	static cocos2d::Scene* createScene();

	void setup();

	virtual bool init();

	CREATE_FUNC(Level3);
};

class LevelRandom : public GameScene
{
public:
	static cocos2d::Scene* createScene();

	void setup();

	virtual bool init();

	CREATE_FUNC(LevelRandom);

};

#endif // __LEVELS_H__

