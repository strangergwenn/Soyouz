/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Game/engine.hpp"
#include "Game/ship.hpp"
#include "Engine/game.hpp"


/*----------------------------------------------
	Class definitions
----------------------------------------------*/
	
Engine::Engine(Game* g, String name, MeshActor* parent, Vector3 location, Quaternion rotation)
	: MeshActor(g, name, "", "")
{
	// Creation
	mMesh = g->createGameEntity(name + "_mesh", "SM_Exhaust.mesh");
	mMesh->setCastShadows(false);
	mNode->attachObject(mMesh);
	mRelPosition = location;
	mShip = (Ship*)parent;
	mAlpha = 0.0f;
	rotate(rotation);
	setLocation(location);
	parent->attachActor(this);
		
	// Engine customization
	setMaterial("MI_Exhaust");
	setStrength(500);

	// Debug
	Ogre::ManualObject* dir = mGame->getDebugLine(Vector3(0, 0, 1), mName + "_DBG", "White");
	mNode->attachObject(dir);
}

void Engine::tick(const Ogre::FrameEvent& evt)
{
	// Basic data
	Vector3 direction = mNode->getOrientation() * Vector3(0, 0, -1);
	Vector3 target = mShip->getDirectionCommand();
	float colinearity = target.dotProduct(direction);
	
	// Speed limiter data
	Vector3 angularSpeed = mShip->getAngularSpeed();
	Vector3 linearSpeed = mShip->getSpeed();

	// Speed limiter
	float limiter = 1.0;
	// 1. déterminer sur quels axes ce moteur peut induire de la vitesse angulaire
	//		par exemple un moteur en (1, 1, 0) de direction (1, 0, 0) produit une rotation sur l'axe Z
	// 2. si la vitesse angulaire est supérieure à mShip->getMaxAngularSpeed(), déterminer le ratio d'erreur et l'appliquer à limiter
	// 3. même process sur la vitesse linéaire
	// La vitesse angulaire est affichée sur le hud en bas à droite après le ">", format X Y Z

	//gameLog(mName + "DEBUG : " + StringConverter::toString(direction) + " " + StringConverter::toString(angularSpeed));

	// Set the current command (aka alpha)
	if (colinearity > 0.1)
	{
		setAlpha(colinearity * limiter);
	}
	else
	{
		setAlpha(0);
	}

	// Physics
	mShip->applyLocalForce(mAlpha * mStrength * direction, mRelPosition);
	MeshActor::tick(evt);
}


void Engine::setAlpha(float alpha)
{
	alpha = Math::Clamp(alpha, 0.f, 1.f);
	mAlpha = alpha;
	setMaterialParam(1, alpha);
}


void Engine::setStrength(float strength)
{
	mStrength = strength;
}
