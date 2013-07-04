#include "ExampleApplication.h"
 
class Test : public ExampleApplication
{
public:
    void createScene()
    {
		mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
		SceneNode *node= mSceneMgr->getRootSceneNode()->createChildSceneNode("nodeTete");
		Entity *head= mSceneMgr->createEntity("Tete", "penguin.mesh");
		node->attachObject(head);

		Plane plan(Vector3::UNIT_Y, 0);
		MeshManager::getSingleton().createPlane("sol", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plan, 500, 500, 1, 1, true, 1, 1, 1, Vector3::UNIT_Z);

		Entity *ent= mSceneMgr->createEntity("EntiteSol", "sol");
		node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		//node->attachObject(ent);
    }
};

