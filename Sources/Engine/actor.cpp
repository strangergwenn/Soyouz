/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/actor.hpp"


/*----------------------------------------------
	Definitions
----------------------------------------------*/

const Quaternion LEFT =			Quaternion(Radian(Degree(-90).valueRadians()), Vector3(0,1,0));
const Quaternion RIGHT =		Quaternion(Radian(Degree(+90).valueRadians()), Vector3(0,1,0));
const Quaternion TOP =			Quaternion(Radian(Degree(-90).valueRadians()), Vector3(1,0,0));
const Quaternion BOTTOM =		Quaternion(Radian(Degree(+90).valueRadians()), Vector3(1,0,0));
const Quaternion FORWARD =		Quaternion(Radian(Degree(180).valueRadians()), Vector3(0,1,0));
const Quaternion BACK =			Quaternion(Radian(Degree(  0).valueRadians()), Vector3(0,1,0));


/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

Actor::Actor(Game* g, String name)
{
	mGame = g;
	mName = name;
	mNode = g->createGameNode(name);
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
	Helpers
----------------------------------------------*/

Vector3 Actor::vectorFromString(Ogre::String vec)
{
	Ogre::vector<Ogre::String>::type vectorCoords = Ogre::StringUtil::split(vec);
	Vector3 result = Vector3(
		Ogre::StringConverter::parseReal(vectorCoords[0]),
		Ogre::StringConverter::parseReal(vectorCoords[1]),
		Ogre::StringConverter::parseReal(vectorCoords[2]));
	return result;
}


Quaternion Actor::quaternionFromString(Ogre::String quat)
{
	Quaternion result;
	if (quat == "LEFT")
		result = LEFT;
	else if (quat == "RIGHT")
		result = RIGHT;
	else if (quat == "TOP")
		result = TOP;
	else if (quat == "BOTTOM")
		result = BOTTOM;
	else if (quat == "FORWARD")
		result = FORWARD;
	else
		result = BACK;
	return result;
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
