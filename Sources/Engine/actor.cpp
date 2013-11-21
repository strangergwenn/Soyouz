/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/actor.hpp"


/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

Actor::Actor(Game* g, String name)
{
	mGame = g;
	mNode = g->createGameNode(name);
	mGame->registerActor(this);
}


Actor::~Actor()
{
	mGame->unregisterActor(this);
	delete mNode;
}


/*----------------------------------------------
	Methods
----------------------------------------------*/

void Actor::preTick(const FrameEvent& evt)
{
}


void Actor::tick(const FrameEvent& evt)
{
}


void Actor::attachObject(MovableObject* obj)
{
	mNode->attachObject(obj);
}


void Actor::attachActor(Actor* obj)
{
	SceneNode* target = obj->getNode();
	gameLog("Actor::attachActor " + target->getName() + " to " + mNode->getName());
	target->getParent()->removeChild(target);
	mNode->addChild(target);
}


void Actor::setLocation(Vector3 newPos)
{
	mNode->setPosition(newPos);
}


void Actor::setRotation(Vector3 newRot)
{
	mNode->setOrientation(0, newRot[0], newRot[1], newRot[2]);
}


void Actor::setScale(float scale)
{
	mNode->setScale(scale, scale, scale);
}


void Actor::translate(Vector3 offset, bool bRelative)
{
	mNode->translate(offset[0], offset[1], offset[2], bRelative ? Node::TS_LOCAL : Node::TS_WORLD);
}


void Actor::rotate(Vector3 rotator)
{
	mNode->pitch(Degree(rotator[0]));
	mNode->yaw(Degree(rotator[1]));
	mNode->roll(Degree(rotator[2]));
}


/*----------------------------------------------
	Getters
----------------------------------------------*/

void Actor::gameLog(String text)
{
	mGame->gameLog(text);
}

Vector3 Actor::location()
{
	return mNode->getPosition();
}


Vector3 Actor::rotation()
{
	Quaternion rot = mNode->getOrientation();
	return Vector3(
		Radian(rot.getPitch()).valueDegrees(),
		Radian(rot.getYaw()).valueDegrees(),
		Radian(rot.getRoll()).valueDegrees());
}


SceneNode* Actor::getNode()
{
	return mNode;
}
