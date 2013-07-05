/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/world.h"


/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

World::World()
{
	mIOManager = 0;
	mRoot = 0;
	mOverlaySystem=0;

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	mResourcePath = macBundlePath() + "/Contents/Resources/";
	mConfigPath = mResourcePath;
#else
	mResourcePath = "";
	mConfigPath = mResourcePath;
#endif
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

	destruct();

#ifdef USE_RTSHADER_SYSTEM
		finalizeShaderGenerator();
#endif
}


bool World::setup(void)
{
    setupResources();

    bool carryOn = mRoot->showConfigDialog();
    if (carryOn)
	{
        mWindow = mRoot->initialise(true);
	}
	else
	{
		return false;
	}
	
    setupRender();

	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    construct();
	
    mIOManager= new IOManager(mWindow, mCamera);
    mIOManager->showDebugOverlay(true);
    mRoot->addFrameListener(mIOManager);

    return true;

}


void World::setupRender(void)
{
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
}


void World::setupResources(void)
{
    ConfigFile cf;
	String pluginsPath;

#ifndef OGRE_STATIC_LIB
#if OGRE_DEBUG_MODE
	pluginsPath = mResourcePath + "plugins_d.cfg";
#else
	pluginsPath = mResourcePath + "plugins.cfg";
#endif
#endif
		
    mRoot = OGRE_NEW Root(pluginsPath, 
        mConfigPath + "ogre.cfg", mResourcePath + "Ogre.log");
	mOverlaySystem = OGRE_NEW OverlaySystem();

#ifdef OGRE_STATIC_LIB
	mStaticPluginLoader.load();
#endif
#if OGRE_DEBUG_MODE
    cf.load(mResourcePath + "resources_d.cfg");
#else
	cf.load(mResourcePath + "resources.cfg");
#endif

    ConfigFile::SectionIterator seci = cf.getSectionIterator();

    String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
			if (!StringUtil::startsWith(archName, "/", false))
				archName = String(macBundlePath() + "/" + archName);
#endif
            ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);

        }
    }
}


#ifdef USE_RTSHADER_SYSTEM

bool World::initializeShaderGenerator(SceneManager* sceneMgr)
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

void World::finalizeShaderGenerator()
{
	if (mMaterialMgrListener != NULL)
	{			
		MaterialManager::getSingleton().removeListener(mMaterialMgrListener);
		delete mMaterialMgrListener;
		mMaterialMgrListener = NULL;
	}
	if (mShaderGenerator != NULL)
	{
		RTShader::ShaderGenerator::finalize();
		mShaderGenerator = NULL;
	}
}

#endif
