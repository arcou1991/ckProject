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

	// ��ʼ����������
	this->initPhysics();

	setTouchEnabled(true);

	// ����Ϊ���㴥��
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

	// ��ʼ��Ϸѭ��
	scheduleUpdate();
    return true;
}


void HelloWorld::initPhysics()
{
	Size s = Director::getInstance()->getVisibleSize();

	//��������
	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);

	//��������
	world = new b2World(gravity);

	//������������
	world->SetAllowSleeping(true);

	//��ʼ�����������
	world->SetContinuousPhysics(true);

	//�������嶨��
	b2BodyDef groundBodyDef;

	//���½�
	groundBodyDef.position.Set(0.0f, 0.0f);

	//������������
	b2Body* groundBody = world->CreateBody(&groundBodyDef);

	//����һ���бߵ���״
	b2EdgeShape groundBox;

	//�ײ�
	groundBox.Set(b2Vec2(0, 0), b2Vec2(s.width / PTM_RATIO, 0));
	//ʹ�üо߹̶���״��������
	groundBody->CreateFixture(&groundBox, 0);

	//����
	groundBox.Set(b2Vec2(0, s.height / PTM_RATIO), b2Vec2(s.width / PTM_RATIO, s.height / PTM_RATIO));
	groundBody->CreateFixture(&groundBox, 0);

	//���
	groundBox.Set(b2Vec2(0, s.height / PTM_RATIO), b2Vec2(0, 0));
	groundBody->CreateFixture(&groundBox, 0);

	//�ұ�
	groundBox.Set(b2Vec2(s.width / PTM_RATIO, s.height / PTM_RATIO), b2Vec2(s.width / PTM_RATIO, 0));
	groundBody->CreateFixture(&groundBox, 0);
}


void HelloWorld::addNewSpriteAtPosition(Vec2 p)
{
	log("Add sprite %0.2f x %0.2f", p.x, p.y);

	// �����������澫�����
	auto sprite = Sprite::create("btn_world.png");
	sprite->setPosition(Vec2(p.x, p.y));
	this->addChild(sprite);

	// ���嶨��
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(p.x / PTM_RATIO, p.y / PTM_RATIO);
	b2Body *body = world->CreateBody(&bodyDef);
	body->SetUserData(sprite);

	// ����2�׼����ĺ�����״
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1, 1);

	//�о߶���
	b2FixtureDef fixtureDef;

	//���üоߵ���״
	fixtureDef.shape = &dynamicBox;

	//�����ܶ�
	fixtureDef.density = 1.0f;

	//����Ħ��ϵ��
	fixtureDef.friction = 0.3f;

	//ʹ�üо߹̶���״��������
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
