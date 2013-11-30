/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __THRUSTER_H_
#define __THRUSTER_H_

#include "Engine/meshactor.hpp"

class Game;
class Ship;


/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class Thruster : public MeshActor
{

public:
	
	/**
	 * @brief Create an engine
	 * @param g				Game actor
	 * @param name			Unique name to set to the mesh
	 * @param parent		Ship to attach to
	 * @param location		Thruster position
	 * @param rotation		Thruster rotation
	 **/
	Thruster(Game* g, String name, MeshActor* parent, Vector3 location, Quaternion rotation);
	
	/**
	 * @brief Main tick event
	 * @param evt			Frame event
	 **/
	void tick(const Ogre::FrameEvent& evt);
	
	/**
	 * @brief Customize the thruster parameters
	 * @param strength		Maximum force that is applied
	 * @param scale			Exhaust 3D scale
	 * @param rotRatio		Fraction of the full power to use for rotations
	 **/
	void customize(float strength, float scale, float rotRatio = 0.1f);

	/**
	 * @brief Customize the thruster exhaust model
	 * @param scale			Exhaust model
	 **/
	void setModel(String file);


protected:

	Ship* mShip;
	Ogre::Light *mLight;
	
	Vector3 mRelPosition;
	float mRotationRatio;
	float mStrength;

};

#endif /* __THRUSTER_H_ */
