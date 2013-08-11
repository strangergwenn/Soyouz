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
		mEngLeft =		new Engine(g, name + "_mEngLeft",	this, Vector3(-1.2f, 0, 1.5f), Vector3(0, 0, 0));
		mEngRight =		new Engine(g, name + "_mEngRight",	this, Vector3(+1.2f, 0, 1.5f), Vector3(0, 0, 0));
		mEngBottom =	new Engine(g, name + "_mEngBottom",	this, Vector3(0, -1.2f, 1.5f), Vector3(0, 0, 0));
		mEngUp =		new Engine(g, name + "_mEngUp",		this, Vector3(0, +1.2f, 1.5f), Vector3(0, 0, 0));
		
		mEngLeft->setEngineStrength(0);
		mEngRight->setEngineStrength(0);
		mEngBottom->setEngineStrength(0);
		mEngUp->setEngineStrength(0);

		mSteerX = 0;
		mSteerY = 0;
		mSteerRoll = 0;
		mSpeed = 0;
	}

	void preTick(const FrameEvent& evt)
	{
		clearForces();
	}

	void tick(const FrameEvent& evt)
	{
		float speedLeft, speedRight, speedBottom, speedUp;
		speedLeft = mSpeed;
		speedRight = mSpeed;
		speedBottom = mSpeed;
		speedUp = mSpeed;
		
		// X steering
		if (mSteerX > 0)
		{
			speedRight -= Math::Abs(mSteerX);
		}
		else
		{
			speedLeft -= Math::Abs(mSteerX);
		}

		// Y steering
		if (mSteerY > 0)
		{
			speedUp -= Math::Abs(mSteerY);
		}
		else
		{
			speedBottom -= Math::Abs(mSteerY);
		}

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


	void setRoll(float roll)
	{
		mSteerRoll = Math::Clamp(roll, -1.f, 1.f);
	}


protected:
	
	
	// Current speed control
	float mSteerX;
	float mSteerY;
	float mSteerRoll;
	float mSpeed;

	// Engines
	Engine* mEngLeft;
	Engine* mEngRight;
	Engine* mEngBottom;
	Engine* mEngUp;

};

#endif /* __SHIP_H_ */
