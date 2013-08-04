/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/iomanager.hpp"
#include "Engine/game.hpp"
#include "Engine/actor.hpp"
#include "Engine/player.hpp"


/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

IOManager::IOManager(RenderWindow* w, Player* p, Game* g) :
	mDebugOverlay(NULL), mInputManager(NULL), mMouse(NULL), mKeyboard(NULL), mJoy(NULL)
{
	// Startup
	mGame = g;
	mPlayer = p;
	bRunning = true;
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;
	mDebugOverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");

	// Window management
	mWindow = w;
	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	mInputManager = OIS::InputManager::createInputSystem(pl);

	// Input methods
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
	try {
		mJoy = static_cast<OIS::JoyStick*>(mInputManager->createInputObject(OIS::OISJoyStick, true));
	}
	catch (...) {
		mJoy = NULL;
	}

	// Callbacks
	mMouse->setEventCallback(mPlayer);
	mKeyboard->setEventCallback(mPlayer);
	if (mJoy)
	{
		mJoy->setEventCallback(mPlayer);
	}

	// Events
	windowResized(mWindow);
	mDebugOverlay->show();
	WindowEventUtilities::addWindowEventListener(mWindow, this);		
}


IOManager::~IOManager()
{
	WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
}


void IOManager::quit()
{
	bRunning = false;
}


/*----------------------------------------------
	IO events
----------------------------------------------*/

void IOManager::windowResized(RenderWindow* rw)
{
	int left, top;
	unsigned int width, height, depth;

	const OIS::MouseState &ms = mMouse->getMouseState();
	rw->getMetrics(width, height, depth, left, top);

	ms.width = width;
	ms.height = height;
}


void IOManager::windowClosed(RenderWindow* rw)
{
	if (rw == mWindow && mInputManager)
	{
		mInputManager->destroyInputObject(mMouse);
		mInputManager->destroyInputObject(mKeyboard);
		mInputManager->destroyInputObject(mJoy);
		OIS::InputManager::destroyInputSystem(mInputManager);
		mInputManager = NULL;
	}
}


bool IOManager::frameRenderingQueued(const FrameEvent& evt)
{
	// Window check
	if (mWindow->isClosed())
	{
		return false;
	}
	
	// Peripheral capture
	mMouse->capture();
	mKeyboard->capture();
	if (mJoy) mJoy->capture();
	
	// World tick
	mGame->tick(evt);

	// Debug
	mDebugText = "LOC : ";
	Vector3 loc = mPlayer->location();
	mDebugText += StringConverter::toString(Vector3(Math::Floor(loc[0]), Math::Floor(loc[1]), Math::Floor(loc[2])));
	mDebugText += " ROT : ";
	loc = mPlayer->rotation();
	mDebugText += StringConverter::toString(Vector3((loc[0]), (loc[1]), (loc[2])));
	mDebugText += " SPEED : ";
	loc = mPlayer->speed();
	mDebugText += StringConverter::toString(Vector3((loc[0]), (loc[1]), (loc[2])));

	// Return true to keep it running
	return bRunning;
}


bool IOManager::frameEnded(const FrameEvent& evt)
{
	static String currFps = "Current FPS: ";
	static String avgFps = "Average FPS: ";
	static String bestFps = "Best FPS: ";
	static String worstFps = "Worst FPS: ";
	static String tris = "Triangle Count: ";
	static String batches = "Batch Count: ";

	try {
		const RenderTarget::FrameStats& stats = mWindow->getStatistics();

		OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
		guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));

		OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
		guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));

		OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
		guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
			+" "+StringConverter::toString(stats.bestFrameTime)+" ms");

		OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");
		guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
			+" "+StringConverter::toString(stats.worstFrameTime)+" ms");

		OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
		guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount));

		OverlayElement* guiBatches = OverlayManager::getSingleton().getOverlayElement("Core/NumBatches");
		guiBatches->setCaption(batches + StringConverter::toString(stats.batchCount));

		OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
		guiDbg->setCaption(mDebugText);
	}
	catch (...) {}
	return true;
}
