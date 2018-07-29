/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "StartScene.h"
#include "GameScene.h"
#include "Levels.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* StartScene::createScene() {
	return StartScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename) {
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool StartScene::init() {
	// super init first
	if (!Scene::init()) return false;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	int index = 0;

	// Start Level Items (item 1-3)
	auto startLevel1Item = MenuItemLabel::create(Label::createWithTTF("Start Level 1", "fonts/Marker Felt.ttf", 64),
		CC_CALLBACK_1(StartScene::menuStartCallback, this, 1));
	if (startLevel1Item == nullptr) problemLoading("'fonts/Marker Felt.ttf'");

	auto startLevel2Item = MenuItemLabel::create(Label::createWithTTF("Start Level 2", "fonts/Marker Felt.ttf", 64),
		CC_CALLBACK_1(StartScene::menuStartCallback, this, 2));
	if (startLevel2Item == nullptr) problemLoading("'fonts/Marker Felt.ttf'");

	auto startLevel3Item = MenuItemLabel::create(Label::createWithTTF("Start Level 3", "fonts/Marker Felt.ttf", 64),
		CC_CALLBACK_1(StartScene::menuStartCallback, this, 3));
	if (startLevel3Item == nullptr) problemLoading("'fonts/Marker Felt.ttf'");

	auto startRandomLevelItem = MenuItemLabel::create(Label::createWithTTF("Start Random Level", "fonts/Marker Felt.ttf", 64),
		CC_CALLBACK_1(StartScene::menuStartCallback, this, 4));
	if (startRandomLevelItem == nullptr) problemLoading("'fonts/Marker Felt.ttf'");

#ifndef ANDROID
	auto startMPLevelItem = MenuItemLabel::create(Label::createWithTTF("Start Multiplayer Level", "fonts/Marker Felt.ttf", 64),
		CC_CALLBACK_1(StartScene::menuStartCallback, this, 5));
	if (startMPLevelItem == nullptr) problemLoading("'fonts/Marker Felt.ttf'");
#endif

	// Close Item (item 4)
	auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
		CC_CALLBACK_1(StartScene::menuCloseCallback, this));
	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0) problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	else {
		closeItem->setScale(2.0f);
	}


	// create menu, it's an autorelease object
#ifdef ANDROID
	auto menu = Menu::create(startLevel1Item, startLevel2Item, startLevel3Item, startRandomLevelItem, closeItem, NULL);
#else
	auto menu = Menu::create(startLevel1Item, startLevel2Item, startLevel3Item, startRandomLevelItem, startMPLevelItem, closeItem, NULL);
#endif
	menu->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	menu->alignItemsVertically();
	this->addChild(menu, 2);

	// Title
	auto label = Label::createWithTTF("Sers Masstillery", "fonts/Marker Felt.ttf", 64);
	if (label == nullptr) problemLoading("'fonts/Marker Felt.ttf'");
	else {
		// position the label on the center of the screen
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - label->getContentSize().height));

		// add the label as a child to this layer
		this->addChild(label, 1);
	}

	// Background sprite
	auto sprite = Sprite::create("HelloWorld.png");
	if (sprite == nullptr) problemLoading("'HelloWorld.png'");
	else {
		// position the sprite on the center of the screen
		sprite->setPosition(Vec2::ZERO);
		sprite->setAnchorPoint(Vec2::ZERO);
		sprite->setScale(2.0f);
		// add the sprite as a child to this layer
		this->addChild(sprite, 0);
	}

	return true;
}


void StartScene::menuStartCallback(cocos2d::Ref * pSender, int level) {
	switch (level) {
	case 1:	Director::getInstance()->replaceScene(Level1::createScene()); break;
	case 2:	Director::getInstance()->replaceScene(Level2::createScene()); break;
	case 3:	Director::getInstance()->replaceScene(Level3::createScene()); break;
	case 4:	Director::getInstance()->replaceScene(LevelRandom::createScene()); break;
	case 5:	Director::getInstance()->replaceScene(LevelMP::createScene()); break;
	}

}

void StartScene::menuCloseCallback(Ref* pSender) {
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}
