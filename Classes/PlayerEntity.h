#ifndef __PLAYER_ENTITY_H__
#define __PLAYER_ENTITY_H__

#include "cocos2d.h"

class PlayerEntity
{
public:
	bool init();

	void moveLeft(bool state);
	void moveRight(bool state);

	cocos2d::Sprite* getSprite();

private:
	cocos2d::Sprite* playerSprite;
	cocos2d::Action* moveLeftAction;
	cocos2d::Action* moveRightAction;
};

#endif // __PLAYER_ENTITY_H__
