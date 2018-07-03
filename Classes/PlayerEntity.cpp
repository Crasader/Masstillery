#include "PlayerEntity.h"

USING_NS_CC;

bool PlayerEntity::init() {
	playerSprite = Sprite::create("entities/Mass.png");
	if (playerSprite == nullptr) return false;
	auto scale = 150.0 / playerSprite->getContentSize().height;
	playerSprite->setScale(scale);
	playerSprite->setAnchorPoint(Vec2(0.5, 0));
	return true;
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