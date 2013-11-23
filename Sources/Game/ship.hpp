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
	
	Ship(Game* g, String name, String file, String material, float mass)
		: MeshActor(g, name, file, material, true, mass)
	{
		//mEngLeft =		new Engine(g, name + "_mEngLeft",	this, Vector3(-1.2f, 0, 1.5f), Quaternion());
		
		mEngineXPos =	new Engine(g, name + "_XP",	this, Vector3(+3, 0, 0), Quaternion(Radian(Degree(+90).valueRadians()), Vector3(0,1,0)));
		mEngineXNeg =	new Engine(g, name + "_XN",	this, Vector3(-3, 0, 0), Quaternion(Radian(Degree(-90).valueRadians()), Vector3(0,1,0)));
		mEngineYPos =	new Engine(g, name + "_YP",	this, Vector3(0, +3, 0), Quaternion(Radian(Degree(-90).valueRadians()), Vector3(1,0,0)));
		mEngineYNeg =	new Engine(g, name + "_YN",	this, Vector3(0, -3, 0), Quaternion(Radian(Degree(+90).valueRadians()), Vector3(1,0,0)));
		//mEngineZPos =	new Engine(g, name + "_ZP",	this, Vector3(0, 0, +3), Quaternion(Radian(Degree(0).valueRadians()), Vector3(0,1,0)));
		//mEngineZNeg =	new Engine(g, name + "_ZN",	this, Vector3(0, 0, -3), Quaternion(Radian(Degree(180).valueRadians()), Vector3(0,1,0)));
		
		mSteerX = 0;
		mSteerY = 0;
		mSteerRoll = 0;
		mSpeed = 0;
		mMaxSpeed = 1000;
		mMaxAngularSpeed = 1;

		//create3DHelper();
	}

	void preTick(const Ogre::FrameEvent& evt)
	{
		clearForces();
	}

	void tick(const Ogre::FrameEvent& evt)
	{		
		// Speed measures
		btVector3 mAngularSpeed = mPhysBody->getAngularVelocity();
				
		// Control execution
		mEngineXNeg->setEngineStrength(+Math::Clamp(mSteerX, 0.0f, mMaxSpeed));
		mEngineXPos->setEngineStrength(-Math::Clamp(mSteerX, -mMaxSpeed, 0.0f));
		mEngineYNeg->setEngineStrength(+Math::Clamp(mSteerY, 0.0f, mMaxSpeed));
		mEngineYPos->setEngineStrength(-Math::Clamp(mSteerY, -mMaxSpeed, 0.0f));
		MeshActor::tick(evt);
	}


	void setSpeed(float speed)
	{
		mSpeed = Math::Clamp(speed, 0.f, 1.f);
	}


	void setSteer(float x, float y)
	{
		mSteerX = Math::Clamp(x, -1.f, 1.f);
		mSteerY = Math::Clamp(y, -1.f, 1.f);
	}


	void addRoll(float roll)
	{
		mSteerRoll = Math::Clamp(mSteerRoll + roll, -180.0f, 180.0f);
	}


protected:
	
	
	// Steering
	float mSteerX;
	float mSteerY;
	float mSteerRoll;
	float mSpeed;

	// Speed limiter
	float mMaxSpeed;
	float mMaxAngularSpeed;

	// Engines
	//Engine* mEngLeft;
	//Engine* mEngRight;
	//Engine* mEngBottom;
	//Engine* mEngUp;
	
	Engine* mEngineXPos;
	Engine* mEngineXNeg;
	Engine* mEngineYPos;
	Engine* mEngineYNeg;
	Engine* mEngineZPos;
	Engine* mEngineZNeg;

};

#endif /* __SHIP_H_ */
