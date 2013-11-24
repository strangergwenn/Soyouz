/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __DirLightActor_H
#define __DirLightActor_H

#include "Engine/actor.hpp"

class Game;


/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class DirLightActor : public Actor
{

public:
	
	/**
	 * @brief Create a directional light
	 * @param g				Game data
	 * @param name			Name
	 * @param color			Light color
	 * @param range			Light range
	 **/
	DirLightActor(Game* g, String name, Vector3 color, float range);
	
	/**
	 * @brief Delete a light
	 **/
	virtual ~DirLightActor();


protected:

	Ogre::Light* mLight;

};

#endif /* __DirLightActor_H */

