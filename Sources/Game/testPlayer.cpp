/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Game/testPlayer.hpp"


/*----------------------------------------------
	Public methods
----------------------------------------------*/

void TestPlayer::playerTick(const FrameEvent& evt)
{
	mCamera->moveRelative(mSpeed * evt.timeSinceLastFrame);
}


bool TestPlayer::processMouse(const FrameEvent& evt, OIS::Mouse* m)
{
	const OIS::MouseState &ms = m->getMouseState();
	if (ms.buttonDown(OIS::MB_Right))
	{
		mSpeed.x += Real(ms.X.rel * mSpeedFactor);
		mSpeed.y -= Real(ms.Y.rel * mSpeedFactor);
	}
	else
	{
		mCamera->yaw(Radian(-ms.X.rel * mRotFactor) * evt.timeSinceLastFrame);
		mCamera->pitch(Radian(-ms.Y.rel * mRotFactor) * evt.timeSinceLastFrame);
	}
	return true;
}


bool TestPlayer::processKey(const FrameEvent& evt, OIS::Keyboard* kb)
{
	Real moveScale = 1.0;
	
	if (kb->isKeyDown(OIS::KC_LSHIFT))
		moveScale *= 5;
	if (kb->isKeyDown(OIS::KC_A))
		mSpeed.x -= moveScale;
	if (kb->isKeyDown(OIS::KC_D))
		mSpeed.x += moveScale;
	if (kb->isKeyDown(OIS::KC_W))
		mSpeed.z -= moveScale;
	if (kb->isKeyDown(OIS::KC_S))
		mSpeed.z += moveScale;
	if (kb->isKeyDown(OIS::KC_SPACE))
		mSpeed.y += moveScale;
	if (kb->isKeyDown(OIS::KC_LMENU))
		mSpeed.y -= moveScale;

	return (!kb->isKeyDown(OIS::KC_ESCAPE));
}


bool TestPlayer::processJoystick(const FrameEvent& evt, OIS::JoyStick* j)
{
	return true;
}
