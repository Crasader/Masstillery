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
	static BarrierEntity createPolizist() { return BarrierEntity("Polizist", 1000); }
	static BarrierEntity createBreze() { return BarrierEntity("Breze", 200); }
	static BarrierEntity createMoench() { return BarrierEntity("Moench", 400); }
	static BarrierEntity createFestzelt() { return BarrierEntity("Festzelt", 600); }

	BarrierEntity(const std::string& filename, int hitpoints) {
		if (!Entity::init(filename, hitpoints)) return;

		sprite->getPhysicsBody()->setContactTestBitmask(SHOT_TAG | SHOT2_TAG);
		sprite->getPhysicsBody()->setTag(BARRIER_TAG);
	}
};

#endif // __BARRIER_ENTITY_H__
