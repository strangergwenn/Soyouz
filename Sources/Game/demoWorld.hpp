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
		// Earth and sky
		earth = new MeshActor(this, "BackgroundPlanet", "SM_Planet.mesh", "MI_Earth");
		earth->setRotation(Quaternion(Radian(Degree(-90).valueRadians()), Vector3(1,0,0)));
		earth->setLocation(Vector3(0, -2000000, 0));
		earth->setScale(1500);
		mScene->setSkyBox(true, "Sky");

		// Sun
		Ogre::Light* l1 = mScene->createLight();
        l1->setType(Ogre::Light::LT_DIRECTIONAL);
        l1->setDiffuseColour(1.95f, 1.95f, 1.95f);
        l1->setSpecularColour(1.95f, 1.95f, 1.95f);
		l1->setDirection(1, -0.5f, -0.2f);
		l1->setCastShadows(false);

		// Collision crate
		MeshActor* crate = new MeshActor(this, "crate", "crate.mesh", "MI_Crate", true, 1.0f);
		crate->setLocation(Vector3(0, 0, -50));
		crate->commit();
	}

	void destruct(){}

	
	void tick(const Ogre::FrameEvent& evt)
	{
		Game::tick(evt);
		
		earth->rotate(Quaternion(Radian(Degree(-0.1f * evt.timeSinceLastFrame).valueRadians()), Vector3(0,1,0)));
	}
	

	void setupPlayer()
	{
		mPlayer = new DemoPlayer(this, "LocalPlayer");
	}


	MeshActor* earth;

};

#endif /* __DEMOWORLD_H_ */
