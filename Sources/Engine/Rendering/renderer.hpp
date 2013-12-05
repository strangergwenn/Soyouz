/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __OGREINC_H
#define __OGREINC_H


/*----------------------------------------------
	Definitions and includes
----------------------------------------------*/

#include "Ogre.h"
#include "OgreException.h"
#include "OgreConfigFile.h"
#include "OgreStringConverter.h"
#include "OgreSceneManager.h"
#include "OgreCompositorLogic.h"
#include <OgreWindowEventUtilities.h>

#include <Overlay/OgreOverlaySystem.h>

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>


/*----------------------------------------------
	Deferred shading renderer
----------------------------------------------*/

class Renderer : public Ogre::RenderTargetListener
{
public:
	Renderer(Ogre::Viewport *vp, Ogre::SceneManager *sm, Ogre::Camera *cam);
	~Renderer();

	enum DSMode
	{
		DSM_SHOWLIT = 0,     // The deferred shading mode
		DSM_SHOWCOLOUR = 1,  // Show diffuse (for debugging)
		DSM_SHOWNORMALS = 2, // Show normals (for debugging)
		DSM_SHOWDSP = 3,	 // Show depth and specular channel (for debugging)
		DSM_SHOWGLOW = 4,	 // Show glow maps
		DSM_SHOWSSAO = 5,    // Show SSAO
		DSM_NONE = 6		 // None
	};

	static const Ogre::uint8 PRE_GBUFFER_RENDER_QUEUE;
	static const Ogre::uint8 POST_GBUFFER_RENDER_QUEUE;

	void initialize();

	void setMode(DSMode mode);

	
protected:
	Ogre::Viewport *mViewport;
	Ogre::SceneManager *mSceneMgr;
	Ogre::Camera *mCamera;
	
	Ogre::CompositorInstance *mGBufferInstance;
	Ogre::CompositorInstance *mInstance[DSM_NONE];

	DSMode mCurrentMode;

	typedef Ogre::map<Ogre::String, Ogre::CompositorLogic*>::type CompositorLogicMap;
	CompositorLogicMap mCompositorLogics;

	void createResources();
};


/*----------------------------------------------
	Shared data
----------------------------------------------*/

class SharedData : public Ogre::Singleton<SharedData>
{
public:

	SharedData()
		: iRoot(0),
			iCamera(0),
			iWindow(0)
	{
	}

	~SharedData() {}

	Ogre::Real iLastFrameTime;
	Ogre::Root *iRoot;
	Ogre::Camera *iCamera;
	Ogre::RenderWindow *iWindow;

	Renderer *iSystem;
	Ogre::vector<Ogre::Node*>::type mLightNodes;

};


#endif /* __OGREINC_H */
