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
#include "HelpScene.h"
#include "Levels.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;

#define COLOR_LABEL Color3B(255, 214, 49)

Scene* StartScene::createScene(bool musicOn) {
	auto scene = StartScene::create();
	scene->setup(musicOn);
	return scene;
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

	Sprite *background = Sprite::create(START_SCREEN_TEX);
	background->setTextureRect(cocos2d::Rect(0, 0, visibleSize.width, visibleSize.height));

	background->setPosition(Vec2::ZERO);
	background->setAnchorPoint(Vec2::ZERO);

	this->addChild(background);

	// Title
	auto label = Label::createWithTTF("Sers Masstillery", "fonts/Marker Felt.ttf", 64);
	label->setColor(COLOR_LABEL);
	label->enableShadow(Color4B::BLACK, Size(2, -2), 1);
	label->enableOutline(Color4B::WHITE, 2);
	label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - label->getContentSize().height));
	if (label == nullptr) problemLoading("'fonts/Marker Felt.ttf'");

	// Start Level Items (item 1-3)
	auto startLevel1Label = Label::createWithTTF("Start Level 1", "fonts/Marker Felt.ttf", 64);
	startLevel1Label->setColor(COLOR_LABEL);
	startLevel1Label->enableShadow(Color4B::BLACK, Size(2, -2), 1);
	startLevel1Label->enableOutline(Color4B::WHITE, 2);
	auto startLevel1Item = MenuItemLabel::create(startLevel1Label, CC_CALLBACK_1(StartScene::menuStartCallback, this, index++));
	if (startLevel1Item == nullptr) problemLoading("'fonts/Marker Felt.ttf'");

	auto startLevel2Label = Label::createWithTTF("Start Level 2", "fonts/Marker Felt.ttf", 64);
	startLevel2Label->setColor(COLOR_LABEL);
	startLevel2Label->enableShadow(Color4B::BLACK, Size(2, -2), 1);
	startLevel2Label->enableOutline(Color4B::WHITE, 2);
	auto startLevel2Item = MenuItemLabel::create(startLevel2Label, CC_CALLBACK_1(StartScene::menuStartCallback, this, index++));
	if (startLevel2Item == nullptr) problemLoading("'fonts/Marker Felt.ttf'");

	auto startLevel3Label = Label::createWithTTF("Start Level 3", "fonts/Marker Felt.ttf", 64);
	startLevel3Label->setColor(COLOR_LABEL);
	startLevel3Label->enableShadow(Color4B::BLACK, Size(2, -2), 1);
	startLevel3Label->enableOutline(Color4B::WHITE, 2);
	auto startLevel3Item = MenuItemLabel::create(startLevel3Label, CC_CALLBACK_1(StartScene::menuStartCallback, this, index++));
	if (startLevel3Item == nullptr) problemLoading("'fonts/Marker Felt.ttf'");

	auto startRandomLevelLabel = Label::createWithTTF("Start Random Level", "fonts/Marker Felt.ttf", 64);
	startRandomLevelLabel->setColor(COLOR_LABEL);
	startRandomLevelLabel->enableShadow(Color4B::BLACK, Size(2, -2), 1);
	startRandomLevelLabel->enableOutline(Color4B::WHITE, 2);
	auto startRandomLevelItem = MenuItemLabel::create(startRandomLevelLabel, CC_CALLBACK_1(StartScene::menuStartCallback, this, index++));
	if (startRandomLevelItem == nullptr) problemLoading("'fonts/Marker Felt.ttf'");

#ifndef ANDROID
	auto startMPLevelLabel = Label::createWithTTF("Start Multiplayer Level", "fonts/Marker Felt.ttf", 64);
	startMPLevelLabel->setColor(COLOR_LABEL);
	startMPLevelLabel->enableShadow(Color4B::BLACK, Size(2, -2), 1);
	startMPLevelLabel->enableOutline(Color4B::WHITE, 2);
	auto startMPLevelItem = MenuItemLabel::create(startMPLevelLabel, CC_CALLBACK_1(StartScene::menuStartCallback, this, index++));
	if (startMPLevelItem == nullptr) problemLoading("'fonts/Marker Felt.ttf'");
#endif

	auto showHelpLabel = Label::createWithTTF("Show Controls", "fonts/Marker Felt.ttf", 64);
	showHelpLabel->setColor(COLOR_LABEL);
	showHelpLabel->enableShadow(Color4B::BLACK, Size(2, -2), 1);
	showHelpLabel->enableOutline(Color4B::WHITE, 2);
	auto showHelpItem = MenuItemLabel::create(showHelpLabel, CC_CALLBACK_1(StartScene::menuStartCallback, this, index++));
	if (showHelpItem == nullptr) problemLoading("'fonts/Marker Felt.ttf'");

	auto switchMusicLabel = Label::createWithTTF("Music: --", "fonts/Marker Felt.ttf", 64);
	switchMusicLabel->setColor(COLOR_LABEL);
	switchMusicLabel->enableShadow(Color4B::BLACK, Size(2, -2), 1);
	switchMusicLabel->enableOutline(Color4B::WHITE, 2);
	switchMusicItem = MenuItemLabel::create(switchMusicLabel, CC_CALLBACK_0(StartScene::switchMusic, this));
	if (showHelpItem == nullptr) problemLoading("'fonts/Marker Felt.ttf'");

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
	auto menu = Menu::create(startLevel1Item, startLevel2Item, startLevel3Item, startRandomLevelItem, showHelpItem, switchMusicItem, closeItem, NULL);
#else
	auto menu = Menu::create(startLevel1Item, startLevel2Item, startLevel3Item, startRandomLevelItem, startMPLevelItem, showHelpItem, switchMusicItem, closeItem, NULL);
#endif

	menu->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	menu->alignItemsVertically();

	this->addChild(label, 1);
	this->addChild(menu, 2);

	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/blasmusik1.mp3");
	return true;
}


void StartScene::setup(bool musicOn) {
	setMusic(musicOn);
}

void StartScene::menuStartCallback(cocos2d::Ref * pSender, int level) {
	switch (level) {
	case 0:	Director::getInstance()->replaceScene(Level1::createScene(musicOn)); break;
	case 1:	Director::getInstance()->replaceScene(Level2::createScene(musicOn)); break;
	case 2:	Director::getInstance()->replaceScene(Level3::createScene(musicOn)); break;
	case 3:	Director::getInstance()->replaceScene(LevelRandom::createScene(musicOn)); break;
#ifdef ANDROID
	case 4:	Director::getInstance()->replaceScene(HelpScene::createScene(musicOn)); break;
#else
	case 4:	Director::getInstance()->replaceScene(LevelMP::createScene(musicOn)); break;
	case 5: Director::getInstance()->replaceScene(HelpScene::createScene(musicOn)); break;
#endif
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

void StartScene::switchMusic() {
	this->musicOn = !musicOn;
	setMusic(this->musicOn);
}

void StartScene::setMusic(bool musicOn) {
	this->musicOn = musicOn;
	if (musicOn) {
		switchMusicItem->setString("Music: ON");
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/blasmusik1.mp3");
	}
	else {
		switchMusicItem->setString("Music: OFF");
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	}
}
