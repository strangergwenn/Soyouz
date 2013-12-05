/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/game.hpp"
#include "Engine/Rendering/renderer.hpp"
#include "Game/thruster.hpp"
#include "Game/ship.hpp"


/*----------------------------------------------
	Class definitions
----------------------------------------------*/
	
Thruster::Thruster(Game* g, String name, MeshActor* parent, Vector3 location, Quaternion rotation)
	: MeshActor(g, name, "", "")
{
	// Customization
	mMesh = NULL;
	setModel("SM_Exhaust.mesh");
	setMaterial("MI_Exhaust");
	customize(1000, 2.0, 0.1f);
	
	// Local light	
	Ogre::SceneNode *sn = mNode->createChildSceneNode();
	mLight = g->getScene()->createLight();
	mLight->setType(Ogre::Light::LT_POINT);
	mLight->setAttenuation(100, 1.0f, 0.0f, 0.2f);
	sn->attachObject(mLight);
	sn->setPosition(0, 0, -0.1f);
	
	// Position
	mRelPosition = location;
	mShip = (Ship*)parent;
	rotate(rotation);
	setLocation(location);
	parent->attachActor(this);
}


void Thruster::tick(const Ogre::FrameEvent& evt)
{
	// Basic data
	Vector3 direction = mNode->getOrientation() * Vector3(0, 0, 1);
	Vector3 rotAxis = mRelPosition.crossProduct(direction);
	Vector3 target = mShip->getDirectionCommand();
	Vector3 aim = mShip->getRotationCommand();
	float alpha = 0;

	// Rotation manager
	if (fabs(rotAxis.x) > 0.001)
	{
		alpha += aim.x * (rotAxis.x > 0 ? 1: -1);
	}
	if (fabs(rotAxis.y) > 0.001)
	{
		alpha += aim.y * (rotAxis.y > 0 ? 1: -1);
	}
	if (fabs(rotAxis.z) > 0.001)
	{
		alpha += aim.z * (rotAxis.z > 0 ? 1: -1);
	}

	// Final output calculation
	alpha *= mRotationRatio;
	alpha += target.dotProduct(direction);
	alpha = Math::Clamp(alpha, 0.0f, 1.0f);

	// Output
	setMaterialParam(1, alpha);
	float lightAlpha = Math::Clamp(10 * alpha, 0.0f, 3.0f);
	mLight->setDiffuseColour(lightAlpha * Ogre::ColourValue(0.2f, 0.9f, 1.0f));
	mShip->applyLocalForce(alpha * mStrength * direction, mRelPosition);
	MeshActor::tick(evt);
}


void Thruster::customize(float strength, float scale, float rotRatio)
{
	mStrength = strength;
	mRotationRatio = rotRatio;
	mNode->setScale(scale, scale, scale);
}


void Thruster::setModel(String file)
{
	if (mMesh)
	{
		mGame->deleteGameEntity(mMesh);
		mMesh = NULL;
	}
	mMesh = mGame->createGameEntity(mName + "_mesh", file);
	mMesh->setCastShadows(false);
	mNode->attachObject(mMesh);
}
