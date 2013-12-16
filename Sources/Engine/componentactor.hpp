/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Frédéric BERTOLUS
**/

#ifndef __COMPONENTACTOR_H_
#define __COMPONENTACTOR_H_

#include "Engine/game.hpp"
#include "Engine/actor.hpp"
#include "Engine/bulletphysics.hpp"
#include "Engine/gametypes.hpp"

class Game;


/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class ComponentActor : public Actor
{

public:
	
	/**
	 * @brief Create an actor
	 * @param g				Game actor
	 * @param name			Unique name to set to the mesh
	 **/
	ComponentActor(Game* g, String name);
	
	/**
	 * @brief Create an actor
	 * @param g				Game actor
	 * @param name			Unique name to set to the mesh
	 * @param file			Mesh file to load
	 **/
	ComponentActor(Game* g, String name, String file);

	/**
	 * @brief Create an actor
	 * @param g				Game actor
	 * @param name			Unique name to set to the mesh
	 * @param file			Mesh file to load
	 * @param file			Material name to use
	 **/
	ComponentActor(Game* g, String name, String file, String material);
	
	/**
	 * @brief Create an actor
	 * @param g				Game actor
	 * @param name			Unique name to set to the mesh
	 * @param file			Mesh file to load
	 * @param material		Material name to use
	 * @param bCastShadows	Should cast shadows
	 **/
	ComponentActor(Game* g, String name, String file, String material, bool bCastShadows);
	

	/**
	 * @brief Delete an actor
	 **/
	virtual ~ComponentActor();
	
	/**
	 * @brief Main tick event
	 * @param evt			Frame event
	 **/
	virtual void tick(const Ogre::FrameEvent& evt);
	
	/**
	 * @brief Set a new mesh from file name
	 * @param name			Mesh file
	 **/
	void setModel(Ogre::String file);
	
	void setCastShadows(bool bCastShadows);

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

	/**
	 * @brief Generate a hull mesh from the OGRE mesh
	 * @param bOptimize		Set to true to enable hull reduction
	 * @return a hull mesh for Bullet
	 **/
	virtual btCollisionShape* getCollisionMesh(bool bOptimize = false);

protected: 

	
	

	
	// Physics data
	btTransform mLocalPhysTransform;
	btCollisionShape* mCollisionMeshCache;

	// Game data
	String mMaterialName;
	Ogre::Entity* mMesh;
	bool mCastShadow;
};

#endif /* __COMPONENTACTOR_H_ */
