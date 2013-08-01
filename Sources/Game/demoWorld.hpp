/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __DEMOWORLD_H_
#define __DEMOWORLD_H_

#include "Engine/game.hpp"
#include "Engine/actor.hpp"
#include "Engine/player.hpp"
#include "Engine/pointlight.hpp"


/*----------------------------------------------
Class definitions
----------------------------------------------*/

class DemoWorld : public Game
{

public:
	
	void construct()
	{
		Actor* act2 = new Actor(this, "DemoMesh", "Airlock.mesh", "MI_Airlock");
		act2->translate(Vector3(-256,-128,0));
		act2->rotate(Vector3(0, 90, 0));
		
		Actor* penguin = new Actor(this, "penguin", "penguin.mesh", "Penguin");
		penguin->translate(Vector3(100,-100,-100));
		
		Actor* crate = new Actor(this, "crate", "crate.mesh", "MI_Crate");
		crate->translate(Vector3(-100,-100,100));
		
		PointLight* light = new PointLight(this, "Light", mScene, Vector3(0.9, 0.9, 0.9), 500);
		light->translate(Vector3(200, 200, -0));

		// Ground plane
		Plane plane(Vector3::UNIT_Y, 0);
		MeshManager::getSingleton().createPlane("ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 512, 512, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
		Entity* entGround = mScene->createEntity("GroundEntity", "ground");
		entGround->setMaterialName("Material");
		entGround->setCastShadows(false);
		SceneNode* groundNode = mScene->getRootSceneNode()->createChildSceneNode();
		groundNode->attachObject(entGround);
		groundNode->setPosition(0, -256, 0);
		
		// Wall plane
		Plane plane2(Vector3::UNIT_Z, 0);
		MeshManager::getSingleton().createPlane("wall", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane2, 512, 512, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
		Entity* entWall = mScene->createEntity("WallEntity", "wall");
		entWall->setMaterialName("Material");
		entWall->setCastShadows(false);
		SceneNode* wallNode = mScene->getRootSceneNode()->createChildSceneNode();
		wallNode->attachObject(entWall);
		wallNode->setPosition(0, 0,-256);

		// Sphere
		Ogre::SceneNode* mSceneNode = mScene->getRootSceneNode()->createChildSceneNode();
		Ogre::Entity* mEntity = mScene->createEntity("sphere1", Ogre::SceneManager::PT_SPHERE);
		mEntity->setMaterialName("MI_NormalGloss");
		mEntity->setCastShadows(true);
		mSceneNode->attachObject(mEntity);
		mSceneNode->setPosition(Ogre::Vector3(-100,-100,-100));

		// Sphere
		Ogre::SceneNode* mSceneNode2 = mScene->getRootSceneNode()->createChildSceneNode();
		Ogre::Entity* mEntity2 = mScene->createEntity("sphere2", Ogre::SceneManager::PT_SPHERE);
		mEntity2->setMaterialName("MI_Gloss");
		mEntity2->setCastShadows(true);
		mSceneNode2->attachObject(mEntity2);
		mSceneNode2->setPosition(Ogre::Vector3(-100,-100,10));
	}

	void destruct(){}
	
};


#endif /* __DEMOWORLD_H_ */
