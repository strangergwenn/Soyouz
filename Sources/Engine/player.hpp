/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwenna�l ARBONA
**/

#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "Engine/game.hpp"
#include "Engine/actor.hpp"

class Game;


/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class Player : public Actor
{

public:
	
	/**
	 * @brief Create a player
	 * @param g				Game data
	 * @param name			Name
	 * @param scene			Scene data
	 **/
	Player(Game* g, String name, SceneManager* scene);
	
	/**
	 * @brief Delete a player
	 **/
	virtual ~Player();
	
	/**
	 * @brief Get the camera data
	 * @return camera data
	 **/
	Camera* getCamera();

	/**
	 * @brief Set the camera format
	 * @param width			Screen ratio
	 **/
	void setCameraRatio(Real ratio);
	
	/**
	 * @brief Main tick event
	 * @param evt			Frame event
	 **/
	virtual void Tick(const FrameEvent& evt);
	
	/**
	 * @brief Mouse event
	 * @param evt			Frame event
	 * @param m				Mouse data
	 * @return true if continuing
	 **/
	virtual bool processMouse(const FrameEvent& evt, OIS::Mouse* m);

	/**
	 * @brief Keyboard event
	 * @param evt			Frame event
	 * @param kb			Keyboard data
	 * @return true if continuing
	 **/
	virtual bool processKey(const FrameEvent& evt, OIS::Keyboard* kb);
	
	/**
	 * @brief Joystick event
	 * @param evt			Frame event
	 * @param j				Joystick data
	 * @return true if continuing
	 **/
	virtual bool processJoystick(const FrameEvent& evt, OIS::JoyStick* j);


protected: 
	
	int mFOV;
	Real mRotFactor;
	Real mSpeedFactor;

	Camera* mCamera;

};

#endif /* __PLAYER_H_ */

