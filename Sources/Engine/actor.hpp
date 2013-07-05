/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __ACTOR_H_
#define __ACTOR_H_

#include "Engine/engine.hpp"

class World;


/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class Actor
{

public:
	
	/**
	 * @brief Create an actor
	 * @param w				World actor
	 * @param name			Unique name to set to the mesh
	 **/
	Actor(World* w, String name);

	/**
	 * @brief Create an actor
	 * @param w				World actor
	 * @param name			Unique name to set to the mesh
	 * @param file			Mesh file to load
	 **/
	Actor(World* w, String name, String file);
	
	/**
	 * @brief Delete an actor
	 **/
	~Actor();
	
	/**
	 * @brief Get the current speed
	 * @return the speed
	 **/
	Vector3 speed();

	/**
	 * @brief Translate the actor in the world
	 * @param offset		movement vector
	 * @param bRelative		true if relative to the node
	 **/
	void translate(Vector3 offset, bool bRelative = true);
	
	/**
	 * @brief Rotate the actor in the world
	 * @param offset		rotation vector (pitch, yaw, roll) in degrees
	 **/
	void rotate(Vector3 rotator);


protected: 

	Vector3 mSpeed;

	World* world;
	Entity* mMesh;
	SceneNode* mNode;

};

#endif /* __ACTOR_H_ */

