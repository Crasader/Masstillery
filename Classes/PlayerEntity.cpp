#include "PlayerEntity.h"
#include "PhysicsCategories.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

bool PlayerEntity::init(const std::string& name) {

	if (!Entity::init(name, 1000)) return false;

	sprite->getPhysicsBody()->setDynamic(true);
	sprite->getPhysicsBody()->setRotationEnable(false);

	auto playerSpriteSize = sprite->getContentSize();

	arrowNode = DrawNode::create();
	arrowNode->setContentSize(playerSpriteSize);
	arrowNode->setAnchorPoint(Vec2(0.5, 0.5));
	
	sprite->addChild(arrowNode, 2);

	updateArrow();

	return true;
}

void PlayerEntity::moveShootLeft(bool state) {
	arrowNode->stopAllActions();

	if (state) {
		auto moveShootRightAction = RepeatForever::create(RotateBy::create(1, -30.0f));
		arrowNode->runAction(moveShootRightAction);
	}
}

void PlayerEntity::moveShootRight(bool state) {
	arrowNode->stopAllActions();

	if (state) {
		auto moveShootRightAction = RepeatForever::create(RotateBy::create(1, 30.0f));
		arrowNode->runAction(moveShootRightAction);
	}
}

void PlayerEntity::increaseAccel(bool state) {
	arrowNode->stopAllActions();

	if (state) {
		auto increaseAction = CallFunc::create([=]() {
			shootAcceleration += 10;
			if (shootAcceleration > SHOOT_MAX_ACCEL)
				shootAcceleration = SHOOT_MAX_ACCEL;
			updateArrow();
		});
		auto increaseSequence = Sequence::create(DelayTime::create(0.01), increaseAction, nullptr);
		arrowNode->runAction(RepeatForever::create(increaseSequence));
	}
}

void PlayerEntity::decreaseAccel(bool state) {
	arrowNode->stopAllActions();

	if (state) {
		auto decreaseAction = CallFunc::create([=]() {
			shootAcceleration -= 10;
			if (shootAcceleration < SHOOT_MIN_ACCEL)
				shootAcceleration = SHOOT_MIN_ACCEL;
			updateArrow();
		});
		auto decreaseSequence = Sequence::create(DelayTime::create(0.01), decreaseAction, nullptr);
		arrowNode->runAction(RepeatForever::create(decreaseSequence));
	}
}

void PlayerEntity::updateArrow() {
	auto playerSpriteSize = sprite->getContentSize();

	auto accelFactor = 1 + ((float)shootAcceleration - SHOOT_MIN_ACCEL) / (SHOOT_MAX_ACCEL - SHOOT_MIN_ACCEL);

	auto spriteMidX = playerSpriteSize.width * sprite->getAnchorPoint().x;
	auto spriteMidY = playerSpriteSize.height * 0.5;

	arrowNode->clear();
	arrowNode->drawSegment(
		Vec2(spriteMidX, spriteMidY),
		Vec2(spriteMidX, playerSpriteSize.height * accelFactor), 8, Color4F::RED);
	arrowNode->drawSegment(
		Vec2(spriteMidX - 50, spriteMidY  * accelFactor + 50),
		Vec2(spriteMidX, playerSpriteSize.height * accelFactor), 8, Color4F::RED);
	arrowNode->drawSegment(
		Vec2(spriteMidX + 50, spriteMidY * accelFactor + 50),
		Vec2(spriteMidX, playerSpriteSize.height * accelFactor), 8, Color4F::RED);
	arrowNode->setPosition(playerSpriteSize * 0.5);
}

void PlayerEntity::shoot(Color3B color, int shotTag, int playerTag) {
	auto shotPb = PhysicsBody::createCircle(10);
	auto rot = MATH_DEG_TO_RAD(arrowNode->getRotation() + CCRANDOM_MINUS1_1());
	auto angle = Vec2(sin(rot), cos(rot));
	angle.normalize();
	shotPb->setVelocity(angle*shootAcceleration);
	shotPb->setDynamic(true);
	shotPb->setCategoryBitmask(shotTag);
	shotPb->setCollisionBitmask(0x0);
	shotPb->setContactTestBitmask(TERRAIN_TAG | BARRIER_TAG | playerTag);

	auto shot = DrawNode::create();
	shot->drawSolidCircle(Vec2::ZERO, 10, 0, 20, Color4F(color));
	shot->setPhysicsBody(shotPb);

	shot->setPosition(getRealPosition() + Vec2(0, ENTITY_HEIGHT / 2));
	shot->setTag(shotTag);

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/pew.mp3");

	sprite->getScene()->addChild(shot);
}