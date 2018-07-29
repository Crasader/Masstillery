#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "PlayerEntity.h"
#include "BarrierEntity.h"

class GameScene : public cocos2d::Scene
{
public:
	virtual void setup(bool musicOn);

	virtual bool init();

	static std::vector<cocos2d::Vec2> calculateSurface(std::vector<cocos2d::Vec2>& keypoints);

protected:
	int timer = 0;
	bool musicOn = true;

	void startGame();

	cocos2d::Size totalSize;

	PlayerEntity player;

	std::vector<BarrierEntity> enemies;

	cocos2d::Node* foreground;
	cocos2d::Sprite* background;
	cocos2d::ParallaxNode* paraNode;

private:
	void count(float);

	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	void onKeyPressed(const cocos2d::EventKeyboard::KeyCode, cocos2d::Event* event);
	void onKeyReleased(const cocos2d::EventKeyboard::KeyCode, cocos2d::Event* event);
	bool onContactBegin(cocos2d::PhysicsContact& contact);
	void onTouchHandle(cocos2d::Touch* touch, bool started);

	void handleMove();

	bool allDestroyed();

	void checkFinish();
	void endGame(bool victory);

	cocos2d::RepeatForever* moveLeft;
	cocos2d::RepeatForever* moveRight;

	bool isGameRunning = false;
	cocos2d::Label* labelTimeInfo;
};

#endif // __GAME_SCENE_H__

