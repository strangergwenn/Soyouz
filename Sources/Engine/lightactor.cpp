/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/ogre.hpp"
#include "Engine/lightactor.hpp"


/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

LightActor::LightActor(Game* g, String name, Vector3 color, float range) : Actor(g, name)
{
	mLight = g->getScene()->createLight(name);
	mLight->setType(Light::LT_POINT);
	mLight->setCastShadows(true);
		
	mLight->setDiffuseColour(color[0], color[1], color[2]);
	mLight->setSpecularColour(color[0], color[1], color[2]);
	mLight->setAttenuation(range, 0.5f, 0.005f, 0.00005f);

	attachObject(mLight);
}

LightActor::~LightActor()
{
}
