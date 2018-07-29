
#ifndef __HELP_SCENE_H__
#define __HELP_SCENE_H__

#include "cocos2d.h"

class HelpScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene(bool musicOn);

    virtual bool init();

    CREATE_FUNC(HelpScene);

private:
	void onKeyReleased(const cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	bool musicOn = true;
};

#endif // __HELP_SCENE_H__
