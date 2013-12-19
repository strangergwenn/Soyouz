/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __RENDERER_H
#define __RENDERER_H


#include "Ogre.h"
#include "OgreException.h"
#include "OgreConfigFile.h"
#include "OgreStringConverter.h"
#include "OgreSceneManager.h"
#include <OgreWindowEventUtilities.h>

#include <Overlay/OgreOverlaySystem.h>

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

#include "tinyxml2.hpp"


/*----------------------------------------------
	Deferred shading renderer
----------------------------------------------*/

class Renderer : public Ogre::RenderTargetListener
{

public:
	
	enum DSMode
	{
		DSM_SHOWLIT = 0,     // The deferred shading mode
		DSM_SHOWGBUFFER = 1,  // Show GBuffer (for debugging)
		DSM_SHOWSSAO = 2,    // Show SSAO
		DSM_NONE = 3		 // None
	};
	
	/**
	 * @brief Renderer constructor
	 * @param vp				Viewport
	 * @param sm				Scene manager
	 * @param s					System config
	 **/
	Renderer(Ogre::Viewport* vp, Ogre::SceneManager* sm,  tinyxml2::XMLElement* s);
	
	/**
	 * @brief Renderer destructor
	 **/
	~Renderer();
	
	/**
	 * @brief Set the current compositor mode
	 * @param mode				New rendering mode
	 **/
	void setMode(DSMode mode);

	
protected:
	
	// Scene data
	DSMode mCurrentMode;
	Ogre::Viewport* mViewport;
	Ogre::SceneManager* mScene;
	Ogre::CompositorInstance* mInstance[DSM_NONE];
};


#endif /* __RENDERER_H */
