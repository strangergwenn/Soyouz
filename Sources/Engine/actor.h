/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __ACTOR_H_
#define __ACTOR_H_

#include "Engine/engine.h"


/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class Actor
{
public:
	
	/**
	 * @brief Get an actor
	 * @param pscene		Scene manager
	 * @param name			Unique name to set to the mesh
	 **/
	Actor(SceneManager* pscene, String name);

	/**
	 * @brief Get an actor
	 * @param pscene		Scene manager
	 * @param name			Unique name to set to the mesh
	 * @param file			Mesh file to load
	 **/
	Actor(SceneManager* pscene, String name, String file);

	/**
	 * @brief Translate the actor in the world
	 * @param offset		movement vector
	 * @param bRelative		true if relative to the node
	 **/
	void Translate(Vector3 offset, bool bRelative);
	
	/**
	 * @brief Rotate the actor in the world
	 * @param offset		rotation vector (pitch, yaw, roll)
	 **/
	void Rotate(Vector3 rotator);


private: 

	SceneManager* scene;
	SceneNode* node;
	Entity* mesh;

};

#endif /* __ACTOR_H_ */
