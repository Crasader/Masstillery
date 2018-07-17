#ifndef __BARRIER_ENTITY_H__
#define __BARRIER_ENTITY_H__

#include "cocos2d.h"
#include "Entity.h"
#include "PhysicsCategories.h"

#define SHOOT_MAX_ACCEL 1000
#define SHOOT_MIN_ACCEL 500

class BarrierEntity : public Entity
{
public:
	bool init(const std::string& filename) {
		if (!Entity::init(filename)) return false;

		sprite->getPhysicsBody()->setContactTestBitmask(SHOT_TAG);
		sprite->getPhysicsBody()->setTag(BARRIER_TAG);

		return true;
	}
};

#endif // __BARRIER_ENTITY_H__
