/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/player.hpp"


/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

Player::Player(Game* g, String name) : Actor(g, name)
{
	mFOV = 90;
	mRotFactor = Real(0.1);
	mSpeedFactor = Real(5.0);
	mSpeed = Vector3::ZERO;
	mNode->setDirection(0,0,-1);

	mCamera = g->getScene()->createCamera(name + "_camera");
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

void Player::tick(const FrameEvent& evt)
{
	translate(mSpeed * evt.timeSinceLastFrame, true);
}


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


void Player::setCameraSpheric(float distance, Degree horiz, Degree vert)
{
	Quaternion qH, qV;
	Radian alphaH = Radian(horiz.valueRadians());
	Radian alphaV = Radian(vert.valueRadians());
	
	qH.FromAngleAxis(alphaH, Vector3::UNIT_Y);
	qV.FromAngleAxis(alphaV, Vector3::UNIT_X);
	
	mNode->setOrientation(qH * qV);
	setLocation(Vector3(
		distance * Math::Sin(alphaH) * Math::Cos(alphaV),
		-distance * Math::Sin(alphaV),
		distance * Math::Cos(alphaH) * Math::Cos(alphaV)));
}


/*----------------------------------------------
	Debug facilities
----------------------------------------------*/

String Player::debugText()
{
	return "";
}


void Player::setWireframe(bool bWire)
{
	mCamera->setPolygonMode(bWire ? PM_WIREFRAME : PM_SOLID);
}


/*----------------------------------------------
	Internal methods
----------------------------------------------*/

bool Player::mouseMoved(const OIS::MouseEvent &e)
{
	rotate(Vector3(-Real(e.state.Y.rel), -Real(e.state.X.rel), Real(0)) * mRotFactor);
	return true;
}


bool Player::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    return true;
}


bool Player::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    return true;
}


bool Player::keyPressed(const OIS::KeyEvent &e)
{
	float movementSpeed = 100;
	mSpeed = Vector3(0,0,0);

	switch (e.key)
	{
		case OIS::KC_A:
			mSpeed.x = -movementSpeed;
			break;

		case OIS::KC_D:
			mSpeed.x = movementSpeed;
			break;

		case OIS::KC_W:
			mSpeed.z = -movementSpeed;
			break;

		case OIS::KC_S:
			mSpeed.z = movementSpeed;
			break;

		case OIS::KC_SPACE:
			mSpeed.y = movementSpeed;
			break;

		case OIS::KC_LMENU:
			mSpeed.y = -movementSpeed;
			break;

		case OIS::KC_ESCAPE:
			mGame->quit();
			break;

		default:
			break;
	}

    return true;
}


bool Player::keyReleased(const OIS::KeyEvent &e)
{
    return true;
}


bool Player::buttonPressed(const OIS::JoyStickEvent &, int id)
{
    return true;
}


bool Player::buttonReleased(const OIS::JoyStickEvent &, int id)
{
    return true;
}


bool Player::axisMoved(const OIS::JoyStickEvent &, int axis)
{
    return true;
}