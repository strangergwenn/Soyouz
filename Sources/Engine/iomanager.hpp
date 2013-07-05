/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __WINDOW_H_
#define __WINDOW_H_

#include "Engine/engine.hpp"

class World;
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
	 * @param bufferedKeys		Use buffering on the keyboard
	 * @param bufferedMouse		Use buffering on the mouse
	 * @param bufferedJoy		Use buffering on the joystick
	 **/
	IOManager(RenderWindow* w, Player* p, bool bufferedKeys = false, bool bufferedMouse = false, bool bufferedJoy = false);

	/**
	 * @brief Delete the IO manager
	 **/
	~IOManager();

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

#ifdef USE_RTSHADER_SYSTEM
	/**
	 * @brief Input event
	 **/
	void processShaderGeneratorInput();
#endif

	String mDebugText;
	RenderWindow* mWindow;
	Overlay* mDebugOverlay;

	World* mWorld;
	Player* mPlayer;

	OIS::Mouse* mMouse;
	OIS::JoyStick* mJoy;
	OIS::Keyboard* mKeyboard;
	OIS::InputManager* mInputManager;

};

#endif

