/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/iomanager.h"


/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

IOManager::IOManager(RenderWindow* win, Camera* cam, bool bufferedKeys = false, bool bufferedMouse = false,  bool bufferedJoy = false) :
	mCamera(cam), mTranslateVector(Vector3::ZERO), mCurrentSpeed(0), mWindow(win), mStatsOn(true), mNumScreenShots(0),
	mMoveScale(0.0f), mRotScale(0.0f), mTimeUntilNextToggle(0), mFiltering(TFO_BILINEAR),
	mAniso(1), mSceneDetailIndex(0), mMoveSpeed(100), mRotateSpeed(36), mDebugOverlay(0),
	mInputManager(0), mMouse(0), mKeyboard(0), mJoy(0)
{
	// Startup
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;
	LogManager::getSingletonPtr()->logMessage("IOManager::IOManager");
	mDebugOverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");

	// Window management
	win->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	mInputManager = OIS::InputManager::createInputSystem(pl);

	// Input methods
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, bufferedMouse));
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, bufferedKeys));
	try {
		mJoy = static_cast<OIS::JoyStick*>(mInputManager->createInputObject(OIS::OISJoyStick, bufferedJoy));
	}
	catch(...) {
		mJoy = 0;
	}

	// Events
	windowResized(mWindow);
	showDebugOverlay(true);
	WindowEventUtilities::addWindowEventListener(mWindow, this);		
}


IOManager::~IOManager()
{
	LogManager::getSingletonPtr()->logMessage("IOManager::~IOManager");
	WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
}


/*----------------------------------------------
	User methods
----------------------------------------------*/

bool IOManager::processKeyboard()
{
	Real moveScale = mMoveScale;
	if (mKeyboard->isKeyDown(OIS::KC_LSHIFT))
		moveScale *= 10;

	if (mKeyboard->isKeyDown(OIS::KC_A))
		mTranslateVector.x = -moveScale;

	if (mKeyboard->isKeyDown(OIS::KC_D))
		mTranslateVector.x = moveScale;

	if (mKeyboard->isKeyDown(OIS::KC_W) )
		mTranslateVector.z = -moveScale;

	if (mKeyboard->isKeyDown(OIS::KC_S) )
		mTranslateVector.z = moveScale;

	if (mKeyboard->isKeyDown(OIS::KC_PGUP))
		mTranslateVector.y = moveScale;

	if (mKeyboard->isKeyDown(OIS::KC_PGDOWN))
		mTranslateVector.y = -moveScale;

	if (mKeyboard->isKeyDown(OIS::KC_RIGHT))
		mCamera->yaw(-mRotScale);

	if (mKeyboard->isKeyDown(OIS::KC_LEFT))
		mCamera->yaw(mRotScale);

	if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
		return false;
	else
		return true;
}


bool IOManager::processMouse()
{
	const OIS::MouseState &ms = mMouse->getMouseState();
	if (ms.buttonDown(OIS::MB_Right))
	{
		mTranslateVector.x += ms.X.rel * 0.13;
		mTranslateVector.y -= ms.Y.rel * 0.13;
	}
	else
	{
		mRotX = Degree(-ms.X.rel * 0.13);
		mRotY = Degree(-ms.Y.rel * 0.13);
	}
	return true;
}


void IOManager::showDebugOverlay(bool show)
{
	if (mDebugOverlay)
	{
		if (show)
			mDebugOverlay->show();
		else
			mDebugOverlay->hide();
	}
}


/*----------------------------------------------
	Events
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
		mInputManager = 0;
	}
}


bool IOManager::processUnbufferedKeyInput(const FrameEvent& evt)
{
	// On-screen stats
	if (mKeyboard->isKeyDown(OIS::KC_F) && mTimeUntilNextToggle <= 0)
	{
		mStatsOn = !mStatsOn;
		showDebugOverlay(mStatsOn);
		mTimeUntilNextToggle = 1;
	}

	// Texture filtering mode
	if (mKeyboard->isKeyDown(OIS::KC_T) && mTimeUntilNextToggle <= 0)
	{
		switch(mFiltering)
		{
			case TFO_BILINEAR:
				mFiltering = TFO_TRILINEAR;
				mAniso = 1;
				break;
			case TFO_TRILINEAR:
				mFiltering = TFO_ANISOTROPIC;
				mAniso = 8;
				break;
			case TFO_ANISOTROPIC:
				mFiltering = TFO_BILINEAR;
				mAniso = 1;
				break;
			default: break;
		}

		MaterialManager::getSingleton().setDefaultTextureFiltering(mFiltering);
		MaterialManager::getSingleton().setDefaultAnisotropy(mAniso);
		showDebugOverlay(mStatsOn);
		mTimeUntilNextToggle = 1;
	}

	// Screenshot
	if (mKeyboard->isKeyDown(OIS::KC_SYSRQ) && mTimeUntilNextToggle <= 0)
	{
		std::ostringstream ss;
		ss << "screenshot_" << ++mNumScreenShots << ".png";
		mWindow->writeContentsToFile(ss.str());
		mTimeUntilNextToggle = 0.5;
		mDebugText = "Saved: " + ss.str();
	}

	// Scene rendering mode
	if (mKeyboard->isKeyDown(OIS::KC_R) && mTimeUntilNextToggle <= 0)
	{
		mSceneDetailIndex = (mSceneDetailIndex+1)%3 ;
		switch(mSceneDetailIndex) {
			case 0 : mCamera->setPolygonMode(PM_SOLID); break;
			case 1 : mCamera->setPolygonMode(PM_WIREFRAME); break;
			case 2 : mCamera->setPolygonMode(PM_POINTS); break;
		}
		mTimeUntilNextToggle = 0.5;
	}

	// Camera details
	static bool displayCameraDetails = false;
	if (mKeyboard->isKeyDown(OIS::KC_P) && mTimeUntilNextToggle <= 0)
	{
		displayCameraDetails = !displayCameraDetails;
		mTimeUntilNextToggle = 0.5;
		if (!displayCameraDetails)
		{
			mDebugText = "";
		}
	}
	if (displayCameraDetails)
	{
		mDebugText = "P: " + StringConverter::toString(mCamera->getDerivedPosition()) +
			" " + "O: " + StringConverter::toString(mCamera->getDerivedOrientation());
	}

	return processKeyboard();
}


bool IOManager::processUnbufferedMouseInput(const FrameEvent& evt)
{
	return processMouse();
}


bool IOManager::frameRenderingQueued(const FrameEvent& evt)
{

	if(mWindow->isClosed())
	{
		return false;
	}

	mSpeedLimit = mMoveScale * evt.timeSinceLastFrame;

	mKeyboard->capture();
	mMouse->capture();
	if( mJoy ) mJoy->capture();

	bool buffJ = (mJoy) ? mJoy->buffered() : true;

	Ogre::Vector3 lastMotion = mTranslateVector;

	if( !mMouse->buffered() || !mKeyboard->buffered() || !buffJ )
	{
		if (mTimeUntilNextToggle >= 0)
		{
			mTimeUntilNextToggle -= evt.timeSinceLastFrame;
		}

		mRotX = 0;
		mRotY = 0;
		mTranslateVector = Ogre::Vector3::ZERO;
		mMoveScale = mMoveSpeed * evt.timeSinceLastFrame;
		mRotScale = mRotateSpeed * evt.timeSinceLastFrame;
	}

	if( !mKeyboard->buffered() )
		if( processUnbufferedKeyInput(evt) == false )
			return false;

#ifdef USE_RTSHADER_SYSTEM
	processShaderGeneratorInput();
#endif
	if( !mMouse->buffered() )
		if( processUnbufferedMouseInput(evt) == false )
			return false;

	if (mTranslateVector == Ogre::Vector3::ZERO)
	{
		mCurrentSpeed -= evt.timeSinceLastFrame * 0.3;
		mTranslateVector = lastMotion;
	}
	else
	{
		mCurrentSpeed += evt.timeSinceLastFrame;

	}

	if (mCurrentSpeed > 1.0)
		mCurrentSpeed = 1.0;
	if (mCurrentSpeed < 0.0)
		mCurrentSpeed = 0.0;

	mTranslateVector *= mCurrentSpeed;

	if (!mMouse->buffered() || !mKeyboard->buffered() || !buffJ)
	{
		mCamera->yaw(mRotX);
		mCamera->pitch(mRotY);
		mCamera->moveRelative(mTranslateVector);
	}

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

	if (mKeyboard->isKeyDown(OIS::KC_F4) && mTimeUntilNextToggle <= 0)
	{	
		mTimeUntilNextToggle = 1.0;

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

