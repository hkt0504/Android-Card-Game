LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := main/main.cpp \
                   ../../Classes/common.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/ui/StartScene.cpp \
                   ../../Classes/ui/MenuScene.cpp \
                   ../../Classes/ui/ModeScene.cpp \
                   ../../Classes/ui/HelpScene.cpp \
                   ../../Classes/ui/UserScene.cpp \
                   ../../Classes/ui/GameScene.cpp \
                   ../../Classes/ui/AchievementScene.cpp \
                   ../../Classes/ui/LeaderboardScene.cpp \
                   ../../Classes/ui/BidScene.cpp \
                   ../../Classes/ui/TarneebScene.cpp \
                   ../../Classes/ui/ResultScene.cpp \
                   ../../Classes/ui/BuyScene.cpp \
                   ../../Classes/ui/PlayerObject.cpp \
                   ../../Classes/ui/CardSprite.cpp \
                   ../../Classes/ui/ClosePopup.cpp \
                   ../../Classes/engine/CardDeck.cpp \
                   ../../Classes/engine/Hand.cpp \
                   ../../Classes/engine/Player.cpp \
                   ../../Classes/engine/Controller.cpp \
                   ../../Classes/engine/GameResult.cpp \
                   ../../Classes/engine/Table.cpp \
                   ../../Classes/engine/Level.cpp \
                   ../../Classes/services/Facebook.cpp \
                   ../../Classes/services/Google.cpp \
                   ../../Classes/services/AppService.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
                    $(LOCAL_PATH)/../../Classes/ui \
                    $(LOCAL_PATH)/../../Classes/engine \
                    $(LOCAL_PATH)/../../Classes/services \
                    $(LOCAL_PATH)/../../cocos2d

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_ui_static

# LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
# LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)
$(call import-module,ui)

# $(call import-module,Box2D)
# $(call import-module,editor-support/cocosbuilder)
# $(call import-module,editor-support/spine)
# $(call import-module,editor-support/cocostudio)
# $(call import-module,network)
