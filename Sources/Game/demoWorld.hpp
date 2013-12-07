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
#include "Engine/meshactor.hpp"
#include "Engine/lightactor.hpp"
#include "Game/demoPlayer.hpp"

#include "Engine/Rendering/renderer.hpp"


/*----------------------------------------------
Class definitions
----------------------------------------------*/

class DemoWorld : public Game
{
	
	void construct()
	{
		// Collision crate
		MeshActor* crate = new MeshActor(this, "crate", "crate.mesh", "MI_Crate", true, 1.0f);
		crate->setLocation(Vector3(0, 0, -50));
		
		// Earth
		Ogre::Real distance = 2000000;
		Ogre::Real scale = 30000;
		Ogre::SceneNode* mSceneNode = mScene->getRootSceneNode()->createChildSceneNode();
		Ogre::Entity* mEntity = mScene->createEntity("sphere1", Ogre::SceneManager::PT_SPHERE);
		mEntity->setMaterialName("Default");
		mEntity->setCastShadows(true);
		mSceneNode->attachObject(mEntity);
		mSceneNode->setScale(scale, scale, scale);
		mSceneNode->setPosition(Vector3(0, -distance, 0));
				
		// zCrate
		Ogre::SceneNode* mSceneNode2 = mScene->getRootSceneNode()->createChildSceneNode();
		Ogre::Entity* mEntity2 = mScene->createEntity("zcrate", Ogre::SceneManager::PT_CUBE);
		mEntity2->setMaterialName("MI_zCrate");
		mEntity2->setCastShadows(true);
		mSceneNode2->attachObject(mEntity2);
		mSceneNode2->setPosition(Vector3(-128, -64, -64));


		mScene->setSkyBox(true, "Sky");

		// Light
		Ogre::Light* l1 = mScene->createLight();
        l1->setType(Ogre::Light::LT_DIRECTIONAL);
        l1->setDiffuseColour(1.0f, 0.9f, 0.5f);
        l1->setSpecularColour(1.0f, 0.9f, 0.5f);
		l1->setDirection(1, -0.5f, -0.2f);
		l1->setCastShadows(false);
	}

	void destruct(){}

	
	void setupPlayer()
	{
		mPlayer = new DemoPlayer(this, "LocalPlayer");
	}

};

#endif /* __DEMOWORLD_H_ */
