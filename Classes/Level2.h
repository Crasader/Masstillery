#ifndef __LEVEL_2_H__
#define __LEVEL_2_H__

#include "cocos2d.h"
#include "GameScene.h"
#include "PlayerEntity.h"
#include "BarrierEntity.h"

#define LEVEL2_TIME			20
#define LEVEL2_SKY_TEX		"background/sky2.png"
#define LEVEL2_TERRAIN_TEX	"background/background3.png"
#define LEVEL2_TILED		true

class Level2 : public GameScene
{
public:
	static cocos2d::Scene* createScene();

	void setup();

	virtual bool init();

	CREATE_FUNC(Level2);

private:
	void handleContact(cocos2d::PhysicsContact& contact) override;
	bool allDestroyed() override;

	BarrierEntity festzelt;
	BarrierEntity polizist;
	BarrierEntity moench;

};

#endif // __LEVEL_1_H__

