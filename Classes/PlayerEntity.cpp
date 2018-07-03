#include "PlayerEntity.h"

USING_NS_CC;

bool PlayerEntity::init() {
	playerSprite = Sprite::create("entities/Mass.png");

	return playerSprite == nullptr;
}

cocos2d::Sprite* PlayerEntity::getSprite() {
	return playerSprite;
}

void PlayerEntity::moveLeft(bool state) {
	playerSprite->stopAllActions();

	if (state) {
		moveLeftAction = RepeatForever::create(MoveBy::create(1, Vec2(-200, 0)));
		playerSprite->runAction(moveLeftAction);
	}
}

void PlayerEntity::moveRight(bool state) {
	playerSprite->stopAllActions();

	if (state) {
		moveRightAction = RepeatForever::create(MoveBy::create(1, Vec2(200, 0)));
		playerSprite->runAction(moveRightAction);
	}
}