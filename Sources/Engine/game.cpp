/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/game.hpp"
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

Game::Game()
{
	mIOManager = 0;
	mRoot = 0;
	mOverlaySystem=0;
}


Game::~Game()
{
	if (mIOManager)
	{
		delete mIOManager;
	}
	if (mOverlaySystem)
	{
		if(mScene) mScene->removeRenderQueueListener(mOverlaySystem);
		delete mOverlaySystem;
	}
	if (mRoot)
	{
		delete mRoot;
	}

#ifdef OGRE_STATIC_LIB
	mStaticPluginLoader.unload();
#endif
}


/*----------------------------------------------
	Public methods
----------------------------------------------*/

void Game::run()
{
	setup(false);
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


SceneNode* Game::createGameNode(String name)
{
	return mScene->getRootSceneNode()->createChildSceneNode(name);
}


Entity* Game::createGameEntity(String name, String file)
{
	return mScene->createEntity(name, file);
}


void Game::dumpAllNodes()
{
	std::stringstream ss;
	ss << std::endl << "Game::dumpNodes" << std::endl;
	dumpNodes(ss, mScene->getRootSceneNode(), 0);
	Log(ss.str());
}

 
void Game::Log(String text)
{
	LogManager::getSingletonPtr()->logMessage(text.c_str());
}


/*----------------------------------------------
	Protected methods
----------------------------------------------*/


bool Game::setup(bool bShowConfig)
{
	setupResources();
	setupSystem("OpenGL");
	setupRender();

	construct();
	return true;
}


void Game::setupResources()
{
	ConfigFile cf;
	String pluginsPath, secName, typeName, archName;
	
	// Plugins
#ifndef OGRE_STATIC_LIB
	pluginsPath = PLUGINS_CONF;
#endif	
	mRoot = new Root(pluginsPath, OGRE_CONF, LOGFILE_NAME);
	mOverlaySystem = new OverlaySystem();
#ifdef OGRE_STATIC_LIB
	mStaticPluginLoader.load();
#endif

	// Resources
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


bool Game::setupSystem(const String desiredRenderer)
{
    bool bRes = false;
	RenderSystemList rdrs;
    RenderSystem* renderSystem;
	
	// Render systems list
    rdrs = mRoot->getAvailableRenderers();
    if (rdrs.empty())
	{
        return false;
	}

	// Search a correct render system
    for (RenderSystemList::iterator it = rdrs.begin(); it != rdrs.end(); it++)
    {
        renderSystem = (*it);
		Log(renderSystem->getName());
        if (StringUtil::startsWith(renderSystem->getName(), desiredRenderer, false))
        {
            bRes = true;
            break;
        }
    }

	// Selection
    if (!bRes)
	{
        renderSystem = *(rdrs.begin());
    }
    mRoot->setRenderSystem(renderSystem);
    renderSystem->setConfigOption("Full Screen", "No");
    renderSystem->setConfigOption("Video Mode", "1280 x 720");
    return true;
}


void Game::setupRender()
{
	mWindow = mRoot->initialise(true, "Soyouz");
	mScene = mRoot->createSceneManager(ST_GENERIC, "GameScene");

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

	// IO manager
	mIOManager = new IOManager(mWindow, mPlayer);
	mRoot->addFrameListener(mIOManager);

	// Shadows
	mScene->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	mScene->setShadowTexturePixelFormat(Ogre::PF_FLOAT32_R);
	mScene->setShadowTextureSelfShadow(false);
	mScene->setShadowTextureSize(512);
}


void Game::setupPlayer()
{
	mPlayer = new Player(this, "LocalPlayer", mScene);
}


void Game::dumpNodes(std::stringstream &ss, Ogre::Node* n, int level)
{
	for (int i = 0; i < level; i++)
	{
		ss << " ";
	}
	ss << "SceneNode: " << n->getName() << std::endl;
 
	Ogre::SceneNode::ObjectIterator object_it = ((Ogre::SceneNode *)n)->getAttachedObjectIterator();
	Ogre::Node::ChildNodeIterator node_it = n->getChildIterator();
	Ogre::MovableObject *m;

	while (object_it.hasMoreElements())
	{
		for (int i = 0; i < level + 2; i++)
		{
			ss << " ";
		}
		m = object_it.getNext();
		ss << m->getMovableType() << ": " << m->getName() << std::endl;
	}
	while (node_it.hasMoreElements())
	{
		dumpNodes(ss, node_it.getNext(), level + 2);
	}
}


#ifdef USE_RTSHADER_SYSTEM

bool Game::setupShaderGenerator()
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

void Game::killShaderGenerator()
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
