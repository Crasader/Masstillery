#include "Entity.h"
#include "PhysicsCategories.h"

USING_NS_CC;

bool Entity::init(const std::string& filename) {
	sprite = Sprite::create(filename);
	if (sprite == nullptr) return false;
	auto spriteSize = sprite->getContentSize();
	auto scale = ENTITY_HEIGHT / spriteSize.height;
	sprite->setScale(scale);
	sprite->setAnchorPoint(Vec2(0.5, 0));

	auto pb = PhysicsBody::createBox(sprite->getContentSize());
	pb->setDynamic(false);
	pb->setCollisionBitmask(0x0);
	sprite->setPhysicsBody(pb);

	auto playerSpriteSize = sprite->getContentSize();

	hitpointLabel = Label::createWithTTF(std::to_string(hitpoints), "fonts/Marker Felt.ttf", 80);
	hitpointLabel->setAnchorPoint(Vec2(0.5, 0.5));
	hitpointLabel->setPosition(Vec2(playerSpriteSize.width * 0.5, -(playerSpriteSize.height * 0.2)));

	sprite->addChild(hitpointLabel);

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

void Entity::moveToX(int x) {
	sprite->setPositionX(x);
	sprite->runAction(CallFunc::create(CC_CALLBACK_0(Entity::handleMove, this)));
}

void Entity::handleContact(cocos2d::PhysicsContact & contact) {
	if (hitpoints <= 0) return;

	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	auto vel = 0;

	if (nodeA == sprite) 		vel = contact.getShapeB()->getBody()->getVelocity().length();
	else if (nodeB == sprite)	vel = contact.getShapeA()->getBody()->getVelocity().length();

	hitpoints -= vel * 0.1;

	hitpointLabel->setString(std::to_string(hitpoints));

	if (hitpoints <= 0) explode();
}

void Entity::handleMove() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto playerSize = sprite->getContentSize() * sprite->getScale();
	auto playerX = sprite->getPosition().x;
	Vec2 end(playerX, visibleSize.height);
	Vec2 start(playerX, 0);

	Vec2 point;
	auto func = [&point](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data)->bool {
		if (info.shape->getBody()->getNode()->getTag() != TERRAIN_TAG) return true;

		point = info.contact;
		return false;
	};

	sprite->getScene()->getPhysicsWorld()->rayCast(func, start, end, nullptr);

	auto offsetLeft = playerSize.width * sprite->getAnchorPoint().x;
	auto offsetRight = playerSize.width * (1 - sprite->getAnchorPoint().x);

	if (point.x < offsetLeft) {
		point.x = offsetLeft;
	}
	else if (point.x > visibleSize.width - offsetRight) {
		point.x = visibleSize.width - offsetRight;
	}

	sprite->setPosition(point);
}

void Entity::explode() {
	hitpointLabel->removeFromParentAndCleanup(true);

	auto emitter = ParticleExplosion::create();
	emitter->setPosition(sprite->getPosition() + Vec2(0, ENTITY_HEIGHT / 2));

	sprite->getScene()->addChild(emitter, 0);

	sprite->removeFromParentAndCleanup(true);
}
