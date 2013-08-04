/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __WINDOW_H_
#define __WINDOW_H_

#include "Engine/ogre.hpp"

class Game;
class Player;


/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class IOManager: public FrameListener, public WindowEventListener
{

public:

	/**
	 * @brief Setup the IO manager
	 * @param w					Window
	 * @param cam				Target camera
	 * @param cam				Current game
	 **/
	IOManager(RenderWindow* w, Player* p, Game* g);

	/**
	 * @brief Delete the IO manager
	 **/
	~IOManager();
	
	/**
	 * @brief Quit the game
	 **/
	void quit();

protected:

	/**
	 * @brief Resized event
	 * @param rw			Window data
	 **/
	void windowResized(RenderWindow* rw);

	/**
	 * @brief Close event
	 * @param rw			Window data
	 **/
	void windowClosed(RenderWindow* rw);

	/**
	 * @brief Renderign computed
	 * @param evt			Frame event
	 **/
	bool frameRenderingQueued(const FrameEvent& evt);

	/**
	 * @brief Rendering done
	 * @param evt			Frame event
	 * @return true if continuing
	 **/
	bool frameEnded(const FrameEvent& evt);


	bool bRunning;

	String mDebugText;
	RenderWindow* mWindow;
	Overlay* mDebugOverlay;

	Game* mGame;
	Player* mPlayer;

	OIS::Mouse* mMouse;
	OIS::JoyStick* mJoy;
	OIS::Keyboard* mKeyboard;
	OIS::InputManager* mInputManager;

};

#endif

