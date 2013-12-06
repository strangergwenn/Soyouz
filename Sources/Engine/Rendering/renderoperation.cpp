/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/Rendering/renderoperation.hpp"


/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

RenderOperation::RenderOperation(Ogre::CompositorInstance* instance, const Ogre::CompositionPass* pass)
{
	mViewport = instance->getChain()->getViewport();
	
	// Get GBuffer texture data for lighting (only the first two)
	const Ogre::CompositionPass::InputTex& input0 = pass->getInput(0);
	mTexName0 = instance->getTextureInstanceName(input0.name, input0.mrtIndex);
	const Ogre::CompositionPass::InputTex& input1 = pass->getInput(1);
	mTexName1 = instance->getTextureInstanceName(input1.name, input1.mrtIndex);

	// Create the lighting data
	mLightMaterialGenerator = new LightMaterialGenerator();
	mAmbientLight = new AmbientLight();
	const Ogre::MaterialPtr& mat = mAmbientLight->getMaterial();
	mat->load();
}


RenderOperation::~RenderOperation()
{
	for (LightsMap::iterator it = mLights.begin(); it != mLights.end(); ++it)
	{
		delete it->second;
	}
	mLights.clear();
	
	delete mAmbientLight;
	delete mLightMaterialGenerator;
}


/*----------------------------------------------
	Methods
----------------------------------------------*/

DeferredLight* RenderOperation::createDLight(Ogre::Light* light)
{
	DeferredLight *rv = new DeferredLight(mLightMaterialGenerator,light);
	mLights[light] = rv;
	return rv;
}


void injectTechnique(Ogre::SceneManager* sm, Ogre::Technique* tech, Ogre::Renderable* rend, const Ogre::LightList* lightList)
{
    for (unsigned short i = 0; i < tech->getNumPasses(); ++i)
	{
		Ogre::Pass* pass = tech->getPass(i);
        if (lightList != 0) 
		{
			sm->_injectRenderWithPass(pass, rend, false, false, lightList);
		} 
		else
		{
			sm->_injectRenderWithPass(pass, rend, false);
		}
	}
}


void RenderOperation::execute(Ogre::SceneManager *sm, Ogre::RenderSystem *rs)
{
    Ogre::Camera* cam = mViewport->getCamera();

	// Compute the ambient lighting
	mAmbientLight->updateFromCamera(cam);
    Ogre::Technique* tech = mAmbientLight->getMaterial()->getBestTechnique();
	injectTechnique(sm, tech, mAmbientLight, 0);

	// For each light, compute the lighting 
	const Ogre::LightList& lightList = sm->_getLightsAffectingFrustum();
    for (Ogre::LightList::const_iterator it = lightList.begin(); it != lightList.end(); it++) 
	{
        Ogre::Light* light = *it;
		Ogre::LightList ll;
		ll.push_back(light);

		// Get every light in the scene, create a deferred light
		LightsMap::iterator dLightIt = mLights.find(light);
		DeferredLight* dLight = 0;
		if (dLightIt == mLights.end()) 
		{
			dLight = createDLight(light);
		}
		else 
		{
			dLight = dLightIt->second;
			dLight->updateFromParent();
		}
		dLight->updateFromCamera(cam);
		tech = dLight->getMaterial()->getBestTechnique();

		// Update shadow textures
		if (dLight->getCastShadows())
		{
			// Render textures
			Ogre::SceneManager::RenderContext* context = sm->_pauseRendering();
			sm->prepareShadowTextures(cam, mViewport, &ll);
			sm->_resumeRendering(context);
			
			// Load textures
			Ogre::Pass* pass = tech->getPass(0);
			Ogre::TextureUnitState* tus = pass->getTextureUnitState("ShadowMap");
			assert(tus);

			// Assign textures
			const Ogre::TexturePtr& shadowTex = sm->getShadowTexture(0);
			if (tus->_getTexturePtr() != shadowTex)
			{
				tus->_setTexturePtr(shadowTex);
			}
			
		}
        injectTechnique(sm, tech, dLight, &ll);
	}
}
