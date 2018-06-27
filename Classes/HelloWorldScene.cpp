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

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene() {
	return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename) {
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
	// super init first
	if (!Scene::init()) return false;
	
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	int index = 0;

	// Start Game Item (item 1)
	auto startGameItem = MenuItemLabel::create(Label::createWithTTF("Start Game", "fonts/Marker Felt.ttf", 64));
	if (startGameItem == nullptr) problemLoading("'fonts/Marker Felt.ttf'");
	startGameItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - (index++) * 100));

	// Close Item (item 2)
	auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0) problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	else {
		closeItem->setScale(2.0f);
		closeItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - (index++) * 100));
	}


	// create menu, it's an autorelease object
	auto menu = Menu::create();
	menu->setPosition(Vec2::ZERO);
	menu->addChild(startGameItem, 2);
	menu->addChild(closeItem, 2);
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
		sprite->setPosition(Vec2(visibleSize.width / 2 - sprite->getContentSize().width * 2.0f + origin.x, visibleSize.height / 2 + origin.y));
		sprite->setScale(2.0f);
		// add the sprite as a child to this layer
		this->addChild(sprite, 0);
	}

	return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender) {
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}
