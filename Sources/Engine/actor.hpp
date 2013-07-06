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
	 * @brief Create an actor
	 * @param w				World actor
	 * @param name			Unique name to set to the mesh
	 * @param file			Mesh file to load
	 * @param file			Material name to use
	 **/
	Actor(World* w, String name, String file, String material);
	
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
	 * @brief Attach something to this actor
	 * @param obj			Attached object
	 **/
	void attachObject(MovableObject* obj);

	/**
	 * @brief Set the the actor location in the world
	 * @param offset		Location vector
	 **/
	void setLocation(Vector3 newPos);

	/**
	 * @brief Translate the actor in the world
	 * @param offset		Movement vector
	 * @param bRelative		true if relative to the node
	 **/
	void translate(Vector3 offset, bool bRelative = true);
	
	/**
	 * @brief Rotate the actor in the world
	 * @param offset		Rotation vector (pitch, yaw, roll) in degrees
	 **/
	void rotate(Vector3 rotator);
	
	/**
	 * @brief Set a material
	 * @param name			Material path
	 **/
	void setMaterial(String name);


protected: 

	Vector3 mSpeed;

	World* world;
	Entity* mMesh;
	SceneNode* mNode;

};

#endif /* __ACTOR_H_ */

