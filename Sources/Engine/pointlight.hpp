/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __POINTLIGHT_H
#define __POINTLIGHT_H

#include "Engine/game.hpp"
#include "Engine/actor.hpp"

class Game;


/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class PointLight : public Actor
{

public:
	
	/**
	 * @brief Create a light
	 * @param g				Game data
	 * @param name			Name
	 * @param color			Light color
	 * @param range			Light range
	 **/
	PointLight(Game* g, String name, SceneManager* scene, Vector3 color, float range);
	
	/**
	 * @brief Delete a light
	 **/
	virtual ~PointLight();


protected:

	Light* mLight;

};

#endif /* __POINTLIGHT_H */

