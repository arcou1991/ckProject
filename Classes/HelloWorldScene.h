#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"


#define PTM_RATIO 32

class HelloWorld : public cocos2d::Layer
{
	b2World* world;

public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
	virtual void update(float dt);
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	void initPhysics();
	void addNewSpriteAtPosition(cocos2d::Vec2 p);
};

#endif // __HELLOWORLD_SCENE_H__
