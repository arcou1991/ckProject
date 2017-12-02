#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

CCScene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();
	// 'layer' is an autorelease object
	HelloWorld *layer = HelloWorld::create();
	// add layer as a child to scene
	scene->addChild(layer);
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 初始化物理引擎
	this->initPhysics();

	setTouchEnabled(true);

	// 设置为单点触摸
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

	// 开始游戏循环
	scheduleUpdate();
    return true;
}


void HelloWorld::initPhysics()
{
	Size s = Director::getInstance()->getVisibleSize();

	//重力参数
	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);

	//创建世界
	world = new b2World(gravity);

	//允许物体休眠
	world->SetAllowSleeping(true);

	//开始连续物理测试
	world->SetContinuousPhysics(true);

	//地面物体定义
	b2BodyDef groundBodyDef;

	//左下角
	groundBodyDef.position.Set(0.0f, 0.0f);

	//创建地面物体
	b2Body* groundBody = world->CreateBody(&groundBodyDef);

	//定义一个有边的形状
	b2EdgeShape groundBox;

	//底部
	groundBox.Set(b2Vec2(0, 0), b2Vec2(s.width / PTM_RATIO, 0));
	//使用夹具固定形状到物体上
	groundBody->CreateFixture(&groundBox, 0);

	//顶部
	groundBox.Set(b2Vec2(0, s.height / PTM_RATIO), b2Vec2(s.width / PTM_RATIO, s.height / PTM_RATIO));
	groundBody->CreateFixture(&groundBox, 0);

	//左边
	groundBox.Set(b2Vec2(0, s.height / PTM_RATIO), b2Vec2(0, 0));
	groundBody->CreateFixture(&groundBox, 0);

	//右边
	groundBox.Set(b2Vec2(s.width / PTM_RATIO, s.height / PTM_RATIO), b2Vec2(s.width / PTM_RATIO, 0));
	groundBody->CreateFixture(&groundBox, 0);
}


void HelloWorld::addNewSpriteAtPosition(Vec2 p)
{
	log("Add sprite %0.2f x %0.2f", p.x, p.y);

	// 创建物理引擎精灵对象
	auto sprite = Sprite::create("btn_world.png");
	sprite->setPosition(Vec2(p.x, p.y));
	this->addChild(sprite);

	// 物体定义
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(p.x / PTM_RATIO, p.y / PTM_RATIO);
	b2Body *body = world->CreateBody(&bodyDef);
	body->SetUserData(sprite);

	// 定义2米见方的盒子形状
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1, 1);

	//夹具定义
	b2FixtureDef fixtureDef;

	//设置夹具的形状
	fixtureDef.shape = &dynamicBox;

	//设置密度
	fixtureDef.density = 1.0f;

	//设置摩擦系数
	fixtureDef.friction = 0.3f;

	//使用夹具固定形状到物体上
	body->CreateFixture(&fixtureDef);
}


void HelloWorld::update(float dt)
{
	float timeStep = 0.03f;
	int32 velocityIterations = 8;
	int32 positionIterations = 1;

	world->Step(timeStep, velocityIterations, positionIterations);

	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != nullptr)
		{
			Sprite* sprite = (Sprite*)b->GetUserData();
			sprite->setPosition(Vec2(b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
			sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}

}
