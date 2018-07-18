#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "PlayerEntity.h"
#include "BarrierEntity.h"

class GameScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene(int time, std::string skyTex, std::string terrainTex, bool tiled);

	virtual void setup(int time, std::string skyTex, std::string terrainTex, bool tiled);

	virtual bool init();

	CREATE_FUNC(GameScene);

private:
	void count(float);

	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	void onKeyPressed(const cocos2d::EventKeyboard::KeyCode, cocos2d::Event* event);
	void onKeyReleased(const cocos2d::EventKeyboard::KeyCode, cocos2d::Event* event);
	bool onContactBegin(cocos2d::PhysicsContact& contact);
	void onTouchHandle(cocos2d::Touch* touch, bool started);

	void checkFinish();
	void endGame(bool victory);

	cocos2d::RepeatForever* moveLeft;
	cocos2d::RepeatForever* moveRight;

	int timer = 0;

	PlayerEntity player;
	BarrierEntity festzelt;
	BarrierEntity polizist;
	BarrierEntity moench;
	
	cocos2d::Label* labelTimeInfo;

	cocos2d::Sprite* background;
};

#endif // __GAME_SCENE_H__

