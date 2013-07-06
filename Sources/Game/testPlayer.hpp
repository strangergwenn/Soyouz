/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __TESTPLAYER_H_
#define __TESTPLAYER_H_

#include "Engine/Player.hpp"


/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class TestPlayer : public Player
{

public:
	
	TestPlayer(World* w, String name, SceneManager* scene) : Player(w, name, scene){}

	void playerTick(const FrameEvent& evt);
	bool processKey(const FrameEvent& evt, OIS::Keyboard* kb);
	bool processJoystick(const FrameEvent& evt, OIS::JoyStick* j);

};

#endif /* __TESTPLAYER_H_ */
