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
	scene = pscene;
	node = scene->getRootSceneNode()->createChildSceneNode(name);
}


/*----------------------------------------------
	Methods
----------------------------------------------*/

Actor::Actor(SceneManager* pscene, String name, String file)
{
	scene = pscene;
	node = scene->getRootSceneNode()->createChildSceneNode(name);
	mesh = scene->createEntity(name, file);
	node->attachObject(mesh);
}


void Actor::Translate(Vector3 offset, bool bRelative)
{
	node->translate(offset[0], offset[0], offset[0], bRelative ? Node::TS_LOCAL : Node::TS_WORLD);
}


void Actor::Rotate(Vector3 rotator)
{
	node->pitch(Degree(rotator[0]));
	node->yaw(Degree(rotator[1]));
	node->roll(Degree(rotator[2]));
}
