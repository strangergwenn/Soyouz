/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "Engine/ogre.hpp"
#include "Engine/game.hpp"
#include "Engine/actor.hpp"

class Game;


/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class Player : public Actor, public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener
{

public:
	
	/**
	 * @brief Create a player
	 * @param g				Game data
	 * @param name			Name
	 **/
	Player(Game* g, String name);
	
	/**
	 * @brief Delete a player
	 **/
	virtual ~Player();
	
	/**
	 * @brief Main tick event
	 * @param evt			Frame event
	 **/
	virtual void tick(const FrameEvent& evt);
	
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
	 * @brief Set the camera position around a sphere centered on the player node
	 * @param distance		Circle radius
	 * @param horiz			Circle horizontal angle
	 * @param vert			Circle vertiocal angle
	 **/
	void setCameraSpheric(float distance, Degree horiz = Degree(0), Degree vert = Degree(0));
	
	/**
	 * @brief Get a debug string
	 * @return text
	 **/
	virtual String debugText();


protected:

	/**
	 * @brief Mouse moved
	 * @param e				Event data
	 * @return true to keep the game running
	 **/
	virtual bool mouseMoved(const OIS::MouseEvent &e);
	
	/**
	 * @brief Mouse pressed
	 * @param e				Event data
	 * @param id			Button ID
	 * @return true to keep the game running
	 **/
	virtual bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	
	/**
	 * @brief Mouse released
	 * @param e				Event data
	 * @param id			Button ID
	 * @return true to keep the game running
	 **/
	virtual bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
 
	/**
	 * @brief Key pressed
	 * @param e				Event data
	 * @return true to keep the game running
	 **/
	virtual bool keyPressed(const OIS::KeyEvent &e);
	
	/**
	 * @brief Key released
	 * @param e				Event data
	 * @return true to keep the game running
	 **/
	virtual bool keyReleased(const OIS::KeyEvent &e);
	
	/**
	 * @brief Joystick pressed
	 * @param e				Event data
	 * @param id			Button ID
	 * @return true to keep the game running
	 **/
	virtual bool buttonPressed(const OIS::JoyStickEvent &, int id);
	
	/**
	 * @brief Joystick released
	 * @param e				Event data
	 * @param id			Button ID
	 * @return true to keep the game running
	 **/
	virtual bool buttonReleased(const OIS::JoyStickEvent &, int id);
	
	/**
	 * @brief Joystick moved
	 * @param e				Event data
	 * @param id			Axis ID
	 * @return true to keep the game running
	 **/
	virtual bool axisMoved(const OIS::JoyStickEvent &, int axis);

	
	// Movement data
	Real mRotFactor;
	Vector3 mSpeed;
	Real mSpeedFactor;

	// Render data
	int mFOV;
	Camera* mCamera;

};

#endif /* __PLAYER_H_ */
