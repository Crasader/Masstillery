#ifndef __PLAYER_ENTITY_H__
#define __PLAYER_ENTITY_H__

#include "cocos2d.h"

#define SHOOT_MAX_ACCEL 1000
#define SHOOT_MIN_ACCEL 500

class PlayerEntity
{
public:
	bool init();

	void moveLeft(bool state);
	void moveRight(bool state);

	void moveShootLeft(bool state);
	void moveShootRight(bool state);

	void increaseAccel(bool state);
	void decreaseAccel(bool state);

	void shoot();

	inline cocos2d::Sprite* getSprite() { return playerSprite; }
	inline cocos2d::DrawNode* getShootArrow() { return arrowNode; }

private:
	void handleMove();

	cocos2d::Sprite* playerSprite;
	cocos2d::DrawNode* arrowNode;
	cocos2d::Label* accelLabel;

	cocos2d::Vec2 shootDirection{ 0,-1 };
	int shootAcceleration = SHOOT_MIN_ACCEL;
};

#endif // __PLAYER_ENTITY_H__
