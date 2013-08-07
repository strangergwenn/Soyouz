/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __MESHACTOR_H_
#define __MESHACTOR_H_

#include "Engine/game.hpp"
#include "Engine/actor.hpp"
#include "Engine/bullet.hpp"

class Game;


/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class MeshActor : public Actor
{

public:
	
	/**
	 * @brief Create an actor
	 * @param g				Game actor
	 * @param name			Unique name to set to the mesh
	 * @param file			Mesh file to load
	 **/
	MeshActor(Game* g, String name, String file);

	/**
	 * @brief Create an actor
	 * @param g				Game actor
	 * @param name			Unique name to set to the mesh
	 * @param file			Mesh file to load
	 * @param file			Material name to use
	 **/
	MeshActor(Game* g, String name, String file, String material);
	
	/**
	 * @brief Create an actor
	 * @param g				Game actor
	 * @param name			Unique name to set to the mesh
	 * @param file			Mesh file to load
	 * @param material		Material name to use
	 * @param bCastShadows	Should cast shadows
	 **/
	MeshActor(Game* g, String name, String file, String material, bool bCastShadows);
	
	/**
	 * @brief Create an actor
	 * @param g				Game actor
	 * @param name			Unique name to set to the mesh
	 * @param file			Mesh file to load
	 * @param material		Material name to use
	 * @param bCastShadows	Should cast shadows
	 * @param mass			Physics mass
	 **/
	MeshActor(Game* g, String name, String file, String material, bool bCastShadows, float mass);
	
	/**
	 * @brief Delete an actor
	 **/
	~MeshActor();
	
	/**
	 * @brief Main tick event
	 * @param evt			Frame event
	 **/
	virtual void tick(const FrameEvent& evt);
	/**
	 * @brief Set the the actor location in the world
	 * @param offset		Location vector
	 **/
	virtual void setLocation(Vector3 newPos);

	/**
	 * @brief Translate the actor in the world
	 * @param offset		Movement vector
	 * @param bRelative		true if relative to the node
	 **/
	virtual void translate(Vector3 offset, bool bRelative = true);
	
	/**
	 * @brief Rotate the actor in the world
	 * @param offset		Rotation vector (pitch, yaw, roll) in degrees
	 **/
	virtual void rotate(Vector3 rotator);
	
	/**
	 * @brief Set a material
	 * @param name			Material path
	 **/
	void setMaterial(String name);
	
	/**
	 * @brief Set a material parameter
	 * @param name			Parameter name
	 * @param val			Parameter value
	 **/
	void setMaterialParam(String name, Real val);
	
	/**
	 * @brief Set a material parameter
	 * @param name			Parameter name
	 * @param val			Parameter value
	 **/
	void setMaterialParam(String name, Vector3 val);
	
	/**
	 * @brief Set a material parameter
	 * @param name			Parameter name
	 * @param val			Parameter value
	 **/
	void setMaterialParam(String name, Vector4 val);


protected: 

	
	// Physics data
	btScalar mPhysMass;
	btRigidBody* mPhysBody;
	btScalar mPhysMatrix[16];
	btTransform mPhysTransform;
	btCollisionShape* mPhysShape;
	btDefaultMotionState* mPhysMotionState;

	// Game data
	Entity* mMesh;

};

#endif /* __MESHACTOR_H_ */
