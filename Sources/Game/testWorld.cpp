/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Game/testWorld.h"
#include "Game/testPlayer.h"

#define SIZE 4
#define RES Real(512)
#define SCALE Real(0.01)


/*----------------------------------------------
	Methods
----------------------------------------------*/

void TestWorld::construct()
{
	mScene->setAmbientLight(Ogre::ColourValue(1, 1, 1));
	Actor penguin = Actor(this, "penguin", "penguin.mesh");

	for (int i = -SIZE; i < SIZE; i++)
	{
		for (int j = -SIZE; j < SIZE; j++)
		{
			for (int k = -SIZE; k < SIZE; k++)
			{
				std::ostringstream ss;
				ss << "point" << i << "-" << j << "-" << k;
				Entity* ent = mScene->createEntity(ss.str() , Ogre::SceneManager::PT_SPHERE);
				SceneNode* node = mScene->getRootSceneNode()->createChildSceneNode();
				node->setPosition(i * RES, j * RES, k * RES);
				node->setScale(SCALE, SCALE, SCALE);
				node->attachObject(ent);
			}
		}
	}

}


void TestWorld::destruct()
{
}


void TestWorld::setupPlayer()
{
	mPlayer = new TestPlayer(this, "LocalPlayer", mScene);
}
