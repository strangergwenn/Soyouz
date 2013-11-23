/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwenna�l ARBONA
**/

#include "Game/ship.hpp"
#include "Engine/game.hpp"


/*----------------------------------------------
	Constructor
----------------------------------------------*/
	
Ship::Ship(Game* g, String name, String file, String material, float mass)
	: MeshActor(g, name, file, material, true, mass)
{
	mEngineXPos1 =	new Engine(g, name + "_XP1",	this, Vector3(+3, 0, -3), Quaternion(Radian(Degree(+90).valueRadians()), Vector3(0,1,0)));
	mEngineXNeg1 =	new Engine(g, name + "_XN1",	this, Vector3(-3, 0, -3), Quaternion(Radian(Degree(-90).valueRadians()), Vector3(0,1,0)));
	mEngineYPos1 =	new Engine(g, name + "_YP1",	this, Vector3(0, +3, -3), Quaternion(Radian(Degree(-90).valueRadians()), Vector3(1,0,0)));
	mEngineYNeg1 =	new Engine(g, name + "_YN1",	this, Vector3(0, -3, -3), Quaternion(Radian(Degree(+90).valueRadians()), Vector3(1,0,0)));

	mEngineXPos2 =	new Engine(g, name + "_XP2",	this, Vector3(+3, 0, 3), Quaternion(Radian(Degree(+90).valueRadians()), Vector3(0,1,0)));
	mEngineXNeg2 =	new Engine(g, name + "_XN2",	this, Vector3(-3, 0, 3), Quaternion(Radian(Degree(-90).valueRadians()), Vector3(0,1,0)));
	mEngineYPos2 =	new Engine(g, name + "_YP2",	this, Vector3(0, +3, 3), Quaternion(Radian(Degree(-90).valueRadians()), Vector3(1,0,0)));
	mEngineYNeg2 =	new Engine(g, name + "_YN2",	this, Vector3(0, -3, 3), Quaternion(Radian(Degree(+90).valueRadians()), Vector3(1,0,0)));
	
	//mEngineRollUpLeft =	new Engine(g, name + "_R1L", this, Vector3(-3, +3, 0), Quaternion(Radian(Degree(-90).valueRadians()), Vector3(0,1,0)));
	//mEngineRollUpRight = new Engine(g, name + "_R1R", this, Vector3(3, +3, 0), Quaternion(Radian(Degree(90).valueRadians()), Vector3(0,1,0)));
	//mEngineRollDownLeft = new Engine(g, name + "_R2L", this, Vector3(-3, -3, 0), Quaternion(Radian(Degree(-90).valueRadians()), Vector3(0,1,0)));
	//mEngineRollDownRight = new Engine(g, name + "_R2R", this, Vector3(3, -3, 0), Quaternion(Radian(Degree(90).valueRadians()), Vector3(0,1,0)));

	//mEngineZPos =	new Engine(g, name + "_ZP",	this, Vector3(0, 0, +3), Quaternion(Radian(Degree(0).valueRadians()), Vector3(0,1,0)));
	//mEngineZNeg =	new Engine(g, name + "_ZN",	this, Vector3(0, 0, -3), Quaternion(Radian(Degree(180).valueRadians()), Vector3(0,1,0)));
	
	mSteerX = 0;
	mSteerY = 0;
	mSteerRoll = 0;
	mSpeed = 0;

	mSteerFactor = 1;
	mMaxSpeed = 1000;
	mMaxAngularSpeed = 1;
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
	mCommandVector = 0 * mSpeed * 
		 ((Vector3(0, 0, -1))
		+ (Vector3(+1, 0, 0) * mSteerX)
		+ (Vector3(0, +1, 0) * mSteerY));

	mCommandRotator = Vector3(mSteerY, mSteerX, mSteerRoll);

	MeshActor::tick(evt);
}


void Ship::setSpeed(float speed)
{
	mSpeed = Math::Clamp(speed, 0.f, 1.f);
}


void Ship::setSteer(float x, float y)
{
	mSteerX = Math::Clamp(x, -1.f, 1.f);
	mSteerY = Math::Clamp(y, -1.f, 1.f);
}


void Ship::addRoll(float roll)
{
	mSteerRoll = Math::Clamp(mSteerRoll + roll, -180.0f, 180.0f);
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