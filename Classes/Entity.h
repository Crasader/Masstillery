#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "cocos2d.h"

class Entity
{
public:
	bool init(const std::string& filename);

	void moveLeft(bool state);
	void moveRight(bool state);

	void moveToX(int x);

	cocos2d::Sprite* sprite;

protected:
	void handleMove();

};

#endif // __ENTITY_H__
