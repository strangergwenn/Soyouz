/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Game/pilot.hpp"


/*----------------------------------------------
	Definitions
----------------------------------------------*/

const float MAX_TARGET_SPEED = 1.0f;
const float MIN_TARGET_SPEED = -0.1f;
const float TARGET_SPEED_INCREMENT = 0.1f;


/*----------------------------------------------
	Constructor and destructor
----------------------------------------------*/

Pilot::Pilot(Game* g, String name) : Player(g, name)
{
	// Data
	mStepDistance = 5;
	mHorizAngle = Degree(0);
	mVertAngle = Degree(0);
	mStepAngle = Degree(10);
	mControlDirection = false;
	mFastControlDirection = false;
	mTargetSpeed = 0;
	mInverted = false;

	// Ship mesh
	mShip = new Ship(g, "Ship", "Sovereign");
	mShip->attachActor(this);
	mShip->setLocation(Vector3(0, 0, 300));
	mShip->saveToFile();
	mDistance = mShip->getViewDistance();

	// Camera setup
	setCameraSpheric(mDistance, Degree(0), Degree (0));
}


/*----------------------------------------------
	Events
----------------------------------------------*/
	
void Pilot::preTick(const Ogre::FrameEvent& evt)
{
	if(mInverted) {
		Ogre::Ray ray = mCamera->getCameraToViewportRay(1.0 - mMouseState.X.abs / (float)mCamera->getViewport()->getActualWidth() , 1.0 - mMouseState.Y.abs / (float)mCamera->getViewport()->getActualHeight());
		mShip->setAimDirection(-ray.getDirection());
			
	} else {
		Ogre::Ray ray = mCamera->getCameraToViewportRay(mMouseState.X.abs / (float)mCamera->getViewport()->getActualWidth() ,mMouseState.Y.abs / (float)mCamera->getViewport()->getActualHeight());
		mShip->setAimDirection(ray.getDirection());
	}
}


bool Pilot::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	mFastControlDirection = true;
	updateDirection();
	return true;
}


bool Pilot::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	mFastControlDirection = false;
	updateDirection();
	return true;
}

bool Pilot::mouseMoved(const OIS::MouseEvent &e)
{
	if(e.state.Z.rel > 0) {
		mTargetSpeed += TARGET_SPEED_INCREMENT;
		updateSpeed();
	}
		
	if(e.state.Z.rel < 0) {
		mTargetSpeed -= TARGET_SPEED_INCREMENT;
		updateSpeed();
	}
		
	mMouseState = e.state;
	updateDirection();
	return true;
}


/*----------------------------------------------
	Methods
----------------------------------------------*/

String Pilot::debugText()
{
	return StringConverter::toString(mShip->getAngularSpeed());
}


	
void Pilot::updateDirection() {
	if(mControlDirection || mFastControlDirection) {
		float resX = (float)mCamera->getViewport()->getActualWidth();
		float resY = (float)mCamera->getViewport()->getActualHeight();
		float max = 0.9f * resY;

		float x = Math::Clamp((float)mMouseState.X.abs - (resX / 2), -max, max) / max;
		float y = Math::Clamp((float)mMouseState.Y.abs - (resY / 2), -max, max) / max;
		mShip->setSteer(x, -y);
	} else {
		mShip->setSteer(0, 0);
	}
}


void Pilot::updateSpeed()
{
	if(mTargetSpeed > MAX_TARGET_SPEED) {
		mTargetSpeed = MAX_TARGET_SPEED;
	} else if(mTargetSpeed < MIN_TARGET_SPEED) {
		mTargetSpeed = MIN_TARGET_SPEED;
	}
	mShip->setSpeed(mTargetSpeed);
}


bool Pilot::keyPressed(const OIS::KeyEvent &e)
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

		case OIS::KC_SPACE:
			mControlDirection = !mControlDirection;
			updateDirection();
			break;
		case OIS::KC_BACK:
			mTargetSpeed = 0.0;
			updateSpeed();
			break;
		case OIS::KC_Q:
			mShip->setRoll(1.0f);
			break;

		case OIS::KC_E:
			mShip->setRoll(-1.0f);
			break;
				
		case OIS::KC_I:
			mInverted = !mInverted;
			break;	

		case OIS::KC_NUMPAD5:
			mHorizAngle = 0;
			mVertAngle = 0;
			break;

		case OIS::KC_NUMPAD4:
			mHorizAngle -= mStepAngle;
			break;

		case OIS::KC_NUMPAD6:
			mHorizAngle += mStepAngle;
			break;

		case OIS::KC_NUMPAD2:
			mVertAngle += mStepAngle;
			break;

		case OIS::KC_NUMPAD8:
			mVertAngle -= mStepAngle;
			break;

		case OIS::KC_ADD:
			mDistance -= mStepDistance;
			break;

		case OIS::KC_SUBTRACT:
			mDistance += mStepDistance;
			break;
				
		case OIS::KC_LCONTROL:
			mShip->setFireOrder(true);
			mShip->setFireOrder(true);
			break;

		default:
			break;
	}

	setCameraSpheric(mDistance, Degree(mHorizAngle), Degree (mVertAngle));
	return Player::keyPressed(e);
}


bool Pilot::keyReleased(const OIS::KeyEvent &e)
{
	switch (e.key)
	{
		case OIS::KC_Q:
		case OIS::KC_E:
			mShip->setRoll(0.0f);
			break;
		case OIS::KC_LCONTROL:
			mShip->setFireOrder(false);
			mShip->setFireOrder(false);
			break;
		case OIS::KC_TAB:
			//mTargetSpeed = MAX_TARGET_SPEED;
			//updateSpeed();
			break;
		default:
		break;
	}
	return Player::keyReleased(e);
}

