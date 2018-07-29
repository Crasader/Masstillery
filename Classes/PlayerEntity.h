#ifndef __PLAYER_ENTITY_H__
#define __PLAYER_ENTITY_H__

#include "cocos2d.h"
#include "Entity.h"
#include "PhysicsCategories.h"

#define SHOOT_MAX_ACCEL 1000
#define SHOOT_MIN_ACCEL 500

class PlayerEntity : public Entity
{
public:
	bool init(const std::string& filename);

	void moveShootLeft(bool state);
	void moveShootRight(bool state);

	void increaseAccel(bool state);
	void decreaseAccel(bool state);

	void shoot(cocos2d::Color3B color = cocos2d::Color3B(255, 214, 49), int shotTag = SHOT_TAG, int playerTag = PLAYER2_TAG);

	inline cocos2d::DrawNode* getShootArrow() { return arrowNode; }

private:
	void updateArrow();

	cocos2d::DrawNode* arrowNode;

	cocos2d::Vec2 shootDirection{ 0,-1 };
	int shootAcceleration = SHOOT_MIN_ACCEL;
};

#endif // __PLAYER_ENTITY_H__
