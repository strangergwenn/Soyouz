/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __SHIP_H_
#define __SHIP_H_

#include "Engine/meshactor.hpp"
#include "Game/engine.hpp"

class Game;


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
	 * @param file			Ship file
	 * @param material		Ship material
	 * @param mass			Ship mass
	 **/
	Ship(Game* g, String name, String file, String material, float mass);
	
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


protected:
	
	// Steering
	float mSteerX;
	float mSteerY;
	float mSteerRoll;
	float mSpeed;
	Vector3 mAngularSpeed;
	Vector3 mCommandVector;
	Vector3 mCommandRotator;

	// Speed characteristics
	float mSteerFactor;
	float mMaxSpeed;
	float mMaxAngularSpeed;

	// Engines	
	Engine* mEngineXPos1;
	Engine* mEngineXNeg1;
	Engine* mEngineYPos1;
	Engine* mEngineYNeg1;

	Engine* mEngineXPos2;
	Engine* mEngineXNeg2;
	Engine* mEngineYPos2;
	Engine* mEngineYNeg2;
	
	Engine* mEngineRollUpLeft;
	Engine* mEngineRollUpRight;
	Engine* mEngineRollDownLeft;
	Engine* mEngineRollDownRight;

	Engine* mEngineZPos;
	Engine* mEngineZNeg;

};

#endif /* __SHIP_H_ */
