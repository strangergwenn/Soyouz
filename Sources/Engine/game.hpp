/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/


#ifndef __GAME_H_
#define __GAME_H_

#include "Engine/ogre.hpp"
#include "Engine/bullet.hpp"
#include "Engine/iomanager.hpp"

class Actor;
class Player;
class PointLight;


/*----------------------------------------------
	Post-processing compositor
----------------------------------------------*/

class PostProcessListener : public Ogre::MaterialManager::Listener
{
protected:
	Ogre::MaterialPtr mBlackMat;

public:
	PostProcessListener()
	{
		mBlackMat = Ogre::MaterialManager::getSingleton().create("mGlowBlack", "Internal");
		mBlackMat->getTechnique(0)->getPass(0)->setDiffuse(0,0,0,0);
		mBlackMat->getTechnique(0)->getPass(0)->setSpecular(0,0,0,0);
		mBlackMat->getTechnique(0)->getPass(0)->setAmbient(0,0,0);
		mBlackMat->getTechnique(0)->getPass(0)->setSelfIllumination(0,0,0);
	}
 
	Ogre::Technique *handleSchemeNotFound(unsigned short, const Ogre::String& schemeName, Ogre::Material*mat, unsigned short, const Ogre::Renderable*)
	{
		return mBlackMat->getTechnique(0);
	}
};


/*----------------------------------------------
	Game class definition
----------------------------------------------*/

class Game
{

public:
	
	/**
	 * @brief Load the world
	 **/
	Game();

	/**
	 * @brief Unload the world
	 **/
	virtual ~Game();

	/**
	 * @brief Run the level (blocking)
	 **/
	virtual void run();
	
	/**
	 * @brief Main tick event
	 * @param evt			Frame event
	 **/
	virtual void tick(const FrameEvent& evt);
	
	/**
	 * @brief Register an actor to the world
	 * @param ref				Actor reference
	 **/
	void registerActor(Actor* ref);
	
	/**
	 * @brief Unregister an actor to the world
	 * @param ref				Actor reference
	 **/
	void unregisterActor(Actor* ref);

	/**
	 * @brief Run the level (blocking)
	 * @param name					Node name
	 * @return the scene node
	 **/
	SceneNode* createGameNode(String name);
	
	/**
	 * @brief Run the level (blocking)
	 * @param name					Node name
	 * @param name					File name
	 * @return the scene entity
	 **/
	Entity* createGameEntity(String name, String file);
	
	/**
	 * @brief Register a rigid body to the scene
	 * @param body					Rigid body
	 **/
	void addRigidBody(btRigidBody* body);
	
	/**
	 * @brief Write text to the log file
	 * @param text				Input data
	 **/
	virtual void gameLog(String text);
 
	/**
	 * @brief Dump all scene nodes to the log file
	 **/
	void dumpAllNodes();
	
	/**
	 * @brief Quit the game
	 **/
	virtual void quit();
	
	/**
	 * @brief Get the current scene manager
	 * @return the scene
	 **/
	SceneManager* getScene();


protected:

	/**
	 * @brief Level construction
	 **/
	virtual void construct() = 0;

	/**
	 * @brief Level destruction
	 **/
	virtual void destruct() = 0;
	
	/**
	 * @brief Setup the level
	 **/
	virtual bool setup();

	/**
	 * @brief Setup resources
	 **/
	virtual void setupResources();
	
	/**
	 * @brief Setup the render sytsem
	 * @param desiredRenderer	Render system to use
	 **/
	virtual bool setupSystem(const String desiredRenderer);

	/**
	 * @brief Setup rendering methods
	 * @param bShowPostProcess	Should show post processing
	 **/
	virtual void setupRender(bool bShowPostProcess = true);
	
	/**
	 * @brief Setup the player
	 **/
	virtual void setupPlayer();

	/**
	 * @brief Setup the physics world
	 * @param gravity			Gravity vector
	 * @param bDrawDebug		Should display hulls
	 **/
	virtual void setupPhysics(Vector3 gravity, bool bDrawDebug = false);
	
	/**
	 * @brief Dump a node to a string stream
	 * @param ss				Output stream
	 * @param n					Node
	 * @param leveln			Current recursion depth
	 * @return the scene entity
	 **/
	void dumpNodes(std::stringstream &ss, Ogre::Node *n, int level);

	
	// OGRE data
	Root* mRoot;
	SceneManager* mScene;
	RenderWindow* mWindow;
	OverlaySystem* mOverlaySystem;

	// Bullet data
	DebugDrawer* mDebugDrawer;
	btDiscreteDynamicsWorld* mPhysWorld;
	btBroadphaseInterface* mPhysBroadphase;
	btCollisionDispatcher* mPhysDispatcher;
	btDefaultCollisionConfiguration* mPhysCollisionConfiguration;
	btSequentialImpulseConstraintSolver* mPhysSequentialImpulseConstraintSolver;

	// Custom data
	Player* mPlayer;
	IOManager* mIOManager;

	list<Actor*>::type mAllActors;

#ifdef OGRE_STATIC_LIB
	StaticPluginLoader mStaticPluginLoader;
#endif

};

#endif /* __GAME_H_ */

