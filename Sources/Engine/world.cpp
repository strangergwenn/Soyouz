/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/world.h"

#define CONFIG_PATH		"../../Config/"
#define OGRE_CONF		"soyouz.cfg"
#if OGRE_DEBUG_MODE
#  define PLUGINS_CONF		"plugins_d.cfg"	
#  define RESOURCES_CONF	"resources_d.cfg"	
#else
#  define PLUGINS_CONF		"plugins.cfg"	
#  define RESOURCES_CONF	"resources.cfg"	
#endif
#define LOGFILE_NAME		"soyouz.log"


/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

World::World()
{
	mIOManager = 0;
	mRoot = 0;
	mOverlaySystem=0;
}


World::~World()
{
	if (mIOManager)
	{
		delete mIOManager;
	}
	if (mOverlaySystem)
	{
		if(mScene) mScene->removeRenderQueueListener(mOverlaySystem);
		OGRE_DELETE mOverlaySystem;
	}
	if (mRoot)
	{
		OGRE_DELETE mRoot;
	}

#ifdef OGRE_STATIC_LIB
	mStaticPluginLoader.unload();
#endif
}


/*----------------------------------------------
	Methods
----------------------------------------------*/

void World::run(void)
{
	setup();
	mRoot->startRendering();

	while (!mWindow->isClosed() && mRoot->renderOneFrame())
	{
	    WindowEventUtilities::messagePump();
	}
	destruct();

#ifdef USE_RTSHADER_SYSTEM
	finalizeShaderGenerator();
#endif
}


bool World::setup(void)
{
	setupResources();

	if (!mRoot->showConfigDialog())
	{
		return false;
	}

	setupRender();

	construct();

	mIOManager = new IOManager(mWindow, mCamera);
	mRoot->addFrameListener(mIOManager);
	return true;

}


void World::setupResources(void)
{
	ConfigFile cf;
	String pluginsPath, secName, typeName, archName;

	// Plugin loading
#ifndef OGRE_STATIC_LIB
	pluginsPath = CONFIG_PATH + PLUGINS_CONF;
#endif	
	mRoot = OGRE_NEW Root(pluginsPath, CONFIG_PATH + OGRE_CONF, CONFIG_PATH + LOGFILE_NAME);
	mOverlaySystem = OGRE_NEW OverlaySystem();
#ifdef OGRE_STATIC_LIB
	mStaticPluginLoader.load();
#endif

	// Resource loading
	cf.load(CONFIG_PATH + RESOURCES_CONF);
	ConfigFile::SectionIterator seci = cf.getSectionIterator();
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap::iterator i;
		ConfigFile::SettingsMultiMap *settings = seci.getNext();
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}
}


void World::setupRender(void)
{
	mWindow = mRoot->initialise(true);
	mScene = mRoot->createSceneManager(ST_GENERIC, "ExampleSMInstance");
	if(mOverlaySystem)
	{
			mScene->addRenderQueueListener(mOverlaySystem);
	}

	mCamera = mScene->createCamera("PlayerCam");
	mCamera->setPosition(Vector3(0,0,500));
	mCamera->lookAt(Vector3(0,0,-300));
	mCamera->setNearClipDistance(5);

	Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(ColourValue(0,0,0));
	mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
	
#ifdef USE_RTSHADER_SYSTEM
	initializeShaderGenerator(mSceneMgr);
#endif
	TextureManager::getSingleton().setDefaultNumMipmaps(5);
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}


#ifdef USE_RTSHADER_SYSTEM

bool World::setupShaderGenerator(SceneManager* sceneMgr)
{	
	if (RTShader::ShaderGenerator::initialize())
	{
		mShaderGenerator = RTShader::ShaderGenerator::getSingletonPtr();
		mShaderGenerator->addSceneManager(sceneMgr);

		ResourceGroupManager::LocationList resLocationsList = ResourceGroupManager::getSingleton().getResourceLocationList("Popular");
		ResourceGroupManager::LocationList::iterator it = resLocationsList.begin();
		ResourceGroupManager::LocationList::iterator itEnd = resLocationsList.end();
		String shaderCoreLibsPath;
		String shaderCachePath;

		shaderCachePath = "./";

		for (; it != itEnd; ++it)
		{

			if ((*it)->archive->getName().find("RTShaderLib") != String::npos)
			{
				shaderCoreLibsPath = (*it)->archive->getName() + "/";
				shaderCachePath    = shaderCoreLibsPath;
				break;
			}
		}

		if (shaderCoreLibsPath.empty())
		{
			return false;
		}

		ResourceGroupManager::getSingleton().addResourceLocation(shaderCoreLibsPath , "FileSystem");
		mShaderGenerator->setShaderCachePath(shaderCachePath);
		mMaterialMgrListener = new ShaderGeneratorTechniqueResolverListener(mShaderGenerator);	
		MaterialManager::getSingleton().addListener(mMaterialMgrListener);
	}
	return true;
}

void World::stopShaderGenerator()
{
	if (mMaterialMgrListener)
	{			
		MaterialManager::getSingleton().removeListener(mMaterialMgrListener);
		delete mMaterialMgrListener;
		mMaterialMgrListener = NULL;
	}
	if (mShaderGenerator)
	{
		RTShader::ShaderGenerator::finalize();
		mShaderGenerator = NULL;
	}
}

#endif
