/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __DEMOPLAYER_H_
#define __DEMOPLAYER_H_

#include "Engine/player.hpp"
#include "Game/ship.hpp"


/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class DemoPlayer : public Player
{

public:
	
	DemoPlayer(Game* g, String name) : Player(g, name)
	{
		// Ship mesh
		mShip = new Ship(g, "Ship", "SM_Soyouz.mesh", "Default", 100.0f);
		setLocation(Vector3(0, 0, 8));
		mShip->attachActor(this);
		mShip->setLocation(Vector3(0, 0, 300));
	}


protected:

	bool mouseMoved(const OIS::MouseEvent &e)
	{
		return true;
	}
	
 
	bool keyPressed(const OIS::KeyEvent &e)
	{
		return true;
	}


	MeshActor* mShip;

};

#endif /* __DEMOPLAYER_H_ */
