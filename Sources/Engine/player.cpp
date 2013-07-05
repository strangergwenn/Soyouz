/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/player.hpp"
#include "Engine/world.hpp"


/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

Player::Player(World* w, String name, SceneManager* scene) : Actor(w, name)
{
	mFOV = 90;
	mRotFactor = Real(0.5);
	mSpeedFactor = Real(5.0);
	mSpeed = Vector3::ZERO;

	mCamera = scene->createCamera("PlayerCam");
	mCamera->setPosition(Vector3(0,0,250));
	mCamera->lookAt(Vector3(0,0,0));
	mCamera->setNearClipDistance(5);
}


Player::~Player()
{
}


/*----------------------------------------------
	Public methods
----------------------------------------------*/

Camera* Player::getCamera()
{
	return mCamera;
}


void Player::setCameraRatio(Real ratio)
{
	Radian R = Math::ATan(Math::Tan(Degree((float)mFOV / 2)) / ratio) * 2;
	mCamera->setAspectRatio(ratio);
	mCamera->setFOVy(R);
}


void Player::playerTick(const FrameEvent& evt)
{
	mCamera->moveRelative(mSpeed * evt.timeSinceLastFrame);
}


bool Player::processMouse(const FrameEvent& evt, OIS::Mouse* m)
{
	return true;
}


bool Player::processKey(const FrameEvent& evt, OIS::Keyboard* kb)
{
	if (kb->isKeyDown(OIS::KC_F1))
		mCamera->setPolygonMode(PM_SOLID);
	if (kb->isKeyDown(OIS::KC_F2))
		mCamera->setPolygonMode(PM_WIREFRAME);
	return (!kb->isKeyDown(OIS::KC_ESCAPE));
}


bool Player::processJoystick(const FrameEvent& evt, OIS::JoyStick* j)
{
	return true;
}
