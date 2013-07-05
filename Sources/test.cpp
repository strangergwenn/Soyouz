/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "test.h"


/*----------------------------------------------
	Methods
----------------------------------------------*/

void Test::Construct()
{	
	scene->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
	Actor penguin = Actor(scene, "penguin", "penguin.mesh");
}
