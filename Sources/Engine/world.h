/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __WORLD_H_
#define __WORLD_H_

#include "Engine/engine.h"
#include "Engine/actor.h"


/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class World : public ExampleApplication
{
public:
	
	/**
	 * @brief World constructor
	 **/
	World();
	
	/**
	 * @brief Scene definition
	 **/
	void createScene();

	/**
	 * @brief World definition
	 **/
	void Construct();


private: 

	SceneManager* scene;

};

#endif /* __WORLD_H_ */
