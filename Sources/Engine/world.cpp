/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/world.h"


/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

World::World()
{
	scene = mSceneMgr;
}


/*----------------------------------------------
	Methods
----------------------------------------------*/

void World::createScene()
{	
	Construct();
}


void World::Construct()
{
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
	Actor penguin = Actor(mSceneMgr, "penguin", "penguin.mesh");
}
