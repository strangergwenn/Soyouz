/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/actor.h"
#include "Engine/world.h"


/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

Actor::Actor(World* w, String name)
	: mMesh(NULL)
{
	mNode = w->createWorldNode(name);
}


Actor::Actor(World* w, String name, String file)
{
	mNode = w->createWorldNode(name);
	mMesh = w->createWorldEntity(name, file);
	mNode->attachObject(mMesh);
}


Actor::~Actor()
{
}


/*----------------------------------------------
	Methods
----------------------------------------------*/

Vector3 Actor::speed()
{
	return mSpeed;
}


void Actor::translate(Vector3 offset, bool bRelative)
{
	mNode->translate(offset[0], offset[0], offset[0], bRelative ? Node::TS_LOCAL : Node::TS_WORLD);
}


void Actor::rotate(Vector3 rotator)
{
	mNode->pitch(Degree(rotator[0]));
	mNode->yaw(Degree(rotator[1]));
	mNode->roll(Degree(rotator[2]));
}

