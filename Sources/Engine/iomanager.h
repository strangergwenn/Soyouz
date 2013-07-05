/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __WINDOW_H_
#define __WINDOW_H_

#include "Engine/engine.h"


/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class IOManager: public FrameListener, public WindowEventListener
{


public:

	/**
	 * @brief Setup the IO manager
	 * @param win			Target window
	 * @param cam			Target camera
	 * @param bufferedKeys		Use buffering on the keyboard
	 * @param bufferedMouse		Use buffering on the mouse
	 * @param bufferedJoy		Use buffering on the joystick
	 **/
	IOManager(RenderWindow* win, Camera* cam, bool bufferedKeys = false, bool bufferedMouse = false, bool bufferedJoy = false);

	/**
	 * @brief Delete the IO manager
	 **/
	~IOManager();

	/**
	 * @brief Activate the debug overlay
	 * @param show			New state
	 **/
	void showDebugOverlay(bool show);

protected:

	/**
	 * @brief User keyboard event
	 * @return true if continuing
	 **/
	bool processKeyboard();

	/**
	 * @brief User keyboard event
	 * @return true if continuing
	 **/
	bool processMouse();

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
	 * @brief Keyboard event
	 * @param evt			Frame event
	 * @return true if continuing
	 **/
	bool processUnbufferedKeyInput(const FrameEvent& evt);

	/**
	 * @brief Mouse event
	 * @param evt			Frame event
	 * @return true if continuing
	 **/
	bool processUnbufferedMouseInput(const FrameEvent& evt);

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

	float mMoveScale;
	float mSpeedLimit;
	Real mMoveSpeed;
	Real mCurrentSpeed;
	Radian mRotX, mRotY;
	Degree mRotScale;
	Degree mRotateSpeed;
	Vector3 mTranslateVector;

	bool mStatsOn;
	String mDebugText;
	Real mTimeUntilNextToggle;
	unsigned int mNumScreenShots;

	int mAniso;
	int mSceneDetailIndex;
	TextureFilterOptions mFiltering;

	Camera* mCamera;
	RenderWindow* mWindow;
	Overlay* mDebugOverlay;

	OIS::InputManager* mInputManager;
	OIS::Mouse*    mMouse;
	OIS::Keyboard* mKeyboard;
	OIS::JoyStick* mJoy;
};

#endif

