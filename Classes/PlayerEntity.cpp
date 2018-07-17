#include "PlayerEntity.h"
#include "PhysicsCategories.h"

USING_NS_CC;

bool PlayerEntity::init(const std::string& filename) {

	if (!Entity::init(filename)) return false;

	auto playerSpriteSize = sprite->getContentSize();

	arrowNode = DrawNode::create();
	arrowNode->setContentSize(playerSpriteSize);
	arrowNode->setAnchorPoint(Vec2(0.5, 0.5));
	arrowNode->drawLine(
		Vec2(playerSpriteSize.width * 0.5, playerSpriteSize.height * 0.5),
		Vec2(playerSpriteSize.width * 0.5, playerSpriteSize.height), Color4F::RED);
	arrowNode->drawLine(
		Vec2(playerSpriteSize.width * 0.25, playerSpriteSize.height * 0.75),
		Vec2(playerSpriteSize.width * 0.5, playerSpriteSize.height), Color4F::RED);
	arrowNode->drawLine(
		Vec2(playerSpriteSize.width * 0.75, playerSpriteSize.height * 0.75),
		Vec2(playerSpriteSize.width * 0.5, playerSpriteSize.height), Color4F::RED);
	arrowNode->setPosition(playerSpriteSize * 0.5);

	accelLabel = Label::createWithTTF(std::to_string(shootAcceleration), "fonts/Marker Felt.ttf", 80);
	accelLabel->setAnchorPoint(Vec2(0.5, 0.5));
	accelLabel->setPosition(Vec2(playerSpriteSize.width * 0.5, playerSpriteSize.height * 0.1));

	sprite->addChild(arrowNode, 2);
	sprite->addChild(accelLabel);

	return true;
}

void PlayerEntity::moveShootLeft(bool state) {
	arrowNode->stopAllActions();

	if (state) {
		auto moveShootRightAction = RepeatForever::create(RotateBy::create(1, -20.0f));
		arrowNode->runAction(moveShootRightAction);
	}
}

void PlayerEntity::moveShootRight(bool state) {
	arrowNode->stopAllActions();

	if (state) {
		auto moveShootRightAction = RepeatForever::create(RotateBy::create(1, 20.0f));
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
			accelLabel->setString(std::to_string(shootAcceleration));
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
			accelLabel->setString(std::to_string(shootAcceleration));
		});
		auto decreaseSequence = Sequence::create(DelayTime::create(0.01), decreaseAction, nullptr);
		arrowNode->runAction(RepeatForever::create(decreaseSequence));
	}
}

void PlayerEntity::shoot() {
	auto shotPb = PhysicsBody::createCircle(10);
	auto rot = MATH_DEG_TO_RAD(arrowNode->getRotation());
	auto angle = Vec2(sin(rot), cos(rot));
	angle.normalize();
	shotPb->setVelocity(angle*shootAcceleration);
	shotPb->setDynamic(true);
	shotPb->setCategoryBitmask(SHOT_TAG);
	shotPb->setContactTestBitmask(TERRAIN_TAG & BARRIER_TAG);

	auto shot = DrawNode::create();
	shot->drawSolidCircle(Vec2::ZERO, 10, 0, 20, Color4F::YELLOW);
	shot->setPhysicsBody(shotPb);
	//shot->setAnchorPoint(Vec2(0.5, 0.5));
	shot->setPosition(sprite->getPosition() + Vec2(0, 100));
	shot->setTag(SHOT_TAG);

	sprite->getScene()->addChild(shot);
}