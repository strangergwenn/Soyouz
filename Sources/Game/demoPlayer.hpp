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
		// Data
		stepDistance = 4;
		distance = 8;
		horizAngle = Degree(0);
		vertAngle = Degree(0);
		stepAngle = Degree(15);

		// Ship mesh
		mShip = new Ship(g, "Ship", "SM_Soyouz.mesh", "MI_Gloss", 100.0f);
		mShip->attachActor(this);
		mShip->setLocation(Vector3(0, 0, 300));

		// Camera setup
		setCameraSpheric(distance, Degree(0), Degree (0));
	}
	
	String debugText()
	{
		return StringConverter::toString(mShip->rotation().z + 180);
	}


protected:

	bool mouseMoved(const OIS::MouseEvent &e)
	{
		float resX = (float)mCamera->getViewport()->getActualWidth();
		float resY = (float)mCamera->getViewport()->getActualHeight();
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
				break;

			case OIS::KC_S:
				break;

			case OIS::KC_TAB:
				mShip->setSpeed(1.0f);
				break;

			case OIS::KC_Q:
				mShip->addRoll(10.0f);
				break;

			case OIS::KC_E:
				mShip->addRoll(-10.0f);
				break;

			case OIS::KC_NUMPAD5:
				horizAngle = 0;
				vertAngle = 0;
				break;

			case OIS::KC_NUMPAD4:
				horizAngle -= stepAngle;
				break;

			case OIS::KC_NUMPAD6:
				horizAngle += stepAngle;
				break;

			case OIS::KC_NUMPAD2:
				vertAngle += stepAngle;
				break;

			case OIS::KC_NUMPAD8:
				vertAngle -= stepAngle;
				break;

			case OIS::KC_ADD:
				distance -= stepDistance;
				break;

			case OIS::KC_SUBTRACT:
				distance += stepDistance;
				break;
				
			case OIS::KC_F1:
				mGame->setDebugMode(0);
				break;

			case OIS::KC_F2:
				mGame->setDebugMode(1);
				break;

			case OIS::KC_F3:
				mGame->setDebugMode(2);
				break;

			case OIS::KC_F4:
				mGame->setDebugMode(3);
				break;

			case OIS::KC_ESCAPE:
				mGame->quit();
				break;

			default:
				break;
		}

		setCameraSpheric(distance, Degree(horizAngle), Degree (vertAngle));
		return true;
	}

	
	bool keyReleased(const OIS::KeyEvent &e)
	{
		switch (e.key)
		{
			case OIS::KC_TAB:
				mShip->setSpeed(0.0f);
				break;
		}
		return true;
	}


	// Game data
	float distance;
	float stepDistance;
	Degree horizAngle;
	Degree vertAngle;
	Degree stepAngle;
	Ship* mShip;

};

#endif /* __DEMOPLAYER_H_ */
