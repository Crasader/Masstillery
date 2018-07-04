#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "PlayerEntity.h"

class GameScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(GameScene);

private:
	void count(float);

	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	void onKeyPressed(const cocos2d::EventKeyboard::KeyCode, cocos2d::Event* event);
	void onKeyReleased(const cocos2d::EventKeyboard::KeyCode, cocos2d::Event* event);
	
	cocos2d::RepeatForever* moveLeft;
	cocos2d::RepeatForever* moveRight;

	int timer = 0;

	PlayerEntity player;

	cocos2d::Label* labelTouchInfo;
	cocos2d::Label* labelTimeInfo;
};

#endif // __GAME_SCENE_H__

