/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Game/ship.hpp"
#include "Engine/game.hpp"


/*----------------------------------------------
	Constructor
----------------------------------------------*/
	
Ship::Ship(Game* g, String name, String file, String material, float mass)
	: MeshActor(g, name, file, material, true, mass)
{		
	mEngineXPos =	new Engine(g, name + "_XP",	this, Vector3(+3, 0, -3), Quaternion(Radian(Degree(+90).valueRadians()), Vector3(0,1,0)));
	mEngineXNeg =	new Engine(g, name + "_XN",	this, Vector3(-3, 0, -3), Quaternion(Radian(Degree(-90).valueRadians()), Vector3(0,1,0)));
	mEngineYPos =	new Engine(g, name + "_YP",	this, Vector3(0, +3, -3), Quaternion(Radian(Degree(-90).valueRadians()), Vector3(1,0,0)));
	mEngineYNeg =	new Engine(g, name + "_YN",	this, Vector3(0, -3, -3), Quaternion(Radian(Degree(+90).valueRadians()), Vector3(1,0,0)));
	mEngineZPos =	new Engine(g, name + "_ZP",	this, Vector3(0, 0, +3), Quaternion(Radian(Degree(0).valueRadians()), Vector3(0,1,0)));
	mEngineZNeg =	new Engine(g, name + "_ZN",	this, Vector3(0, 0, -3), Quaternion(Radian(Degree(180).valueRadians()), Vector3(0,1,0)));
	
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
	mCommandVector = mSpeed * 
		 ((Vector3(0, 0, -1))
		+ (Vector3(+1, 0, 0) * mSteerX)
		+ (Vector3(0, +1, 0) * mSteerY));

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


float Ship::getMaxSpeed(void)
{
	return mMaxSpeed;
}
	

float Ship::getMaxAngularSpeed(void)
{
	return mMaxAngularSpeed;
}
