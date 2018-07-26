#ifndef __LEVEL_1_H__
#define __LEVEL_1_H__

#include "cocos2d.h"
#include "GameScene.h"
#include "PlayerEntity.h"
#include "BarrierEntity.h"

#define LEVEL1_TIME			30
#define LEVEL1_SKY_TEX		"background/sky.png"
#define LEVEL1_TERRAIN_TEX	"background/background1.png"
#define LEVEL1_TILED		true

class Level1 : public GameScene
{
public:
	static cocos2d::Scene* createScene();

	void setup();

	virtual bool init();

	CREATE_FUNC(Level1);

private:
	void handleContact(cocos2d::PhysicsContact& contact) override;
	bool allDestroyed() override;

	BarrierEntity festzelt;
	BarrierEntity polizist;
	BarrierEntity moench;

};

#endif // __LEVEL_1_H__

