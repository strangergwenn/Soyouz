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
		
		mEngLeft->setEngineStrength(0.1);
		mEngRight->setEngineStrength(0.1);
		mEngBottom->setEngineStrength(0.9);
		mEngUp->setEngineStrength(1.0);
	}

	void preTick(const FrameEvent& evt)
	{
		clearForces();
	}


protected:
	
	Engine* mEngLeft;
	Engine* mEngRight;
	Engine* mEngBottom;
	Engine* mEngUp;

};

#endif /* __SHIP_H_ */
