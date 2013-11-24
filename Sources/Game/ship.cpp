/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Game/ship.hpp"
#include "Engine/game.hpp"

const float MAX_SPEED = 1.0f;
const float MIN_SPEED = -1.0f;

/*----------------------------------------------
	Constructor
----------------------------------------------*/
	
Ship::Ship(Game* g, String name, String file, String material, float mass)
	: MeshActor(g, name, file, material, true, mass)
{
	// RCS ring 1
	addEngine(Vector3(+3, 0, -3), Quaternion(Radian(Degree(+90).valueRadians()), Vector3(0,1,0)));
	addEngine(Vector3(+3, 0, -3), Quaternion(Radian(Degree(+90).valueRadians()), Vector3(0,1,0)));
	addEngine(Vector3(-3, 0, -3), Quaternion(Radian(Degree(-90).valueRadians()), Vector3(0,1,0)));
	addEngine(Vector3(0, +3, -3), Quaternion(Radian(Degree(-90).valueRadians()), Vector3(1,0,0)));
	addEngine(Vector3(0, -3, -3), Quaternion(Radian(Degree(+90).valueRadians()), Vector3(1,0,0)));

	// RCS ring 2
	addEngine( Vector3(+3, 0, 3), Quaternion(Radian(Degree(+90).valueRadians()), Vector3(0,1,0)));
	addEngine(Vector3(-3, 0, 3), Quaternion(Radian(Degree(-90).valueRadians()), Vector3(0,1,0)));
	addEngine(Vector3(0, +3, 3), Quaternion(Radian(Degree(-90).valueRadians()), Vector3(1,0,0)));
	addEngine(Vector3(0, -3, 3), Quaternion(Radian(Degree(+90).valueRadians()), Vector3(1,0,0)));
	
	// Roll ring
	addEngine(Vector3(-3, +3, 0), Quaternion(Radian(Degree(-90).valueRadians()), Vector3(0,1,0)));
	addEngine(Vector3(3, +3, 0), Quaternion(Radian(Degree(90).valueRadians()), Vector3(0,1,0)));
	addEngine(Vector3(-3, -3, 0), Quaternion(Radian(Degree(-90).valueRadians()), Vector3(0,1,0)));
	addEngine(Vector3(3, -3, 0), Quaternion(Radian(Degree(90).valueRadians()), Vector3(0,1,0)));

	// Main thrusters
	addEngine(Vector3(0, 0, +3), Quaternion(Radian(Degree(0).valueRadians()), Vector3(0,1,0)));
	addEngine(Vector3(0, 0, -3), Quaternion(Radian(Degree(180).valueRadians()), Vector3(0,1,0)));
	
	// Steering controls
	mSteerX = 0;
	mSteerY = 0;
	mSteerRoll = 0;
	mSpeed = 0;

	// Ship characteristics
	mMaxSpeed = 75;
	mMaxAngularSpeed = 2;
	mSoftModeLimit = 5;
	mSoftModeAngularLimit = 0.1f;
}


/*----------------------------------------------
	Events
----------------------------------------------*/
	

void Ship::preTick(const Ogre::FrameEvent& evt)
{
	clearForces();
}


void Ship::tick(const Ogre::FrameEvent& evt)
{
	Vector3 localSpeed = getLocalSpeed();
	Vector3 localAngularSpeed = getLocalAngularSpeed();

	mCommandRotator.x = computeSoftAngularCommand(localAngularSpeed.x, mSteerY);
	mCommandRotator.y = computeSoftAngularCommand(localAngularSpeed.y, -mSteerX);
	mCommandRotator.z = computeSoftAngularCommand(localAngularSpeed.z, mSteerRoll);

	mCommandVector.x = computeSoftLinearCommand(localSpeed.x, 0.0f);
	mCommandVector.y = computeSoftLinearCommand(localSpeed.y, 0.0f);
	mCommandVector.z = computeSoftLinearCommand(localSpeed.z, mSpeed);

	MeshActor::tick(evt);
}


/*----------------------------------------------
	Steering
----------------------------------------------*/

float Ship::computeSoftLinearCommand(float measure, float command)
{
	float result;
	
	if (measure + (command * mMaxSpeed) < -mSoftModeLimit)
	{
		result = 1;
	}
	else if (measure + (command * mMaxSpeed) > mSoftModeLimit )
	{
		result = -1;
	}
	else
	{
		result =  - (1.0f / mSoftModeLimit) * (measure + (command * mMaxSpeed));
	}

	return result;
}


float Ship::computeSoftAngularCommand(float measure, float command)
{
	float result;

	if (measure - (command * mMaxAngularSpeed) < -mSoftModeAngularLimit)
	{
		result = 1;
	}
	else if (measure - (command * mMaxAngularSpeed) > mSoftModeAngularLimit)
	{
		result = -1;
	}
	else
	{
		result = - (1.0f / mSoftModeAngularLimit) * (measure - (command * mMaxAngularSpeed));
	}

	return result;
}


void Ship::addEngine(Vector3 location, Quaternion rotation)
{
	Engine* engine = new Engine(mGame, mName + "_Eng" + StringConverter::toString(mEngines.size()), this, location, rotation);
	mEngines.push_back(engine);
}


/*----------------------------------------------
	Getters / Setters
----------------------------------------------*/

void Ship::setSpeed(float speed)
{
	mSpeed = Math::Clamp(speed, MIN_SPEED, MAX_SPEED);
}


void Ship::setSteer(float x, float y)
{
	mSteerX = Math::Clamp(x, -1.f, 1.f);
	mSteerY = Math::Clamp(y, -1.f, 1.f);
}


void Ship::setRoll(float roll)
{
	mSteerRoll = Math::Clamp(roll, -20.0f, 20.0f);
}


Vector3 Ship::getDirectionCommand(void)
{
	return mCommandVector;
}
	

Vector3 Ship::getRotationCommand(void)
{
	return mCommandRotator;
}


float Ship::getMaxSpeed(void)
{
	return mMaxSpeed;
}
	

float Ship::getMaxAngularSpeed(void)
{
	return mMaxAngularSpeed;
}
