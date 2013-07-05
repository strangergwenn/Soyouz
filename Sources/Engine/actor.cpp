/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/actor.h"


/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

Actor::Actor(SceneManager* pscene, String name)
{
	mScene = pscene;
	mNode = mScene->getRootSceneNode()->createChildSceneNode(name);
}


/*----------------------------------------------
	Methods
----------------------------------------------*/

Actor::Actor(SceneManager* pscene, String name, String file)
{
	mScene = pscene;
	mNode = mScene->getRootSceneNode()->createChildSceneNode(name);
	mMesh = mScene->createEntity(name, file);
	mNode->attachObject(mMesh);
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

