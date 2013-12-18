/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Frédéric Bertolus
**/

#ifndef __WEAPON_H_
#define __WEAPON_H_

#include "Engine/componentactor.hpp"

class Game;
class Ship;


/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class Weapon : public ComponentActor
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
	Weapon(Game* g, String name, Ship* parent, Vector3 location, Quaternion rotation);
	
	/**
	 * @brief Main tick event
	 * @param evt			Frame event
	 **/
	void tick(const Ogre::FrameEvent& evt);

	/**
	 * @brief Order the weapon to fire
	 * @param fire		activate or disable fire
	 **/
	void setFireOrder(bool fire);

	void setAimDirection(Vector3 aimDirection);

protected:

	virtual void fire() = 0; 

	Ship* mShip;
	
	Vector3 mRelPosition;
	float mRotationRatio;
	bool mFiring;
	Real mFirerate;
	Real mTimeSinceLastFire;
	Vector3 mAimDirection;
	Quaternion mTurretFirstRotation;
	Quaternion mTurretSecondRotation;
	Radian mMinTurretFirstRotationAngle;
	Radian mMaxTurretFirstRotationAngle;
	Radian mTurretFirstRotationSpeed;
	Radian mMinTurretSecondRotationAngle;
	Radian mMaxTurretSecondRotationAngle;
	Radian mTurretSecondRotationSpeed;
	ComponentActor* mBarrelActor;
	ComponentActor* mTurretActor;
};

#endif /* __WEAPON_H_ */
