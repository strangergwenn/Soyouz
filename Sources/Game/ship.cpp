/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/game.hpp"
#include "Game/ship.hpp"
#include "Game/thruster.hpp"
#include "Game/machinegun.hpp"


/*----------------------------------------------
	Definitions
----------------------------------------------*/

const float MAX_SPEED_RATIO =	+1.0f;
const float MIN_SPEED_RATIO =	-1.0f;

const Quaternion LEFT =			Quaternion(Radian(Degree(+90).valueRadians()), Vector3(0,1,0));
const Quaternion RIGHT =		Quaternion(Radian(Degree(-90).valueRadians()), Vector3(0,1,0));
const Quaternion TOP =			Quaternion(Radian(Degree(+90).valueRadians()), Vector3(1,0,0));
const Quaternion BOTTOM =		Quaternion(Radian(Degree(-90).valueRadians()), Vector3(1,0,0));
const Quaternion FORWARD =		Quaternion(Radian(Degree(  0).valueRadians()), Vector3(0,1,0));
const Quaternion BACK =			Quaternion(Radian(Degree(180).valueRadians()), Vector3(0,1,0));


/*----------------------------------------------
	Constructor
----------------------------------------------*/
	
Ship::Ship(Game* g, String name, String file, String material, float mass)
	: MeshActor(g, name, file, material, true, mass)
{
	// Foward RCS ring
	addThruster(Vector3(+4, 0, -5), RIGHT);
	addThruster(Vector3(-4, 0, -5), LEFT);
	addThruster(Vector3(0, +3, -5), TOP);
	addThruster(Vector3(0, -3, -5), BOTTOM);
	
	// Roll ring
	addThruster(Vector3(-3.6f, +0.6f, 0), TOP);
	addThruster(Vector3(-3.6f, -0.6f, 0), BOTTOM);
	addThruster(Vector3(+3.6f, +0.6f, 0), TOP);
	addThruster(Vector3(+3.6f, -0.6f, 0), BOTTOM);

	// Back RCS ring
	addThruster(Vector3(+4, 0, 5), RIGHT);
	addThruster(Vector3(-4, 0, 5), LEFT);
	addThruster(Vector3(0, +3, 5), TOP);
	addThruster(Vector3(0, -3, 5), BOTTOM);

	// Main thrusters
	addThruster(Vector3(0, 0, -8), FORWARD);
	addThruster(Vector3(-3, +1.6f, +8), BACK);
	addThruster(Vector3(+3, +1.6f, +8), BACK);
	addThruster(Vector3(-3, -1.6f, +8), BACK);
	addThruster(Vector3(+3, -1.6f, +8), BACK);
	
	addWeapon(Vector3(0, 0, -8), BACK);
	
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


void Ship::addThruster(Vector3 location, Quaternion rotation)
{
	Thruster* engine = new Thruster(mGame, mName + "_Eng" + StringConverter::toString(mThrusters.size()), this, location, rotation);
	mThrusters.push_back(engine);
}


void Ship::addWeapon(Vector3 location, Quaternion rotation)
{
	Weapon* weapon = new MachineGun(mGame, mName + "_Weapon" + StringConverter::toString(mWeapons.size()), this, location, rotation);
	mWeapons.push_back(weapon);
}

void Ship::setFireOrder(bool fire)
{
	if (mWeapons.size() > 0) {
		mWeapons.front()->setFireOrder(fire);
	}
}

/*----------------------------------------------
	Getters / Setters
----------------------------------------------*/

void Ship::setSpeed(float speed)
{
	mSpeed = Math::Clamp(speed, MIN_SPEED_RATIO, MAX_SPEED_RATIO);
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
