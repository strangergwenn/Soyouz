/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/game.hpp"
#include "Game/thruster.hpp"
#include "Game/ship.hpp"


/*----------------------------------------------
	Class definitions
----------------------------------------------*/
	
Thruster::Thruster(Game* g, String name, MeshActor* parent, Vector3 location, Quaternion rotation)
	: MeshActor(g, name, "", "")
{
	// Creation
	mMesh = g->createGameEntity(name + "_mesh", "SM_Exhaust.mesh");
	mMesh->setCastShadows(false);
	mNode->attachObject(mMesh);
	mRelPosition = location;
	mShip = (Ship*)parent;

	// Position
	rotate(rotation);
	setLocation(location);
	parent->attachActor(this);
		
	// Customization
	setAlpha(0.0f);
	setStrength(1000);
	setRotationRatio(0.1f);
	setMaterial("MI_Exhaust");

	// Debug
	//Ogre::ManualObject* dir = mGame->getDebugLine(Vector3(0, 0, 1), mName + "_DBG", "White");
	//mNode->attachObject(dir);
}

void Thruster::tick(const Ogre::FrameEvent& evt)
{
	// Basic data
	Vector3 direction = mNode->getOrientation() * Vector3(0, 0, -1);
	Vector3 rotAxis = mRelPosition.crossProduct(direction);
	Vector3 target = mShip->getDirectionCommand();
	Vector3 aim = mShip->getRotationCommand();
	float output = 0;

	// Rotation manager
	if (fabs(rotAxis.x) > 0.001)
	{
		output += aim.x * (rotAxis.x > 0 ? 1: -1);
	}
	if (fabs(rotAxis.y) > 0.001)
	{
		output += aim.y * (rotAxis.y > 0 ? 1: -1);
	}
	if (fabs(rotAxis.z) > 0.001)
	{
		output += aim.z * (rotAxis.z > 0 ? 1: -1);
	}

	// Final output calculation
	output *= mRotationRatio;
	output += target.dotProduct(direction);

	// Output
	setAlpha(output);
	mShip->applyLocalForce(mAlpha * mStrength * direction, mRelPosition);
	MeshActor::tick(evt);
}


void Thruster::setAlpha(float alpha)
{
	alpha = Math::Clamp(alpha, 0.f, 1.f);
	mAlpha = alpha;
	setMaterialParam(1, alpha);
}


void Thruster::setStrength(float strength)
{
	mStrength = strength;
}


void Thruster::setRotationRatio(float strength)
{
	mRotationRatio = strength;
}
