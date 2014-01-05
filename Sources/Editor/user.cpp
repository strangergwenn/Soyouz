/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Editor/user.hpp"
#include "Editor/editor.hpp"


/*----------------------------------------------
Class definitions
----------------------------------------------*/


User::User(Editor* g, String name) : Player(g, name)
{
	mEditor = g;
	bMeshTurning = false;
	bLightsTurning = false;

	mDistance = 0;
	mDistanceStep = 0.01f;
	mDegreeStep = 0.5f;
}


void User::tick(const Ogre::FrameEvent& evt)
{
	Player::tick(evt);
	setCameraSpheric(mDistance, mSceneYaw, mScenePitch);
}
	

bool User::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Left)
	{
		bMeshTurning = true;
	}
	else if (id == OIS::MB_Right)
	{
		bLightsTurning = true;
	}
	return true;
}


bool User::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Left)
	{
		bMeshTurning = false;
	}
	else if (id == OIS::MB_Right)
	{
		bLightsTurning = false;
	}
	return true;
}


bool User::mouseMoved(const OIS::MouseEvent &e)
{
	// Zooming
	if (e.state.Z.rel > 0)
	{
		mDistance -= mDistanceStep;
	}
	else if (e.state.Z.rel < 0)
	{
		mDistance += mDistanceStep;
	}
		
	// Turning
	if (bMeshTurning)
	{
		if (e.state.X.rel > 0)
		{
			mSceneYaw -= Ogre::Degree(mDegreeStep);
		}
		else if (e.state.X.rel < 0)
		{
			mSceneYaw += Ogre::Degree(mDegreeStep);
		}

		if (e.state.Y.rel > 0)
		{
			mScenePitch -= Ogre::Degree(mDegreeStep);
		}
		else if (e.state.Y.rel < 0)
		{
			mScenePitch += Ogre::Degree(mDegreeStep);
		}
	}
		
	return true;
}


bool User::keyPressed(const OIS::KeyEvent &e)
{
	switch (e.key)
	{
		case OIS::KC_A:
		case OIS::KC_D:
		case OIS::KC_W:
		case OIS::KC_S:
			break;
			
		case OIS::KC_RIGHT:
			mEditor->step(true);
			break;

		case OIS::KC_LEFT:
			mEditor->step(false);
			break;

		default:
			break;
	}

	return Player::keyPressed(e);
}
