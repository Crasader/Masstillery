#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "cocos2d.h"

#define ENTITY_HEIGHT 150
#define MAX_HITPOINTS 1000

class Entity
{
public:
	bool init(const std::string& name);

	void moveLeft(bool state);
	void moveRight(bool state);

	void moveToX(int x);

	cocos2d::Sprite* sprite;

	void handleContact(cocos2d::PhysicsContact& contact);

	inline bool isDestroyed() { return hitpoints <= 0; }

private:
	void handleMove();

	void explode();

	cocos2d::Label* hitpointLabel;

	int hitpoints = MAX_HITPOINTS;
};

#endif // __ENTITY_H__
