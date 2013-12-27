/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __EDITOR_H_
#define __EDITOR_H_

#include "Engine/game.hpp"
#include "Engine/player.hpp"
#include "Engine/meshactor.hpp"


/*----------------------------------------------
Class definitions
----------------------------------------------*/

class Editor : public Game
{
	
	void construct()
	{
		MeshActor* sphere = new MeshActor(this, "Sphere", "SM_Sphere.mesh", "Default");
		sphere->setScale(1);

		Ogre::Light* l1 = mScene->createLight();
        l1->setType(Ogre::Light::LT_DIRECTIONAL);
        l1->setDiffuseColour(1.95f, 1.95f, 1.95f);
        l1->setSpecularColour(1.95f, 1.95f, 1.95f);
		l1->setDirection(1, -0.5f, -0.2f);
		l1->setCastShadows(false);
	}

	void destruct(){}


	void setupPlayer()
	{
		Game::setupPlayer();
		//mPlayer->
	}

	
	void tick(const Ogre::FrameEvent& evt)
	{
		Game::tick(evt);
	}

};

#endif /* __EDITOR_H_ */
