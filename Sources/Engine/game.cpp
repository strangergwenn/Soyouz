/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/


#include "Engine/Rendering/renderer.hpp"
#include "Engine/game.hpp"
#include "Engine/actor.hpp"
#include "Engine/player.hpp"


#define OGRE_CONF			"Config/soyouz.cfg"
#if OGRE_DEBUG_MODE
#  define PLUGINS_CONF		"Config/plugins_d.cfg"
#else
#  define PLUGINS_CONF		"Config/plugins.cfg"
#endif
#define RESOURCES_CONF		"Config/resources.cfg"
#define LOGFILE_NAME		"Config/soyouz.log"


/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

Game::Game()
{
	mRoot = NULL;
	mIOManager = NULL;
	mPhysWorld = NULL;
	mOverlaySystem = NULL;
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
}


/*----------------------------------------------
	Public methods
----------------------------------------------*/

void Game::run()
{
	setup();
	mRoot->startRendering();
	while (!mWindow->isClosed() && mRoot->renderOneFrame())
	{
	    Ogre::WindowEventUtilities::messagePump();
	}
	destruct();
}


void Game::tick(const Ogre::FrameEvent& evt)
{
	Actor* ref;

	// Actor pre-tick
	for (Ogre::list<Actor*>::iterator it = mAllActors.begin(); it != mAllActors.end(); it++)
    {
		ref = *it;
        ref->preTick(evt);
    }

	// Actor tick
	for (Ogre::list<Actor*>::iterator it = mAllActors.begin(); it != mAllActors.end(); it++)
    {
		ref = *it;
        ref->tick(evt);
    }

	// Debug physics
	mPhysDrawer->step();
	
	// Physics tick
	if (mPhysWorld)
	{
		mPhysWorld->stepSimulation(evt.timeSinceLastFrame);
	}
}
	

void Game::registerActor(Actor* ref)
{
	mAllActors.push_back(ref);
}
	

void Game::unregisterActor(Actor* ref)
{
	mAllActors.remove(ref);
}


void Game::deleteActor(Actor* target)
{
	deleteGameNode(target->getNode());
	delete[] target;
}


Ogre::SceneNode* Game::createGameNode(String name)
{
	return mScene->getRootSceneNode()->createChildSceneNode(name);
}


void Game::deleteGameNode(Ogre::SceneNode* node)
{
	mScene->destroySceneNode(node);
}


Ogre::Entity* Game::createGameEntity(String name, String file)
{
	return mScene->createEntity(name, file);
}


void Game::deleteGameEntity(Ogre::Entity* entity)
{
	mScene->destroyEntity(entity);
}


void Game::registerRigidBody(btRigidBody* body)
{
	mPhysWorld->addRigidBody(body);
}


void Game::unregisterRigidBody(btRigidBody* body)
{
	mPhysWorld->removeRigidBody(body);
}


void Game::quit()
{
	bRunning = false;
}


Ogre::SceneManager* Game::getScene()
{
	return mScene;
}


/*----------------------------------------------
	Events
----------------------------------------------*/

bool Game::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	tick(evt);
	mIOManager->prerender(evt);
	return bRunning;
}


bool Game::frameEnded(const Ogre::FrameEvent& evt)
{
	mIOManager->postrender(evt);
	return bRunning;
}


/*----------------------------------------------
	Debug utility
----------------------------------------------*/
 
void Game::gameLog(String text)
{
	Ogre::LogManager::getSingletonPtr()->logMessage(text.c_str());
}


void Game::setDebugMode(int newStatus)
{
	mPhysDrawer->setDebugMode(0);
	switch (newStatus)
	{
	case 0:
		SharedData::getSingleton().iSystem->setMode(Renderer::DSM_SHOWLIT);
		break;

	case 1:
		SharedData::getSingleton().iSystem->setMode(Renderer::DSM_SHOWCOLOUR);
		break;

	case 2:
		SharedData::getSingleton().iSystem->setMode(Renderer::DSM_SHOWCOLOUR);
		mPhysDrawer->setDebugMode(1);
		break;

	case 3:
		SharedData::getSingleton().iSystem->setMode(Renderer::DSM_SHOWNORMALS);
		break;

	case 4:
		SharedData::getSingleton().iSystem->setMode(Renderer::DSM_SHOWDSP);
		break;

	case 5:
		SharedData::getSingleton().iSystem->setMode(Renderer::DSM_SHOWGLOW);
		break;

	case 6:
		SharedData::getSingleton().iSystem->setMode(Renderer::DSM_SHOWSSAO);
		break;

	default:
		break;
	}
}


Ogre::ManualObject* Game::getDebugLine(Vector3 line, String name, String material)
{
	Ogre::ManualObject* tmp = mScene->createManualObject(name); 
	tmp->begin(material, Ogre::RenderOperation::OT_LINE_LIST); 
	tmp->position(0, 0, 0); 
	tmp->position(line.x, line.y, line.z); 
	tmp->end(); 
	return tmp;
}


void Game::dumpAllNodes()
{
	std::stringstream ss;
	ss << std::endl << "Game::dumpNodes" << std::endl;
	dumpNodes(ss, mScene->getRootSceneNode(), 0);
	gameLog(ss.str());
}


/*----------------------------------------------
	Protected methods
----------------------------------------------*/

bool Game::setup()
{
	setupResources();
	setupSystem("OpenGL");
	setupPhysics(Vector3(0, 0, 0), false);
	setupRender(true);
	construct();
	return true;
}


void Game::setupResources()
{
	Ogre::ConfigFile cf;
	String pluginsPath, secName, typeName, archName;
	
	// Plugins
	pluginsPath = PLUGINS_CONF;
	mRoot = new Ogre::Root(pluginsPath, OGRE_CONF, LOGFILE_NAME);
	mOverlaySystem = new Ogre::OverlaySystem();

	// Resources
	cf.load(RESOURCES_CONF);
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}
}


bool Game::setupSystem(const String desiredRenderer)
{
    bool bRes = false;
	Ogre::RenderSystemList rdrs;
    Ogre::RenderSystem* renderSystem;
	
	// Render systems list
    rdrs = mRoot->getAvailableRenderers();
    if (rdrs.empty())
	{
        return false;
	}

	// Search a correct render system
    for (Ogre::RenderSystemList::iterator it = rdrs.begin(); it != rdrs.end(); it++)
    {
        renderSystem = (*it);
		Ogre::Log(renderSystem->getName());
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
	renderSystem->setConfigOption("Fixed Pipeline Enabled", "Yes");
	renderSystem->setConfigOption("Full Screen", "No");
	renderSystem->setConfigOption("RTT Preferred Mode", "FBO");
	renderSystem->setConfigOption("VSync", "No");
    renderSystem->setConfigOption("Video Mode", "1280 x 720");
	mWindow = mRoot->initialise(true, "Soyouz");
	mScene = mRoot->createSceneManager(Ogre::ST_GENERIC, "GameScene");
    return true;
}


void Game::setupRender(bool bShowPostProcess)
{
	// Render resources
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Boot");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Core");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Game");

	// Player
	if (mOverlaySystem)
	{
			mScene->addRenderQueueListener(mOverlaySystem);
	}
	setupPlayer();
	Ogre::Camera* cam = mPlayer->getCamera();

	// Window
	Ogre::Viewport* vp = mWindow->addViewport(cam);
	mPlayer->setCameraRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
	mScene->setAmbientLight(Ogre::ColourValue(0,0,0));
	vp->setBackgroundColour(Ogre::ColourValue(0.1f, 0.1f, 0.1f));
	
	// IO manager
	mIOManager = new IOManager(mWindow, mPlayer, this);
	mRoot->addFrameListener(this);

	// Deferred rendering setup
	mRenderer = new Renderer(mWindow->getViewport(0), mScene, mPlayer->getCamera());
	SharedData::getSingleton().iCamera = cam;
	SharedData::getSingleton().iRoot = mRoot;
	SharedData::getSingleton().iWindow = mWindow;
	mRenderer->setMode(Renderer::DSM_SHOWLIT);
}


void Game::setupPhysics(Vector3 gravity, bool bDrawDebug)
{
	mPhysCollisionConfiguration = new btDefaultCollisionConfiguration();
	mPhysDispatcher = new btCollisionDispatcher(mPhysCollisionConfiguration);
	mPhysBroadphase = new btDbvtBroadphase();
	mPhysSequentialImpulseConstraintSolver = new btSequentialImpulseConstraintSolver;

	mPhysWorld = new btDiscreteDynamicsWorld(
		mPhysDispatcher,
		mPhysBroadphase,
		mPhysSequentialImpulseConstraintSolver,
		mPhysCollisionConfiguration);

	mPhysWorld->setGravity(btVector3(gravity[0], gravity[1], gravity[2]));

	mPhysDrawer = new DebugDrawer(mScene, mScene->getRootSceneNode(), mPhysWorld);
	mPhysDrawer->setDebugMode(bDrawDebug ? 1:0);
	mPhysWorld->setDebugDrawer(mPhysDrawer);
}


void Game::setupPlayer()
{
	mPlayer = new Player(this, "LocalPlayer");
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

