#ifndef __LEVEL_2_H__
#define __LEVEL_2_H__

#include "cocos2d.h"
#include "GameScene.h"
#include "PlayerEntity.h"
#include "BarrierEntity.h"



class Level2 : public GameScene
{
public:
	static cocos2d::Scene* createScene();

	void setup();

	virtual bool init();

	CREATE_FUNC(Level2);
};

#endif // __LEVEL_1_H__

