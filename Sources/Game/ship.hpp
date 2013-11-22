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
		mEngLeft =		new Engine(g, name + "_mEngLeft",	this, Vector3(-1.2f, 0, 1.5f), Quaternion());
		mEngRight =		new Engine(g, name + "_mEngRight",	this, Vector3(+1.2f, 0, 1.5f), Quaternion());
		mEngBottom =	new Engine(g, name + "_mEngBottom",	this, Vector3(0, -1.2f, 1.5f), Quaternion());
		mEngUp =		new Engine(g, name + "_mEngUp",		this, Vector3(0, +1.2f, 1.5f), Quaternion());
		
		mEngLeft->setEngineStrength(0);
		mEngRight->setEngineStrength(0);
		mEngBottom->setEngineStrength(0);
		mEngUp->setEngineStrength(0);

		mSteerX = 0;
		mSteerY = 0;
		mSteerRoll = 0;
		mSpeed = 0;
		mMaxAngularSpeed = 1;

		mRollControlMax = 500;
		mRollControlKP = 500;
		mRollControlKD = 100;
		mRollControlThreshold = 5;

		create3DHelper();
	}

	void preTick(const Ogre::FrameEvent& evt)
	{
		clearForces();
	}

	void tick(const Ogre::FrameEvent& evt)
	{
		float speedLeft, speedRight, speedBottom, speedUp;
		speedLeft = mSpeed;
		speedRight = mSpeed;
		speedBottom = mSpeed;
		speedUp = mSpeed;
		
		// Speed measures
		mRollControlValue = rotation().z;
		mAngularSpeed = mPhysBody->getAngularVelocity();
		
		// X steering
		if (mSteerX > 0)
		{
			speedRight -= Math::Abs(mSteerX/2);
		}
		else
		{
			speedLeft -= Math::Abs(mSteerX/2);
		}

		// Y steering
		if (mSteerY > 0)
		{
			speedUp -= Math::Abs(mSteerY/2);
		}
		else
		{
			speedBottom -= Math::Abs(mSteerY/2);
		}

		// Roll steering
		float diff = mSteerRoll - mRollControlValue;
		if (Math::Abs(diff) > mRollControlThreshold)
		{
			if (Math::Abs(diff) > 180.0f)
			{
				diff *= -1;
			}
			float power = mRollControlKP * (diff)
						+ mRollControlKD * (mRollControlPrevious - mRollControlValue) / evt.timeSinceLastFrame;
			//mPhysBody->applyTorque(btVector3(0, 0, Math::Clamp(power, -mRollControlMax, mRollControlMax)));
		}
		else
		{
			//mAngularSpeed.setZ(0.0f);
		}
		mRollControlPrevious = mRollControlValue;

		// Angular speed limits
		//mAngularSpeed.setX(Math::Clamp(mAngularSpeed.getX(), -mMaxAngularSpeed, mMaxAngularSpeed));
		//mAngularSpeed.setY(Math::Clamp(mAngularSpeed.getY(), -mMaxAngularSpeed, mMaxAngularSpeed));
		mPhysBody->setAngularVelocity(mAngularSpeed);

		// Control execution
		mEngLeft->setEngineStrength(speedLeft);
		mEngRight->setEngineStrength(speedRight);
		mEngBottom->setEngineStrength(speedBottom);
		mEngUp->setEngineStrength(speedUp);
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

	// Roll control
	float mRollControlMax;
	float mRollControlKP;
	float mRollControlKD;
	float mRollControlValue;
	float mRollControlPrevious;
	float mRollControlThreshold;

	// Speed limiter
	float mMaxAngularSpeed;
	btVector3 mAngularSpeed;

	// Engines
	Engine* mEngLeft;
	Engine* mEngRight;
	Engine* mEngBottom;
	Engine* mEngUp;

};

#endif /* __SHIP_H_ */
