/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/world.hpp"
#include "Engine/actor.hpp"
#include "Engine/player.hpp"

#define OGRE_CONF			"../../Config/soyouz.cfg"
#if OGRE_DEBUG_MODE
#  define PLUGINS_CONF		"../../Config/plugins_d.cfg"
#else
#  define PLUGINS_CONF		"../../Config/plugins.cfg"
#endif
#define RESOURCES_CONF		"../../Config/resources.cfg"
#define LOGFILE_NAME		"../../Config/soyouz.log"


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
	Public methods
----------------------------------------------*/

void World::run()
{
	setup();
	mRoot->startRendering();

	while (!mWindow->isClosed() && mRoot->renderOneFrame())
	{
	    WindowEventUtilities::messagePump();
	}
	destruct();

#ifdef USE_RTSHADER_SYSTEM
	killShaderGenerator();
#endif
}


SceneNode* World::createWorldNode(String name)
{
	return mScene->getRootSceneNode()->createChildSceneNode(name);
}


Entity* World::createWorldEntity(String name, String file)
{
	return mScene->createEntity(name, file);
}


/*----------------------------------------------
	Private methods
----------------------------------------------*/

bool World::setup()
{
	setupResources();
	if (!mRoot->showConfigDialog())
	{
		return false;
	}
	setupRender();

	construct();

	mIOManager = new IOManager(mWindow, mPlayer);
	mRoot->addFrameListener(mIOManager);
	return true;
}


void World::setupResources()
{
	ConfigFile cf;
	String pluginsPath, secName, typeName, archName;

	// Plugin loading
#ifndef OGRE_STATIC_LIB
	pluginsPath = PLUGINS_CONF;
#endif	
	mRoot = OGRE_NEW Root(pluginsPath, OGRE_CONF, LOGFILE_NAME);
	mOverlaySystem = OGRE_NEW OverlaySystem();
#ifdef OGRE_STATIC_LIB
	mStaticPluginLoader.load();
#endif

	// Resource loading
	cf.load(RESOURCES_CONF);
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


void World::setupRender()
{
	mWindow = mRoot->initialise(true);
	mScene = mRoot->createSceneManager(ST_GENERIC, "WorldScene");

	// Shadows
	mScene->setShadowTexturePixelFormat(Ogre::PF_FLOAT16_R);
	mScene->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);
	mScene->setShadowTextureSelfShadow(true);
	mScene->setShadowTextureSize(512);

	// Player
	if (mOverlaySystem)
	{
			mScene->addRenderQueueListener(mOverlaySystem);
	}
	setupPlayer();

	// Window
	Viewport* vp = mWindow->addViewport(mPlayer->getCamera());
	mPlayer->setCameraRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
	mScene->setAmbientLight(Ogre::ColourValue(0,0,0));
	vp->setBackgroundColour(ColourValue(0,0,0));
	
	// Engine settings
#ifdef USE_RTSHADER_SYSTEM
	setupShaderGenerator();
#endif
	TextureManager::getSingleton().setDefaultNumMipmaps(5);
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	MaterialManager::getSingleton().setDefaultTextureFiltering(TFO_ANISOTROPIC);
	MaterialManager::getSingleton().setDefaultAnisotropy(8);
}


void World::setupPlayer()
{
	mPlayer = new Player(this, "LocalPlayer", mScene);
}


#ifdef USE_RTSHADER_SYSTEM

bool World::setupShaderGenerator()
{	
	if (RTShader::ShaderGenerator::initialize())
	{
		mShaderGenerator = RTShader::ShaderGenerator::getSingletonPtr();
		mShaderGenerator->addSceneManager(mScene);

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

void World::killShaderGenerator()
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
