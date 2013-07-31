/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/pointlight.hpp"


/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

PointLight::PointLight(Game* g, String name, SceneManager* scene, Vector3 color, float range) : Actor(g, name)
{
	mLight = scene->createLight(name);
	mLight->setType(Light::LT_POINT);
	mLight->setCastShadows(true);
		
	mLight->setDiffuseColour(color[0], color[1], color[2]);
	mLight->setSpecularColour(color[0], color[1], color[2]);
	mLight->setAttenuation(range, 1.0, 0.007, 0);

	attachObject(mLight);
}

PointLight::~PointLight()
{
}
