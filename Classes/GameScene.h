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
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	void onKeyPressed(const cocos2d::EventKeyboard::KeyCode, cocos2d::Event* event);
	void onKeyReleased(const cocos2d::EventKeyboard::KeyCode, cocos2d::Event* event);
	
	cocos2d::RepeatForever* moveLeft;
	cocos2d::RepeatForever* moveRight;

	PlayerEntity player;
	
	cocos2d::Sprite* background;

	cocos2d::Label* labelTouchInfo;
};

#endif // __GAME_SCENE_H__
