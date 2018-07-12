#include "Entity.h"

USING_NS_CC;

bool Entity::init(const std::string& filename) {
	sprite = Sprite::create(filename);
	if (sprite == nullptr) return false;
	auto spriteSize = sprite->getContentSize();
	auto scale = 150.0 / spriteSize.height;
	sprite->setScale(scale);
	sprite->setAnchorPoint(Vec2(0.5, 0));

	//auto pb = PhysicsBody::createBox(sprite->getContentSize());
	//pb->setDynamic(false);
	//sprite->setPhysicsBody(pb);

	return true;
}

void Entity::moveLeft(bool state) {
	sprite->stopAllActions();

	if (state) {
		auto handleMoveAction = RepeatForever::create(Spawn::create(CallFunc::create(CC_CALLBACK_0(Entity::handleMove, this)), nullptr));
		auto moveLeftAction = RepeatForever::create(MoveBy::create(1, Vec2(-200, 0)));
		sprite->runAction(moveLeftAction);
		sprite->runAction(handleMoveAction);
	}
}

void Entity::moveRight(bool state) {
	sprite->stopAllActions();

	if (state) {
		auto handleMoveAction = RepeatForever::create(Spawn::create(CallFunc::create(CC_CALLBACK_0(Entity::handleMove, this)), nullptr));
		auto moveRightAction = RepeatForever::create(MoveBy::create(1, Vec2(200, 0)));
		sprite->runAction(moveRightAction);
		sprite->runAction(handleMoveAction);
	}
}

void Entity::handleMove() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto playerSize = sprite->getContentSize() * sprite->getScale();
	auto playerX = sprite->getPosition().x;
	Vec2 end(playerX, visibleSize.height);
	Vec2 start(playerX, 0);

	Vec2 point;
	auto func = [&point](PhysicsWorld& world,
		const PhysicsRayCastInfo& info, void* data)->bool {
		point = info.contact;
		return true;
	};

	sprite->getScene()->getPhysicsWorld()->rayCast(func, start, end, nullptr);

	if (point.x < playerSize.width * 0.33) {
		point.x = playerSize.width * 0.33;
	}
	else if (point.x > visibleSize.width - playerSize.width * 0.66) {
		point.x = visibleSize.width - playerSize.width * 0.66;
	}

	sprite->setPosition(point);
}