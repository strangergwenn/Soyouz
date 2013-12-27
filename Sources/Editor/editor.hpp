/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __EDITOR_H_
#define __EDITOR_H_

#include "Engine/game.hpp"
#include "Engine/meshactor.hpp"


/*----------------------------------------------
Class definition
----------------------------------------------*/

class Editor : public Game
{

public:
	
	/**
	 * @brief Rotate lights
	 * @param yaw				Yaw component
	 * @param pitch				Pitch component
	 **/
	void rotateLights(Ogre::Degree yaw, Ogre::Degree pitch);
	
	/**
	 * @brief Step the material list
	 * @param bRight			True if toward the right side
	 **/
	void step(bool bRight);


protected:
	
	/**
	 * @brief Level construction
	 **/
	void construct();
	
	/**
	 * @brief Level destruction
	 **/
	void destruct(){}
	
	/**
	 * @brief Setup the player
	 **/
	void setupPlayer();


protected:
	
	// Scene settings
	float mSphereGap;
	int mMaterialIndex;

	// Scene content
	Ogre::Light* mLight;
	Ogre::vector<MeshActor*>::type mSpheres;
	

};

#endif /* __EDITOR_H_ */
