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
	mRotFactor = Real(1.0);
	mSpeedFactor = Real(5.0);
	mSpeed = Vector3::ZERO;
	mNode->setDirection(0,0,-1);

	mCamera = scene->createCamera(name + "_camera");
	mCamera->setFarClipDistance(5000);
	mCamera->setNearClipDistance(5);
	mCamera->setPosition(Vector3(0,0,0));
	mCamera->setDirection(Vector3(0,0,-1));

	attachObject(mCamera);
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
	translate(mSpeed * evt.timeSinceLastFrame, true);
}


bool Player::processMouse(const FrameEvent& evt, OIS::Mouse* m)
{
	const OIS::MouseState &ms = m->getMouseState();
	rotate(Vector3(-ms.Y.rel, -ms.X.rel, 0) * mRotFactor * evt.timeSinceLastFrame);
	return true;
}


bool Player::processKey(const FrameEvent& evt, OIS::Keyboard* kb)
{
	float movementSpeed = 100;
	if (kb->isKeyDown(OIS::KC_A))
		mSpeed.x = -movementSpeed;
	if (kb->isKeyDown(OIS::KC_D))
		mSpeed.x = movementSpeed;
	if (kb->isKeyDown(OIS::KC_W))
		mSpeed.z = -movementSpeed;
	if (kb->isKeyDown(OIS::KC_S))
		mSpeed.z = movementSpeed;
	if (kb->isKeyDown(OIS::KC_SPACE))
		mSpeed.y = movementSpeed;
	if (kb->isKeyDown(OIS::KC_LMENU))
		mSpeed.y = -movementSpeed;
	translate(mSpeed * evt.timeSinceLastFrame);
	mSpeed = Vector3::ZERO;

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
