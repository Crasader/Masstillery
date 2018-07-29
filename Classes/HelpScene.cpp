#include "HelpScene.h"
#include "StartScene.h"

USING_NS_CC;

Scene* HelpScene::createScene() {
	return HelpScene::create();
}

// on "init" you need to initialize your instance
bool HelpScene::init() {
	// super init first
	if (!Scene::init()) return false;

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyReleased = CC_CALLBACK_2(HelpScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto bg = cocos2d::LayerColor::create(Color4B::WHITE);

#ifdef ANDROID
	auto kb = Sprite::create("background/smartphone.png");
#else
	auto kb = Sprite::create("background/keyboard.png");
#endif // ANDROID

	kb->setPosition(visibleSize / 2);

	auto scale = 1.0f;

	if (kb->getContentSize().width > visibleSize.width) {
		scale = visibleSize.width / kb->getContentSize().width;
		kb->setScale(scale);
	}

	if (kb->getContentSize().height * scale > visibleSize.height) {
		scale =  visibleSize.height / kb->getContentSize().height;
		kb->setScale(scale);
	}

	this->addChild(bg);
	this->addChild(kb);

	return true;
}

void HelpScene::onKeyReleased(const EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) Director::getInstance()->replaceScene(StartScene::createScene());
}

