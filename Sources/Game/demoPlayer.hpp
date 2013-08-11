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
		float resX = mCamera->getViewport()->getActualWidth();
		float resY = mCamera->getViewport()->getActualHeight();
		float max = 0.9f * resY;

		float x = Math::Clamp((float)e.state.X.abs - (resX / 2), -max, max) / max;
		float y = Math::Clamp((float)e.state.Y.abs - (resY / 2), -max, max) / max;
		mShip->setSteer(x, -y);
		return true;
	}
	
 
	bool keyPressed(const OIS::KeyEvent &e)
	{
		switch (e.key)
		{
			case OIS::KC_A:
				break;

			case OIS::KC_D:
				break;

			case OIS::KC_W:
				mShip->setSpeed(1.0f);
				break;

			case OIS::KC_S:
				mShip->setSpeed(0.0f);
				break;

			case OIS::KC_Q:
				mShip->setRoll(1.0f);
				break;

			case OIS::KC_E:
				mShip->setRoll(-1.0f);
				break;

			case OIS::KC_ESCAPE:
				mGame->quit();
				break;

			default:
				break;
		}
		return true;
	}


	Ship* mShip;

};

#endif /* __DEMOPLAYER_H_ */
