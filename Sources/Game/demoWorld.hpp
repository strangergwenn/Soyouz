/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __DEMOWORLD_H_
#define __DEMOWORLD_H_

#include "Engine/world.hpp"
#include "Engine/actor.hpp"
#include "Engine/player.hpp"


/*----------------------------------------------
Class definitions
----------------------------------------------*/

class DemoWorld : public World
{

public:
	
	void construct()
	{
		Actor act = Actor(this, "DemoMesh", "Airlock.mesh", "MI_Airlock");
		Light *light = mScene->createLight("light");
		/*
		Plane plane(Vector3::UNIT_Y, 0);
		MeshManager::getSingleton().createPlane("ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 512, 512, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
		Entity* entGround = mScene->createEntity("GroundEntity", "ground");
		SceneNode* groundNode = mScene->getRootSceneNode()->createChildSceneNode();
		groundNode->attachObject(entGround);
		groundNode->setPosition(0, -128, 0);
		
		Plane plane2(Vector3::UNIT_Z, 0);
		MeshManager::getSingleton().createPlane("wall", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane2, 512, 512, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
		Entity* entWall = mScene->createEntity("WallEntity", "wall");
		SceneNode* wallNode = mScene->getRootSceneNode()->createChildSceneNode();
		wallNode->attachObject(entWall);
		wallNode->setPosition(0, 0, -128);*/

		light->setDiffuseColour(1.0, 1.0, 1.0);
		light->setDiffuseColour(1.0, 1.0, 1.0);
		light->setType(Light::LT_POINT);
		light->setAttenuation(500, 0.1, 0.007, 0);
		light->setCastShadows(true);
		
		mPlayer->attachObject(light);
	}

	void destruct(){}
	
};


#endif /* __DEMOWORLD_H_ */
