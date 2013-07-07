/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Game/testWorld.hpp"
#include "Game/testPlayer.hpp"

#define SIZE 10
#define RES Real(64)
#define SCALE Real(0.05)


/*----------------------------------------------
	Methods
----------------------------------------------*/

void TestWorld::construct()
{
	mScene->setAmbientLight(Ogre::ColourValue(1, 1, 1));

	Actor* penguin = new Actor(this, "penguin", "penguin.mesh");
	penguin->translate(Vector3(0,0,-200));
	penguin->rotate(Vector3(0, 180, 0));
	mPlayer->attachActor(penguin);

	for (int i = -SIZE; i < SIZE; i++)
	{
		for (int j = -SIZE; j < SIZE; j++)
		{
			std::ostringstream ss;
			ss << "point" << i << "-" << j;
			Entity* ent = mScene->createEntity(ss.str() , Ogre::SceneManager::PT_SPHERE);
			SceneNode* node = mScene->getRootSceneNode()->createChildSceneNode();
			node->setPosition(4 * i * RES, 0, j * RES);
			node->setScale(SCALE, SCALE, SCALE);
			node->attachObject(ent);
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
