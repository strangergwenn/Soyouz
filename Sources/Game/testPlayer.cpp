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
