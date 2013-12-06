/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/Rendering/renderer.hpp"
#include "Engine/Rendering/renderoperation.hpp"


/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

Renderer::Renderer(Ogre::Viewport* vp, Ogre::SceneManager* sm)
	: mViewport(vp), mScene(sm)
{
	// Texture filtering
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
	Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(4);
	
	// General render settings
	vp->setBackgroundColour(Ogre::ColourValue(0.1f, 0.1f, 0.1f));
	sm->setAmbientLight(Ogre::ColourValue(0,0,0));
	sm->setShadowTextureCount(1);
	sm->setShadowFarDistance(200);
	sm->setShadowDirectionalLightExtrusionDistance(200);
	sm->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
	sm->setShadowTextureConfig(0, 512, 512, Ogre::PF_FLOAT16_R, 2 );
	sm->setShadowTextureCasterMaterial("Render/ShadowCaster");
	
	// GBuffer creation
	Ogre::CompositorManager &compMan = Ogre::CompositorManager::getSingleton();
	compMan.registerCustomCompositionPass("DeferredLight", new DeferredRenderPass);
	Ogre::CompositorInstance* gbuffer = compMan.addCompositor(mViewport, "DeferredShading/GBuffer");
	gbuffer->setEnabled(true);
	
	// Prepare instances
	for (int i = 0; i < DSM_NONE; ++i)
	{
		mInstance[i] = 0;
	}

	// Load instances
	mInstance[DSM_SHOWLIT] =		compMan.addCompositor(mViewport, "DeferredShading/ShowLit");
	mInstance[DSM_SHOWNORMALS] =	compMan.addCompositor(mViewport, "DeferredShading/ShowNormals");
	mInstance[DSM_SHOWDSP] =		compMan.addCompositor(mViewport, "DeferredShading/ShowDepthSpecular");
	mInstance[DSM_SHOWCOLOUR] =		compMan.addCompositor(mViewport, "DeferredShading/ShowColour");
	mInstance[DSM_SHOWGLOW] =		compMan.addCompositor(mViewport, "DeferredShading/ShowGlow");
	mInstance[DSM_SHOWSSAO] =		compMan.addCompositor(mViewport, "DeferredShading/ShowSSAO");

	// Ready up
	mCurrentMode = DSM_NONE;
	setMode(mCurrentMode);
}


Renderer::~Renderer()
{
	// Chain deletion
	Ogre::CompositorChain *chain = Ogre::CompositorManager::getSingleton().getCompositorChain(mViewport);
	for(int i = 0; i < DSM_NONE; ++i)
	{
		chain->_removeInstance(mInstance[i]);
	}
	Ogre::CompositorManager::getSingleton().removeCompositorChain(mViewport);

	// Prepare iterators
	Ogre::CompositorManager& compMgr = Ogre::CompositorManager::getSingleton();
	CompositorLogicMap::const_iterator itor = mCompositorLogics.begin();
	CompositorLogicMap::const_iterator end  = mCompositorLogics.end();

	// Clear all compositors
	while (itor != end)
	{
		compMgr.unregisterCompositorLogic(itor->first);
		delete itor->second;
		++itor;
	}
	mCompositorLogics.clear();
}


/*----------------------------------------------
	Compositor switch
----------------------------------------------*/

void Renderer::setMode(DSMode mode)
{
	assert( 0 <= mode && mode <= DSM_NONE);

	if (mCurrentMode != mode)
	{
		for (int i=0; i<DSM_NONE; ++i)
		{
			mInstance[i]->setEnabled(i == mode);
		}
		mCurrentMode = mode;
	}
}
