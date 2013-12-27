/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __USER_H_
#define __USER_H_

#include "Engine/game.hpp"
#include "Engine/player.hpp"
#include "Editor/editor.hpp"


/*----------------------------------------------
Class definitions
----------------------------------------------*/

class User : public Player
{	

public:
	
	/**
	 * @brief Create a user
	 * @param g				Editor data
	 * @param name			Name
	 **/
	User(Editor* g, String name);


protected:
	
	/**
	 * @brief Main tick event
	 * @param evt			Frame event
	 **/
	void tick(const Ogre::FrameEvent& evt);	
	
	/**
	 * @brief Mouse pressed
	 * @param e				Event data
	 * @param id			Button ID
	 * @return true to keep the game running
	 **/
	bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	
	/**
	 * @brief Mouse released
	 * @param e				Event data
	 * @param id			Button ID
	 * @return true to keep the game running
	 **/
	bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	
	/**
	 * @brief Mouse moved
	 * @param e				Event data
	 * @return true to keep the game running
	 **/
	bool mouseMoved(const OIS::MouseEvent &e);

	/**
	 * @brief Key pressed
	 * @param e				Event data
	 * @return true to keep the game running
	 **/
	bool keyPressed(const OIS::KeyEvent &e);
	

protected:

	// Game state
	Editor* mEditor;
	bool bMeshTurning;
	bool bLightsTurning;

	// Spheric coordinates
	float mDistance;
	Ogre::Degree mSceneYaw;
	Ogre::Degree mScenePitch;

	// Stepping data
	float mDistanceStep;
	Ogre::Degree mDegreeStep;
	

};

#endif /* __USER_H_ */
