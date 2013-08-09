/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __DEMOWORLD_H_
#define __DEMOWORLD_H_

#include "Engine/ogre.hpp"
#include "Engine/game.hpp"
#include "Engine/actor.hpp"
#include "Engine/player.hpp"
#include "Engine/meshactor.hpp"
#include "Engine/lightactor.hpp"
#include "Game/demoPlayer.hpp"


/*----------------------------------------------
Class definitions
----------------------------------------------*/

class DemoWorld : public Game
{
	
	void construct()
	{
		// Airlock
		MeshActor* act2 = new MeshActor(this, "DemoMesh", "Airlock.mesh", "MI_Airlock", true, 0.0f);
		act2->setLocation(Vector3(-256,-128,0));
		act2->setRotation(Vector3(0,90,0));
		
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
		
		// Wood crates
		MeshActor* crate = new MeshActor(this, "crate", "crate.mesh", "MI_Crate", true, 1.0f);
		crate->setLocation(Vector3(92, 64, -96));
		MeshActor* crate2 = new MeshActor(this, "crate2", "crate.mesh", "MI_Crate", true, 0.0f);
		crate2->setLocation(Vector3(64, -64, -96));
		
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
		LightActor* left = new LightActor(this, "left", Vector3(0.2f, 0.4f, 0.9f), 2000);
		left->translate(Vector3(-100, 100, 100));
		LightActor* center = new LightActor(this, "center", Vector3(0.5f, 0.5f, 0.5f), 2000);
		center->translate(Vector3(0, 100, 100));
		LightActor* right = new LightActor(this, "right", Vector3(0.9f, 0.7f, 0.2f), 2000);
		right->translate(Vector3(100, 100, 100));
	}

	void destruct(){}

	
	void setupPlayer()
	{
		mPlayer = new DemoPlayer(this, "LocalPlayer");
	}
	
};


#endif /* __DEMOWORLD_H_ */
