LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := $(LOCAL_PATH)/hellocpp/main.cpp \
                   $(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \
				   $(LOCAL_PATH)/../../../Classes/Entity.cpp \
           $(LOCAL_PATH)/../../../Classes/HelpScene.cpp \
				   $(LOCAL_PATH)/../../../Classes/GameScene.cpp \
				   $(LOCAL_PATH)/../../../Classes/Level1.cpp \
				   $(LOCAL_PATH)/../../../Classes/Level2.cpp \
				   $(LOCAL_PATH)/../../../Classes/Level3.cpp \
				   $(LOCAL_PATH)/../../../Classes/LevelMP.cpp \
				   $(LOCAL_PATH)/../../../Classes/LevelRandom.cpp \
				   $(LOCAL_PATH)/../../../Classes/PhysicsShapeCache.cpp \
				   $(LOCAL_PATH)/../../../Classes/PlayerEntity.cpp \
           $(LOCAL_PATH)/../../../Classes/StartScene.cpp 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(LOCAL_PATH)/../../../cocos2d)
$(call import-module, cocos)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
