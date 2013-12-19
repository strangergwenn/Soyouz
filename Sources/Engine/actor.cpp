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
	mName = name;
	mNode = g->createGameNode();
	mGame->registerActor(this);
}


Actor::~Actor()
{
	mGame->deleteGameNode(mNode);
}


/*----------------------------------------------
	Methods
----------------------------------------------*/

void Actor::preTick(const Ogre::FrameEvent& evt)
{
}


void Actor::tick(const Ogre::FrameEvent& evt)
{
}


void Actor::attachObject(Ogre::MovableObject* obj)
{
	mNode->attachObject(obj);
}


void Actor::attachActor(Actor* obj)
{
	Ogre::SceneNode* target = obj->getNode();
	gameLog("Actor::attachActor " + target->getName() + " to " + mNode->getName());
	target->getParent()->removeChild(target);
	mNode->addChild(target);
}


void Actor::setLocation(Vector3 newPos)
{
	mNode->setPosition(newPos);
}


void Actor::setRotation(Quaternion newRot)
{
	mNode->setOrientation(newRot);
}


void Actor::setScale(float scale)
{
	mNode->setScale(scale, scale, scale);
}


void Actor::translate(Vector3 offset, bool bRelative)
{
	mNode->translate(offset[0], offset[1], offset[2], bRelative ? Ogre::Node::TS_LOCAL : Ogre::Node::TS_WORLD);
}


void Actor::rotate(Quaternion rotator)
{
	mNode->rotate(rotator);
}


/*----------------------------------------------
	Save
----------------------------------------------*/

void Actor::save()
{
	setSaveGroup("actor");
	saveValue(location(), "location");
	saveValue(rotation(), "rotation");
}


void Actor::load()
{
	setSaveGroup("actor");
}


String Actor::getFileName()
{
	return mName + Ogre::String(".xml");
}


/*----------------------------------------------
	Getters
----------------------------------------------*/

Vector3 Actor::location()
{
	return mNode->getPosition();
}


Quaternion Actor::rotation()
{
	return mNode->getOrientation();
}


Ogre::SceneNode* Actor::getNode()
{
	return mNode;
}


/*----------------------------------------------
	Debug facilities
----------------------------------------------*/

void Actor::gameLog(String text)
{
	mGame->gameLog(text);
}


void Actor::create3DHelper(void)
{
	Ogre::ManualObject* lineX = mGame->getDebugLine(Vector3(5,0,0), mName + "_DBGX", "Red");
	Ogre::ManualObject* lineY = mGame->getDebugLine(Vector3(0,5,0), mName + "_DBGY", "Green");
	Ogre::ManualObject* lineZ = mGame->getDebugLine(Vector3(0,0,5), mName + "_DBGZ", "Blue");
	mNode->attachObject(lineX);
	mNode->attachObject(lineY);
	mNode->attachObject(lineZ);
}
