#include "PlayerEntity.h"

USING_NS_CC;

bool PlayerEntity::init() {
	playerSprite = Sprite::create("entities/Mass.png");
	if (playerSprite == nullptr) return false;
	auto playerSpriteSize = playerSprite->getContentSize();
	auto scale = 150.0 / playerSpriteSize.height;
	playerSprite->setScale(scale);
	playerSprite->setAnchorPoint(Vec2(0.5, 0));

	//auto pb = PhysicsBody::createBox(playerSprite->getContentSize());
	//pb->setDynamic(false);
	//playerSprite->setPhysicsBody(pb);

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

	playerSprite->addChild(arrowNode, 2);
	playerSprite->addChild(accelLabel);

	return true;
}

void PlayerEntity::moveLeft(bool state) {
	playerSprite->stopAllActions();

	if (state) {
		auto handleMoveAction = RepeatForever::create(Spawn::create(CallFunc::create(CC_CALLBACK_0(PlayerEntity::handleMove, this)), nullptr));
		auto moveLeftAction = RepeatForever::create(MoveBy::create(1, Vec2(-200, 0)));
		playerSprite->runAction(moveLeftAction);
		playerSprite->runAction(handleMoveAction);
	}
}

void PlayerEntity::moveRight(bool state) {
	playerSprite->stopAllActions();

	if (state) {
		auto handleMoveAction = RepeatForever::create(Spawn::create(CallFunc::create(CC_CALLBACK_0(PlayerEntity::handleMove, this)), nullptr));
		auto moveRightAction = RepeatForever::create(MoveBy::create(1, Vec2(200, 0)));
		playerSprite->runAction(moveRightAction);
		playerSprite->runAction(handleMoveAction);
	}
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

	auto  shot = DrawNode::create();
	shot->drawSolidCircle(Vec2::ZERO, 10, 0, 20, Color4F::YELLOW);
	shot->setPhysicsBody(shotPb);
	//shot->setAnchorPoint(Vec2(0.5, 0.5));
	shot->setPosition(playerSprite->getPosition() + Vec2(0, 100));


	playerSprite->getScene()->addChild(shot);
}

void PlayerEntity::handleMove() {
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