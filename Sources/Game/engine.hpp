/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __ENGINE_H_
#define __ENGINE_H_

#include "Engine/meshactor.hpp"

class Game;


/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class Engine : public MeshActor
{

public:
	
	Engine(Game* g, String name, MeshActor* parent, Vector3 location, Vector3 rotation)
		: MeshActor(g, name, "", "")
	{
		// Creation
		mMesh = g->createGameEntity(name + "_mesh", "SM_Exhaust.mesh");
		mMesh->setCastShadows(false);
		mNode->attachObject(mMesh);
		mRelPosition = location;
		mShip = parent;
		mAlpha = 0.0f;
		rotate(rotation);
		setLocation(location);
		parent->attachActor(this);
		
		// Engine customization
		setMaterial("MI_Exhaust");
		mEngineStrength = 100;
	}

	void tick(const FrameEvent& evt)
	{
		Vector3 direction = mNode->getOrientation() * Vector3(0, 0, -1);
		mShip->applyLocalForce(mAlpha * mEngineStrength * direction, mRelPosition);
		MeshActor::tick(evt);
	}

	void setEngineStrength(float alpha)
	{
		alpha = Math::Clamp(alpha, 0.f, 1.f);
		mAlpha = alpha;
		setMaterialParam(1, alpha);
	}


protected:

	MeshActor* mShip;
	Vector3 mRelPosition;
	float mEngineStrength;
	float mAlpha;

};

#endif /* __ENGINE_H_ */
