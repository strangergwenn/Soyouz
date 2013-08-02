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
		// Airlock
		Actor* act2 = new Actor(this, "DemoMesh", "Airlock.mesh", "MI_Airlock");
		act2->translate(Vector3(-256,-128,0));
		act2->rotate(Vector3(0, 90, 0));
		
		// zCrate
		Ogre::SceneNode* mSceneNode2 = mScene->getRootSceneNode()->createChildSceneNode();
		Ogre::Entity* mEntity2 = mScene->createEntity("zcrate", Ogre::SceneManager::PT_CUBE);
		mEntity2->setMaterialName("MI_zCrate");
		mEntity2->setCastShadows(true);
		mSceneNode2->attachObject(mEntity2);
		mSceneNode2->setPosition(Ogre::Vector3(-128, -64, -64));

		// Glossy sphere
		Ogre::SceneNode* mSceneNode = mScene->getRootSceneNode()->createChildSceneNode();
		Ogre::Entity* mEntity = mScene->createEntity("sphere1", Ogre::SceneManager::PT_SPHERE);
		mEntity->setMaterialName("MI_Gloss");
		mEntity->setCastShadows(true);
		mSceneNode->attachObject(mEntity);
		mSceneNode->setPosition(Ogre::Vector3(0, -64, -64));
		
		// Wood crate
		Actor* crate = new Actor(this, "crate", "crate.mesh", "MI_Crate");
		crate->translate(Vector3(64, -96, -96));
		
		// Exhaust cone
		Actor* cone = new Actor(this, "cone", "cone.mesh", "MI_Exhaust", false);
		cone->setMaterialParam("baseAlpha", 1.0);
		cone->translate(Vector3(256, -64, -64));
		
		// Smoke
		//ParticleSystem* ps = mScene->createParticleSystem("FX", "Smoke");
		//mScene->getRootSceneNode()->attachObject(ps);

		// Ground plane
		Plane plane(Vector3::UNIT_Y, 0);
		MeshManager::getSingleton().createPlane("ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 512, 512, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
		Entity* entGround = mScene->createEntity("GroundEntity", "ground");
		entGround->setMaterialName("Default");
		entGround->setCastShadows(false);
		SceneNode* groundNode = mScene->getRootSceneNode()->createChildSceneNode();
		groundNode->attachObject(entGround);
		groundNode->setPosition(0, -256, 0);
		
		// Wall plane
		Plane plane2(Vector3::UNIT_Z, 0);
		MeshManager::getSingleton().createPlane("wall", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane2, 512, 512, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
		Entity* entWall = mScene->createEntity("WallEntity", "wall");
		entWall->setMaterialName("Default");
		entWall->setCastShadows(false);
		SceneNode* wallNode = mScene->getRootSceneNode()->createChildSceneNode();
		wallNode->attachObject(entWall);
		wallNode->setPosition(0, 0,-256);
		
		// Lights
		PointLight* light = new PointLight(this, "Light", mScene, Vector3(0.2, 0.4, 0.9), 2000);
		light->translate(Vector3(200, 0, 200));
		PointLight* light2 = new PointLight(this, "Light2", mScene, Vector3(0.9, 0.7, 0.2), 2000);
		light2->translate(Vector3(0, 0, 200));
		PointLight* light3 = new PointLight(this, "Light3", mScene, Vector3(0.5, 0.5, 0.5), 2000);
		light2->translate(Vector3(200, 0, 0));
	}

	void destruct(){}
	
};


#endif /* __DEMOWORLD_H_ */
