/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/iomanager.h"
#include "Engine/actor.h"
#include "Engine/player.h"


/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

IOManager::IOManager(RenderWindow* w, Player* p, bool bufferedKeys, bool bufferedMouse,  bool bufferedJoy) :
	mDebugOverlay(NULL), mInputManager(NULL), mMouse(NULL), mKeyboard(NULL), mJoy(NULL)
{
	// Startup
	mPlayer = p;
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;
	LogManager::getSingletonPtr()->logMessage("IOManager::IOManager");
	mDebugOverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");

	// Window management
	mWindow = w;
	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	mInputManager = OIS::InputManager::createInputSystem(pl);

	// Input methods
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, bufferedMouse));
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, bufferedKeys));
	try {
		mJoy = static_cast<OIS::JoyStick*>(mInputManager->createInputObject(OIS::OISJoyStick, bufferedJoy));
	}
	catch (...) {
		mJoy = NULL;
	}

	// Events
	windowResized(mWindow);
	mDebugOverlay->show();
	WindowEventUtilities::addWindowEventListener(mWindow, this);		
}


IOManager::~IOManager()
{
	LogManager::getSingletonPtr()->logMessage("IOManager::~IOManager");
	WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
}


/*----------------------------------------------
	IO Events
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
		LogManager::getSingletonPtr()->logMessage("IOManager::windowClosed");
		mInputManager->destroyInputObject(mMouse);
		mInputManager->destroyInputObject(mKeyboard);
		mInputManager->destroyInputObject(mJoy);
		OIS::InputManager::destroyInputSystem(mInputManager);
		mInputManager = NULL;
	}
}


bool IOManager::frameRenderingQueued(const FrameEvent& evt)
{
	if (mWindow->isClosed())
	{
		return false;
	}
	
	mMouse->capture();
	mKeyboard->capture();
	if (mJoy) mJoy->capture();

	if (!mMouse->buffered())
	{
		if (mPlayer->processMouse(evt, mMouse) == false)
		{
			return false;
		}
	}
	
	if (!mKeyboard->buffered())
	{
		if (mPlayer->processKey(evt, mKeyboard) == false)
		{
			return false;
		}
	}

	if (mJoy && !mJoy->buffered())
	{
		if (mPlayer->processJoystick(evt, mJoy) == false)
		{
			return false;
		}
	}
	
#ifdef USE_RTSHADER_SYSTEM
	processShaderGeneratorInput();
#endif
	mPlayer->playerTick(evt);
	mDebugText = StringConverter::toString(mPlayer->speed());
	return true;
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


#ifdef USE_RTSHADER_SYSTEM

void IOManager::processShaderGeneratorInput()
{		
	if (mKeyboard->isKeyDown(OIS::KC_F2))
	{	
		mCamera->getViewport()->setMaterialScheme(MaterialManager::DEFAULT_SCHEME_NAME);			
		mDebugText = "Active Viewport Scheme: ";
		mDebugText += MaterialManager::DEFAULT_SCHEME_NAME;						
	}

	if (mKeyboard->isKeyDown(OIS::KC_F3))
	{
		mCamera->getViewport()->setMaterialScheme(RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
		mDebugText = "Active Viewport Scheme: ";
		mDebugText += RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME;
	}	

	if (mKeyboard->isKeyDown(OIS::KC_F4))
	{	
		static bool userPerPixelLightModel = true;
		RTShader::ShaderGenerator* shaderGenerator = RTShader::ShaderGenerator::getSingletonPtr();			
		RTShader::RenderState* renderState = shaderGenerator->getRenderState(RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
		renderState->reset();

		if (userPerPixelLightModel)
		{
			RTShader::SubRenderState* perPixelLightModel = shaderGenerator->createSubRenderState(RTShader::PerPixelLighting::Type);
			renderState->addTemplateSubRenderState(perPixelLightModel);

			mDebugText = "Per pixel lighting model applied to shader generator default scheme";
		}
		else
		{
			mDebugText = "Per vertex lighting model applied to shader generator default scheme";
		}

		shaderGenerator->invalidateScheme(RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
		userPerPixelLightModel = !userPerPixelLightModel;
	}	
	
}

#endif

