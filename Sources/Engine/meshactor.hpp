/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwenna�l ARBONA
**/

#ifndef __MESHACTOR_H_
#define __MESHACTOR_H_

#include "Engine/game.hpp"
#include "Engine/actor.hpp"

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
	 * @param file			Material name to use
	 * @param bCastShadows	Should cast shadows
	 **/
	MeshActor(Game* g, String name, String file, String material, bool bCastShadows);
	
	/**
	 * @brief Delete an actor
	 **/
	~MeshActor();
	
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

	Entity* mMesh;

};

#endif /* __MESHACTOR_H_ */