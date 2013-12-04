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
#include "Engine/bulletphysics.hpp"
#include "Engine/gametypes.hpp"

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
	virtual void tick(const Ogre::FrameEvent& evt);

	/**
	 * @brief Prepare mesh normals for loading
	 * @param name			Mesh file
	 **/
	void prepareLoad(Ogre::String name);

	/**
	 * @brief Set the the actor location in the world
	 * @param offset		Location vector
	 **/
	virtual void setLocation(Vector3 newPos);

	/**
	 * @brief Set the the actor rotation in the world
	 * @param offset		Rotation vector
	 **/
	virtual void setRotation(Quaternion newRot);

	/**
	 * @brief Set the the actor linear velocity in the world
	 * @param offset		linear velocit vector
	 **/
	virtual void setSpeed(Vector3 newSpeed);

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
	virtual void rotate(Quaternion rotator);
	
	/**
	 * @brief Apply a physical force
	 * @param force			Force data
	 * @param location		Force relative location
	 **/
	void applyForce(Vector3 force, Vector3 location);

	/**
	 * @brief Apply a physical force in the local referencial
	 * @param force			Force data
	 * @param location		Force relative location
	 **/
	void applyLocalForce(Vector3 force, Vector3 location);
	
	/**
	 * @brief Remove all physical forces
	 **/
	void clearForces();
	
	/**
	 * @brief Get the current speed
	 * @return a vector materializing the speeds along X, Y, Z
	 **/
	Vector3 getSpeed(void);
	
	/**
	 * @brief Get the current speed in local reference
	 * @return a vector materializing the speeds along X, Y, Z
	 **/
	Vector3 getLocalSpeed(void);

	/**
	 * @brief Get the current angular speed
	 * @return a vector materializing the angular speeds along X, Y, Z
	 **/
	Vector3 getAngularSpeed(void);
	
	/**
	 * @brief Get the current angular speed in local reference
	 * @return a vector materializing the angular speeds along X, Y, Z
	 **/
	Vector3 getLocalAngularSpeed(void);
	
	/**
	 * @brief Get a relative position vector in ship in global reference
	 * @return a vector materializing position along X, Y, Z
	 **/
	Vector3 getGlobalPosition(Vector3 position);

	Quaternion getRotation();

	Vector3 getLocation();
	
	/**
	 * @brief Set a material
	 * @param name			Material path
	 **/
	void setMaterial(String name);
	
	/**
	 * @brief Set a material parameter
	 * @param index			Parameter index
	 * @param val			Parameter value
	 **/
	void setMaterialParam(int index, Real val);
	
	/**
	 * @brief Set a material parameter
	 * @param index			Parameter index
	 * @param val			Parameter value
	 **/
	void setMaterialParam(int index, Vector3 val);
	
	/**
	 * @brief Set a material parameter
	 * @param index			Parameter index
	 * @param val			Parameter value
	 **/
	void setMaterialParam(int index, Vector4 val);


protected: 

	/**
	 * @brief Setup the collision data
	 * @param mass			Mass to set
	 **/
	void generateCollisions(float mass);

	/**
	 * @brief Generate a hull mesh from the OGRE mesh
	 * @param bOptimize		Set to true to enable hull reduction
	 * @return a hull mesh for Bullet
	 **/
	void getCollisionMesh(bool bOptimize = false);
	
	
	// Physics data
	btScalar mPhysMass;
	btRigidBody* mPhysBody;
	btTransform mPhysTransform;
	btConvexShape* mPhysShape;
	btDefaultMotionState* mPhysMotionState;

	// Game data
	String mMaterialName;
	Ogre::Entity* mMesh;

};

#endif /* __MESHACTOR_H_ */
