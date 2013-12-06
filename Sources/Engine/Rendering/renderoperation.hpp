/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef _RENDER_OPERATION_H
#define _RENDER_OPERATION_H


#include "Engine/Rendering/renderer.hpp"
#include "Engine/Rendering/ambient.hpp"
#include "Engine/Rendering/deferredlight.hpp"
#include "Engine/Rendering/lightmaterial.hpp"
#include "OgreCustomCompositionPass.h"


/*----------------------------------------------
	Deferred shading operation
----------------------------------------------*/

class RenderOperation : public Ogre::CompositorInstance::RenderSystemOperation
{

public:
	
	/**
	 * @brief Render operation constructor
	 * @param instance			Current compositor
	 * @param pass				Pass data
	 **/
	RenderOperation(Ogre::CompositorInstance* instance, const Ogre::CompositionPass* pass);
	
	/**
	 * @brief Render operation destructor
	 **/
	virtual ~RenderOperation();
	
	/**
	 * @brief Render operation execution process
	 * @param sm				Current scene manager
	 * @param rs				Renderer
	 **/
	virtual void execute(Ogre::SceneManager* sm, Ogre::RenderSystem* rs);


private:
	
	/**
	 * @brief Create a deferred light from an actual one
	 * @param light				Light entity
	 * @return the new light
	 **/
	DeferredLight* createDLight(Ogre::Light* light);
	

	// Scene data
	Ogre::String mTexName0;
	Ogre::String mTexName1;
	Ogre::Viewport* mViewport;

	// Material generator
	LightMaterialGenerator* mLightMaterialGenerator;

	// Lights
	typedef std::map<Ogre::Light*, DeferredLight*> LightsMap;
	LightsMap mLights;
	AmbientLight* mAmbientLight;

};


/*----------------------------------------------
	Custom pass to set the render operation
----------------------------------------------*/

class DeferredRenderPass : public Ogre::CustomCompositionPass
{

public:

	virtual Ogre::CompositorInstance::RenderSystemOperation* createOperation(
		Ogre::CompositorInstance* instance, const Ogre::CompositionPass* pass)
	{
		return new RenderOperation(instance, pass);
	}

protected:

	virtual ~DeferredRenderPass()
	{
	}
};


#endif
