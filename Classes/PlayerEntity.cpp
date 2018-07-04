#include "PlayerEntity.h"

USING_NS_CC;

bool PlayerEntity::init() {
	playerSprite = Sprite::create("entities/Mass.png");
	if (playerSprite == nullptr) return false;
	auto scale = 150.0 / playerSprite->getContentSize().height;
	playerSprite->setScale(scale);
	playerSprite->setAnchorPoint(Vec2(0.33, 0));

	//auto pb = PhysicsBody::createBox(playerSprite->getContentSize());
	//pb->setDynamic(false);
	//playerSprite->setPhysicsBody(pb);

	return true;
}

cocos2d::Sprite* PlayerEntity::getSprite() {
	return playerSprite;
}

void PlayerEntity::moveLeft(bool state) {
	playerSprite->stopAllActions();

	if (state) {
		handleMoveAction = RepeatForever::create(Spawn::create(CallFunc::create(CC_CALLBACK_0(PlayerEntity::handleMove, this)), nullptr));
		moveLeftAction = RepeatForever::create(MoveBy::create(1, Vec2(-200, 0)));
		playerSprite->runAction(moveLeftAction);
		playerSprite->runAction(handleMoveAction);
	}
}

void PlayerEntity::moveRight(bool state) {
	playerSprite->stopAllActions();

	if (state) {
		handleMoveAction = RepeatForever::create(Spawn::create(CallFunc::create(CC_CALLBACK_0(PlayerEntity::handleMove, this)), nullptr));
		moveRightAction = RepeatForever::create(MoveBy::create(1, Vec2(200, 0)));
		playerSprite->runAction(moveRightAction);
		playerSprite->runAction(handleMoveAction);
	}
}

void PlayerEntity::handleMove()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto playerSize = playerSprite->getContentSize() * playerSprite->getScale();
	auto playerX = playerSprite->getPosition().x;
	Vec2 end(playerX, visibleSize.height);
	Vec2 start(playerX, 0);

	Vec2 point;
	auto func = [&point](PhysicsWorld& world,
		const PhysicsRayCastInfo& info, void* data)->bool {
		point = info.contact;
		return true;
	};

	playerSprite->getScene()->getPhysicsWorld()->rayCast(func, start, end, nullptr);

	if (point.x < playerSize.width * 0.33) {
		point.x = playerSize.width * 0.33;
	}
	else if (point.x > visibleSize.width - playerSize.width * 0.66) {
		point.x = visibleSize.width - playerSize.width * 0.66;
	}

	playerSprite->setPosition(point);
}