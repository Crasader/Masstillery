#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "PlayerEntity.h"
#include "BarrierEntity.h"

class GameScene : public cocos2d::Scene
{
public:
	virtual void setup();

	virtual bool init();

protected:
	int timer = 0;

	PlayerEntity player;

	cocos2d::Sprite* background;

private:
	void count(float);

	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	void onKeyPressed(const cocos2d::EventKeyboard::KeyCode, cocos2d::Event* event);
	void onKeyReleased(const cocos2d::EventKeyboard::KeyCode, cocos2d::Event* event);
	bool onContactBegin(cocos2d::PhysicsContact& contact);
	void onTouchHandle(cocos2d::Touch* touch, bool started);

	virtual void handleContact(cocos2d::PhysicsContact& contact) {};
	virtual bool allDestroyed() { return false; };

	void checkFinish();
	void endGame(bool victory);

	cocos2d::RepeatForever* moveLeft;
	cocos2d::RepeatForever* moveRight;

	bool isGameRunning = false;
	cocos2d::Label* labelTimeInfo;
};

#endif // __GAME_SCENE_H__

