/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __PILOT_H_
#define __PILOT_H_

#include "Engine/player.hpp"
#include "Game/ship.hpp"


/*----------------------------------------------
	Class definition
----------------------------------------------*/

class Pilot : public Player
{

public:
	
	Pilot(Game* g, String name);
	
	String debugText();

protected:

	
	void preTick(const Ogre::FrameEvent& evt);

	bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);

	bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

	bool mouseMoved(const OIS::MouseEvent &e);
	
	void updateDirection();
	
	void updateSpeed();
 
	bool keyPressed(const OIS::KeyEvent &e);

	
	bool keyReleased(const OIS::KeyEvent &e);


	// Game data
	float mDistance;
	float mStepDistance;
	float mTargetSpeed;
	Degree mHorizAngle;
	Degree mVertAngle;
	Degree mStepAngle;
	Ship* mShip;
	OIS::MouseState mMouseState;
	bool mControlDirection;
	bool mFastControlDirection;
	bool mInverted;

};

#endif /* __PILOT_H_ */
