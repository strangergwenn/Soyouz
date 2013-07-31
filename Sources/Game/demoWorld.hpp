/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __DEMOWORLD_H_
#define __DEMOWORLD_H_

#include "Engine/game.hpp"
#include "Engine/actor.hpp"
#include "Engine/player.hpp"
#include "Engine/pointlight.hpp"


/*----------------------------------------------
Class definitions
----------------------------------------------*/

class DemoWorld : public Game
{

public:
	
	void construct()
	{
		Actor* act1 = new Actor(this, "DemoMesh1", "Airlock.mesh", "Material");
		act1->translate(Vector3(-256,-128,0));
		act1->rotate(Vector3(0, 90, 0));

		Actor* act2 = new Actor(this, "DemoMesh2", "Airlock.mesh", "Material");
		act2->translate(Vector3(-256,128,0));
		act2->rotate(Vector3(0, 90, 0));
		
		Actor* penguin = new Actor(this, "penguin", "penguin.mesh", "Penguin");
		penguin->translate(Vector3(100,-100,-100));
		
		PointLight* light1 = new PointLight(this, "Light1", mScene, Vector3(0.2, 1.0, 0.6), 500);
		light1->translate(Vector3(200, 100, -200));

		PointLight* light2 = new PointLight(this, "Light2", mScene, Vector3(1.0, 0.6, 0.2), 500);
		light2->translate(Vector3(-200, 100, -200));

		PointLight* light3 = new PointLight(this, "Light3", mScene, Vector3(1.0, 1.0, 1.0), 500);
		light3->translate(Vector3(0, 200, -0));

		// Ground plane
		Plane plane(Vector3::UNIT_Y, 0);
		MeshManager::getSingleton().createPlane("ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 512, 512, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
		Entity* entGround = mScene->createEntity("GroundEntity", "ground");
		entGround->setMaterialName("Material");
		entGround->setCastShadows(false);
		SceneNode* groundNode = mScene->getRootSceneNode()->createChildSceneNode();
		groundNode->attachObject(entGround);
		groundNode->setPosition(0, -256, 0);
		
		// Wall plane
		Plane plane2(Vector3::UNIT_Z, 0);
		MeshManager::getSingleton().createPlane("wall", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane2, 512, 512, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
		Entity* entWall = mScene->createEntity("WallEntity", "wall");
		entWall->setMaterialName("Material");
		entWall->setCastShadows(false);
		SceneNode* wallNode = mScene->getRootSceneNode()->createChildSceneNode();
		wallNode->attachObject(entWall);
		wallNode->setPosition(0, 0,-256);

		// Sphere
		Ogre::SceneNode* mSceneNode = mScene->getRootSceneNode()->createChildSceneNode();
		Ogre::Entity* mEntity = mScene->createEntity("sphere", Ogre::SceneManager::PT_SPHERE);
		mEntity->setMaterialName("Material");
		mEntity->setCastShadows(true);
		mSceneNode->attachObject(mEntity);
		mSceneNode->setPosition(Ogre::Vector3(-100,-100,-100));
	}

	void destruct(){}
	
};


#endif /* __DEMOWORLD_H_ */
