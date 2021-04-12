#include "AppDelegate.h"
#include "Scene\HelloWorldScene.h"
#include "Scene\IntroScene.h"
#include "Scene\StatScene.h"
#include "Scene\SkillsScene.h"
#include "Scene\SkillSelectionScene.h"
#include "Scene\AlliesSelectionScene.h"
#include "Scene\BossCapScene.h"
#include "Scene\MercenaryShopScene.h"
#include "Scene\BossSelectionScene.h"
#include "Scene\LoyaltySelectionScene.h"
#include "Scene\OptionScene.h"
#include "Scene\GameCompleteScene.h"
#include "Scene\SplashScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
USING_NS_CC;

//static cocos2d::Size designResolutionSize = cocos2d::Size(736, 414);
static cocos2d::Size designResolutionSize = cocos2d::Size(1230, 800);
static cocos2d::Size smallResolutionSize = cocos2d::Size(400, 300);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(800, 600);
static cocos2d::Size largeResolutionSize = cocos2d::Size(1600, 1200);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{

}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("Bug", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("testgame");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

    register_all_packages();

	// preload sounds
	auto audio = SimpleAudioEngine::getInstance();

	audio->preloadBackgroundMusic("Sounds/mp3/Music/1_stage.mp3");
	audio->preloadBackgroundMusic("Sounds/mp3/Music/2_stage.mp3");
	audio->preloadBackgroundMusic("Sounds/mp3/Music/3_stage.mp3");
	audio->preloadBackgroundMusic("Sounds/mp3/Music/4_stage.mp3");
	audio->preloadBackgroundMusic("Sounds/mp3/Music/5_stage.mp3");
	audio->preloadBackgroundMusic("Sounds/mp3/Music/6_stage.mp3");
	audio->preloadBackgroundMusic("Sounds/mp3/Music/7_stage.mp3");
	audio->preloadBackgroundMusic("Sounds/mp3/Music/8_stage.mp3");
	audio->preloadBackgroundMusic("Sounds/mp3/Music/game_lose.mp3");
	audio->preloadBackgroundMusic("Sounds/mp3/Music/game_win.mp3");
	audio->preloadBackgroundMusic("Sounds/mp3/Music/intro.mp3");
	audio->preloadBackgroundMusic("Sounds/mp3/Music/Skill_stat.mp3");
	audio->preloadBackgroundMusic("Sounds/mp3/Music/bossSelection.mp3");
	audio->preloadBackgroundMusic("Sounds/mp3/Music/traitor_or_loyalist.mp3");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	audio->preloadEffect("Sounds/ogg/Effects/arrows.ogg");
	audio->preloadEffect("Sounds/ogg/Effects/fire_ball.ogg");
	audio->preloadEffect("Sounds/ogg/Effects/hurricane.ogg");
	audio->preloadEffect("Sounds/ogg/Effects/ice_spear.ogg");
	audio->preloadEffect("Sounds/ogg/Effects/lightning.ogg");
	audio->preloadEffect("Sounds/ogg/Effects/Mass_unit_spawn.ogg");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	audio->preloadEffect("Sounds/wav/Effects/arrows.wav");
	audio->preloadEffect("Sounds/wav/Effects/fire_ball.wav");
	audio->preloadEffect("Sounds/wav/Effects/hurricane.wav");
	audio->preloadEffect("Sounds/wav/Effects/ice_spear.wav");
	audio->preloadEffect("Sounds/wav/Effects/lightning.wav");
	audio->preloadEffect("Sounds/wav/Effects/Mass_unit_spawn.wav");
	audio->preloadEffect("Sounds/wav/Effects/Gold_Use_sound.wav");
	audio->preloadEffect("Sounds/wav/Effects/archer_man_attack_sound.wav");
	audio->preloadEffect("Sounds/wav/Effects/lightning_wizard_attack.wav");
	audio->preloadEffect("Sounds/wav/Effects/horse_man_spawn.wav");
	audio->preloadEffect("Sounds/wav/Effects/ice_wizard_attack.wav");

#endif

    // create a scene. it's an autorelease object
	auto scene = SplashScene::createScene();

    // run
    director->runWithScene(TransitionFade::create(1.0f, scene));

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();


	auto audio  = SimpleAudioEngine::getInstance();
    // if you use SimpleAudioEngine, it must be pause
	audio->pauseBackgroundMusic();
	audio->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

	auto audio = SimpleAudioEngine::getInstance();
    // if you use SimpleAudioEngine, it must resume here
	audio->resumeBackgroundMusic();
	audio->resumeAllEffects();
}
