#ifndef __LEVEL_3_H__
#define __LEVEL_3_H__

#include "cocos2d.h"
#include "GameScene.h"
#include "PlayerEntity.h"
#include "BarrierEntity.h"

#define LEVEL3_TIME			30
#define LEVEL3_SKY_TEX		"background/sky4.png"
#define LEVEL3_TERRAIN_TEX	"background/background6.png"
#define LEVEL3_TILED		false

class Level3 : public GameScene
{
public:
	static cocos2d::Scene* createScene();

	void setup();

	virtual bool init();

	CREATE_FUNC(Level3);

private:
	void handleContact(cocos2d::PhysicsContact& contact) override;
	bool allDestroyed() override;

	BarrierEntity festzelt;
	BarrierEntity polizist;
	BarrierEntity moench;

};

#endif // __LEVEL_3_H__

