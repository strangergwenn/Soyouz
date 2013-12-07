/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __SHIP_H_
#define __SHIP_H_

#include "Engine/meshactor.hpp"
#include "Game/weapon.hpp"

class Game;
class Thruster;


/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class Ship : public MeshActor
{

public:
	
	/**
	 * @brief Create a ship
	 * @param g				Game actor
	 * @param name			Unique name to set to the mesh
	 * @param templateFile	Ship template file
	 * @param configFile	Ship user config file
	 **/
	Ship(Game* g, String name, Ogre::String templateFile, Ogre::String configFile);
	
	/**
	 * @brief Pre-tick event
	 * @param evt			Frame event
	 **/
	void preTick(const Ogre::FrameEvent& evt);
	
	/**
	 * @brief Main tick event
	 * @param evt			Frame event
	 **/
	void tick(const Ogre::FrameEvent& evt);
	
	/**
	 * @brief Set the ship speed target
	 * @param speed			Speed setting
	 **/
	void setSpeed(float speed);
	
	/**
	 * @brief Set the ship steer target
	 * @param x				X command (-1 - 1)
	 * @param y				Y command (-1 - 1)
	 **/
	void setSteer(float x, float y);
	
	/**
	 * @brief Set the ship roll target
	 * @param roll			Roll command
	 **/
	void setRoll(float roll);
	
	/**
	 * @brief Get the current direction target
	 * @return a vector materializing the travel direction
	 **/
	Vector3 getDirectionCommand(void);
	
	/**
	 * @brief Get the current angular rotation speed
	 * @return a vector materializing the angular speed
	 **/
	Vector3 getRotationCommand(void);

	/**
	 * @brief Get the current max linear speed
	 * @return speed
	 **/
	float getMaxSpeed(void);
	
	/**
	 * @brief Get the current max angular speed
	 * @return speed
	 **/
	float getMaxAngularSpeed(void);

/**
	 * @brief Order the ship to fire with its primary weapon
	 * @param fire		activate or disable fire
	 **/
	void setFireOrder(bool fire);

protected:
	
	/**
	 * @brief Compute a steering component for linear control
	 * @param measure		Input measure
	 * @param command		Input command
	 * @return the soft steering
	 **/
	float computeSoftLinearCommand(float measure, float command);
	
	/**
	 * @brief Compute a steering component for angular control
	 * @param measure		Input measure
	 * @param command		Input command
	 * @return the soft steering
	 **/
	float computeSoftAngularCommand(float measure, float command);
	
	/**
	 * @brief Add an engine to the ship
	 * @param location		Thruster relative location
	 * @param rotation		Thruster rotation
	 **/
	void addThruster(Vector3 location, Quaternion rotation);


	/**
	 * @brief Add an weapon to the ship
	 * @param location		Thruster relative location
	 * @param rotation		Thruster rotation
	 **/
	void addWeapon(Vector3 location, Quaternion rotation);

	// Steering controls for the player
	float mSteerX;
	float mSteerY;
	float mSteerRoll;
	float mSpeed;

	// Steering commands for engines
	Vector3 mCommandVector;
	Vector3 mCommandRotator;

	// Steering characteristics
	float mMaxSpeed;
	float mMaxAngularSpeed;
	float mSoftModeLimit;
	float mSoftModeAngularLimit;

	// Thrusters
	Ogre::list<Thruster*>::type mThrusters;
	
	// Weapons
	Ogre::list<Weapon*>::type mWeapons;

};

#endif /* __SHIP_H_ */

