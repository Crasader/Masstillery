#include "PlayerEntity.h"

USING_NS_CC;

bool PlayerEntity::init() {
	playerSprite = Sprite::create("assets/Mass.png");
	return playerSprite == nullptr;
}

cocos2d::Sprite* PlayerEntity::getSprite() {
	return playerSprite;
}

void PlayerEntity::moveLeft(bool state) {
	if (state) {
		moveLeftAction = RepeatForever::create(MoveBy::create(1, Vec2(-200, 0)));
		playerSprite->runAction(moveLeftAction);
	}
	else {
		playerSprite->stopAction(moveLeftAction);
	}
}

void PlayerEntity::moveRight(bool state) {
	if (state) {
		moveRightAction = RepeatForever::create(MoveBy::create(1, Vec2(200, 0)));
		playerSprite->runAction(moveRightAction);
	}
	else {
		playerSprite->stopAction(moveRightAction);
	}
}