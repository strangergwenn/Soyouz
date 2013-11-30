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

IOManager::IOManager(Ogre::RenderWindow* w, Player* p, Game* g) :
	mDebugOverlay(NULL), mInputManager(NULL), mMouse(NULL), mKeyboard(NULL), mJoy(NULL)
{
	// Startup
	mGame = g;
	mPlayer = p;
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;
	mDebugOverlay = Ogre::OverlayManager::getSingleton().getByName("Core/DebugOverlay");

	// Window management
	mWindow = w;
	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	mInputManager = OIS::InputManager::createInputSystem(pl);

	// Input methods
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
	/*try {
		mJoy = static_cast<OIS::JoyStick*>(mInputManager->createInputObject(OIS::OISJoyStick, true));
	}
	catch (...) {
		mJoy = NULL;
	}*/

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
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	// Create an overlay
	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
	Ogre::Overlay* overlay = overlayManager.create("BeautifulCursor");

	// Create a panel
	cursor = static_cast<Ogre::OverlayContainer*>(overlayManager.createOverlayElement("Panel", "PanelName"));
	cursor->setMetricsMode(Ogre::GMM_PIXELS);
	cursor->setPosition(0.0, 0.0);
	cursor->setDimensions(3, 3);
	cursor->setMaterialName("BaseWhite");

	// Add the panel to the overlay and show it
	overlay->add2D(cursor);
	overlay->show();
}


IOManager::~IOManager()
{
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
}


/*----------------------------------------------
	IO events
----------------------------------------------*/


void IOManager::prerender(const Ogre::FrameEvent& evt)
{
	// Window check
	if (mWindow->isClosed())
	{
		mGame->quit();
		return;
	}
	
	// Peripheral capture
	mMouse->capture();
	mKeyboard->capture();
	if (mJoy) mJoy->capture();

	// Debug
	mDebugText = "LOC : ";
	Vector3 loc = mPlayer->location();
	mDebugText += StringConverter::toString(Vector3(Math::Floor(loc[0]), Math::Floor(loc[1]), Math::Floor(loc[2])));
	mDebugText += " ROT : ";
	Quaternion rot = mPlayer->rotation();
	mDebugText += "roll=" + StringConverter::toString(rot.getRoll()) + " pitch=" + StringConverter::toString(rot.getPitch()) + " yaw=" + StringConverter::toString(rot.getYaw());
	mDebugText += " > " + mPlayer->debugText();

	cursor->setPosition( mMouse->getMouseState().X.abs - 1.0f, mMouse->getMouseState().Y.abs - 1.0f);
}


void IOManager::postrender(const Ogre::FrameEvent& evt)
{
	static String currFps = "Current FPS: ";
	static String avgFps = "Average FPS: ";
	static String bestFps = "Best FPS: ";
	static String worstFps = "Worst FPS: ";
	static String tris = "Triangle Count: ";
	static String batches = "Batch Count: ";

	try {
		const Ogre::RenderTarget::FrameStats& stats = mWindow->getStatistics();

		Ogre::OverlayElement* guiAvg = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
		guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));

		Ogre::OverlayElement* guiCurr = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
		guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));

		Ogre::OverlayElement* guiBest = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
		guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
			+" "+StringConverter::toString(stats.bestFrameTime)+" ms");

		Ogre::OverlayElement* guiWorst = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");
		guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
			+" "+StringConverter::toString(stats.worstFrameTime)+" ms");

		Ogre::OverlayElement* guiTris = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
		guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount));

		Ogre::OverlayElement* guiBatches = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/NumBatches");
		guiBatches->setCaption(batches + StringConverter::toString(stats.batchCount));

		Ogre::OverlayElement* guiDbg = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
		guiDbg->setCaption(mDebugText);
	}
	catch (...) {}
}


void IOManager::windowResized(Ogre::RenderWindow* rw)
{
	int left, top;
	unsigned int width, height, depth;

	const OIS::MouseState &ms = mMouse->getMouseState();
	rw->getMetrics(width, height, depth, left, top);

	ms.width = width;
	ms.height = height;
}


void IOManager::windowClosed(Ogre::RenderWindow* rw)
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
