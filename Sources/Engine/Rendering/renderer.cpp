

#include "Engine/Rendering/renderer.hpp"

#include "DeferredLightCP.h"


namespace Ogre
{
    template<> SharedData* Singleton<SharedData>::msSingleton = 0;
}

using namespace Ogre;

const Ogre::uint8 Renderer::PRE_GBUFFER_RENDER_QUEUE = Ogre::RENDER_QUEUE_1;
const Ogre::uint8 Renderer::POST_GBUFFER_RENDER_QUEUE = Ogre::RENDER_QUEUE_8;

Renderer::Renderer(
		Viewport *vp, SceneManager *sm,  Camera *cam
	):
	mViewport(vp), mSceneMgr(sm), mCamera(cam)
{
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
	Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(4);
	
	sm->setShadowTextureCount(1);
	sm->setShadowFarDistance(200);
	sm->setShadowDirectionalLightExtrusionDistance(200);
	sm->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
	sm->setShadowTextureConfig(0, 512, 512, PF_FLOAT16_R, 2 );
	sm->setShadowTextureCasterMaterial("Render/ShadowCaster");
	
	new SharedData();
	SharedData::getSingleton().iSystem = this;

	for(int i=0; i<DSM_NONE; ++i)
		mInstance[i]=0;

	createResources();
		
	mCurrentMode = DSM_NONE;

	mGBufferInstance->setEnabled(true);
	setMode(mCurrentMode);
}


Renderer::~Renderer()
{
	CompositorChain *chain = CompositorManager::getSingleton().getCompositorChain(mViewport);
	for(int i=0; i<DSM_NONE; ++i)
		chain->_removeInstance(mInstance[i]);
	CompositorManager::getSingleton().removeCompositorChain(mViewport);

	Ogre::CompositorManager& compMgr = Ogre::CompositorManager::getSingleton();
	CompositorLogicMap::const_iterator itor = mCompositorLogics.begin();
	CompositorLogicMap::const_iterator end  = mCompositorLogics.end();
	while( itor != end )
	{
		compMgr.unregisterCompositorLogic( itor->first );
		delete itor->second;
		++itor;
	}
	mCompositorLogics.clear();
}

void Renderer::setMode(DSMode mode)
{
	assert( 0 <= mode && mode <= DSM_NONE);

	if (mCurrentMode == mode)
		return;

	for(int i=0; i<DSM_NONE; ++i)
	{
		if(i == mode)
		{
			mInstance[i]->setEnabled(true);
		}
		else
		{
			mInstance[i]->setEnabled(false);
		}
	}

	mCurrentMode = mode;
}


void Renderer::createResources(void)
{
	CompositorManager &compMan = CompositorManager::getSingleton();

	compMan.registerCustomCompositionPass("DeferredLight", new DeferredLightCompositionPass);
	mGBufferInstance = compMan.addCompositor(mViewport, "DeferredShading/GBuffer");

	mInstance[DSM_SHOWLIT] =		compMan.addCompositor(mViewport, "DeferredShading/ShowLit");
	mInstance[DSM_SHOWNORMALS] =	compMan.addCompositor(mViewport, "DeferredShading/ShowNormals");
	mInstance[DSM_SHOWDSP] =		compMan.addCompositor(mViewport, "DeferredShading/ShowDepthSpecular");
	mInstance[DSM_SHOWCOLOUR] =		compMan.addCompositor(mViewport, "DeferredShading/ShowColour");
	mInstance[DSM_SHOWGLOW] =		compMan.addCompositor(mViewport, "DeferredShading/ShowGlow");
	mInstance[DSM_SHOWSSAO] =		compMan.addCompositor(mViewport, "DeferredShading/ShowSSAO");
}
