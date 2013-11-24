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
		// Sun & space
		mScene->setSkyBox(true, "Skybox");
		Ogre::Light* sun = mScene->createLight("Sun");
		sun->setType(Ogre::Light::LT_DIRECTIONAL);
		sun->setDirection(Vector3(-0.2f, 0.1f, -1.0f));
		sun->setDiffuseColour(Ogre::ColourValue(0.40f, 0.30f, 0.10f));
		sun->setSpecularColour(Ogre::ColourValue(0.40f, 0.30f, 0.10f));
		
		// Back sun
		Ogre::Light* sun2 = mScene->createLight("Sun2");
		sun2->setType(Ogre::Light::LT_DIRECTIONAL);
		sun2->setDirection(Vector3(0.2f, -0.1f, 1.0f));
		sun2->setDiffuseColour(Ogre::ColourValue(0.10f, 0.10f, 0.20f));
		sun2->setSpecularColour(Ogre::ColourValue(0.20f, 0.20f, 0.20f));

		// Collision crate
		MeshActor* crate = new MeshActor(this, "crate", "crate.mesh", "MI_Crate", true, 1.0f);
		crate->setLocation(Vector3(0, 0, -50));
		
		// Airlock
		//MeshActor* act2 = new MeshActor(this, "DemoMesh", "Airlock.mesh", "MI_Airlock", true, 0.0f);
		//act2->setLocation(Vector3(-256,-128,0));
		//act2->setRotation(Vector3(0,90,0));
		
		// zCrate
		Ogre::SceneNode* mSceneNode2 = mScene->getRootSceneNode()->createChildSceneNode();
		Ogre::Entity* mEntity2 = mScene->createEntity("zcrate", Ogre::SceneManager::PT_CUBE);
		mEntity2->setMaterialName("MI_zCrate");
		mEntity2->setCastShadows(true);
		mSceneNode2->attachObject(mEntity2);
		mSceneNode2->setPosition(Vector3(-128, -64, -64));
	}

	void destruct(){}

	
	void setupPlayer()
	{
		mPlayer = new DemoPlayer(this, "LocalPlayer");
	}
	
};


#endif /* __DEMOWORLD_H_ */
