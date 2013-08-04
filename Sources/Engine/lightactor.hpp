/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __LightActor_H
#define __LightActor_H

#include "Engine/game.hpp"
#include "Engine/actor.hpp"

class Game;


/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class LightActor : public Actor
{

public:
	
	/**
	 * @brief Create a light
	 * @param g				Game data
	 * @param name			Name
	 * @param color			Light color
	 * @param range			Light range
	 **/
	LightActor(Game* g, String name, Vector3 color, float range);
	
	/**
	 * @brief Delete a light
	 **/
	virtual ~LightActor();


protected:

	Light* mLight;

};

#endif /* __LightActor_H */

