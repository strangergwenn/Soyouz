/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __WINDOW_H_
#define __WINDOW_H_

#include "Engine/ogre.hpp"
#include "Engine/gametypes.hpp"

class Game;
class Player;


/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class IOManager: public Ogre::WindowEventListener
{

public:

	/**
	 * @brief Setup the IO manager
	 * @param w					Window
	 * @param cam				Target camera
	 * @param cam				Current game
	 **/
	IOManager(Ogre::RenderWindow* w, Player* p, Game* g);

	/**
	 * @brief Delete the IO manager
	 **/
	~IOManager();

	/**
	 * @brief Rendering starting
	 * @param evt			Frame event
	 * @return true if continuing
	 **/
	void prerender(const Ogre::FrameEvent& evt);

	/**
	 * @brief Rendering done
	 * @param evt			Frame event
	 * @return true if continuing
	 **/
	void postrender(const Ogre::FrameEvent& evt);


protected:

	/**
	 * @brief Resized event
	 * @param rw			Window data
	 **/
	void windowResized(Ogre::RenderWindow* rw);

	/**
	 * @brief Close event
	 * @param rw			Window data
	 **/
	void windowClosed(Ogre::RenderWindow* rw);


	String mDebugText;
	Ogre::RenderWindow* mWindow;
	Ogre::Overlay* mDebugOverlay;

	Game* mGame;
	Player* mPlayer;

	OIS::Mouse* mMouse;
	OIS::JoyStick* mJoy;
	OIS::Keyboard* mKeyboard;
	OIS::InputManager* mInputManager;

};

#endif

