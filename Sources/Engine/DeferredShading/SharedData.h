
#ifndef __SHAREDDATA_H
#define __SHAREDDATA_H

#include "DeferredShading.h"

class SharedData : public Ogre::Singleton<SharedData> {

public:

	SharedData()
		: iRoot(0),
		  iCamera(0),
		  iWindow(0),
		  mMLAnimState(0),
		  iMainLight(0)
	{
		iActivate = false;
	}

		~SharedData() {}

		// shared data across the application
		Ogre::Real iLastFrameTime;
		Ogre::Root *iRoot;
		Ogre::Camera *iCamera;
		Ogre::RenderWindow *iWindow;

		DeferredShadingSystem *iSystem;
		bool iActivate;
		bool iGlobalActivate;

		// Animation state for light swarm
		Ogre::AnimationState* mMLAnimState;

		Ogre::Light *iMainLight;

		Ogre::vector<Ogre::Node*>::type mLightNodes;

};

#endif
