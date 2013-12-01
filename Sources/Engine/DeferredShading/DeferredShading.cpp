

#include "DeferredShading.h"

#include "OgreConfigFile.h"
#include "OgreStringConverter.h"
#include "OgreException.h"

#include "OgreRoot.h"
#include "OgreRenderSystem.h"

#include "OgreEntity.h"
#include "OgreSubEntity.h"
#include "OgreRoot.h"

#include "OgreCompositor.h"
#include "OgreCompositorManager.h"
#include "OgreCompositorChain.h"
#include "OgreCompositorInstance.h"

#include "OgreLogManager.h"

#include "DeferredLightCP.h"


namespace Ogre
{
    template<> SharedData* Singleton<SharedData>::msSingleton = 0;
}

using namespace Ogre;

const Ogre::uint8 DeferredShadingSystem::PRE_GBUFFER_RENDER_QUEUE = Ogre::RENDER_QUEUE_1;
const Ogre::uint8 DeferredShadingSystem::POST_GBUFFER_RENDER_QUEUE = Ogre::RENDER_QUEUE_8;

DeferredShadingSystem::DeferredShadingSystem(
		Viewport *vp, SceneManager *sm,  Camera *cam
	):
	mViewport(vp), mSceneMgr(sm), mCamera(cam)
{
	sm->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
	sm->setShadowTextureCasterMaterial("DeferredShading/Shadows/Caster");
	mSceneMgr->setShadowTextureCount(1);
	mSceneMgr->setShadowFarDistance(500);
	mSceneMgr->setShadowTextureConfig(0, 512, 512, PF_FLOAT16_R, 2 );
	mSceneMgr->setShadowDirectionalLightExtrusionDistance(100);
}

void DeferredShadingSystem::initialize()
{
	for(int i=0; i<DSM_COUNT; ++i)
		mInstance[i]=0;

	createResources();
	
	mActive = false;
	
	mCurrentMode = DSM_SHOWLIT;
	setActive(true);
}

DeferredShadingSystem::~DeferredShadingSystem()
{
	CompositorChain *chain = CompositorManager::getSingleton().getCompositorChain(mViewport);
	for(int i=0; i<DSM_COUNT; ++i)
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

void DeferredShadingSystem::setMode(DSMode mode)
{
	assert( 0 <= mode && mode < DSM_COUNT);

	if (mCurrentMode == mode && mInstance[mode]->getEnabled()==mActive)
		return;

	for(int i=0; i<DSM_COUNT; ++i)
	{
		if(i == mode)
		{
			mInstance[i]->setEnabled(mActive);
		}
		else
		{
			mInstance[i]->setEnabled(false);
		}
	}

	mCurrentMode = mode;
}

void DeferredShadingSystem::setActive(bool active)
{
	if (mActive != active)
	{
		mActive = active;
		mGBufferInstance->setEnabled(active);
		setMode(mCurrentMode);
	}
}

DeferredShadingSystem::DSMode DeferredShadingSystem::getMode(void) const
{
	return mCurrentMode;
}

void DeferredShadingSystem::createResources(void)
{
	CompositorManager &compMan = CompositorManager::getSingleton();

	static bool firstTime = true;
	if (firstTime)
	{
		MaterialManager::getSingleton().addListener(new GBufferListener, "GBuffer");
		compMan.registerCustomCompositionPass("DeferredLight", new DeferredLightCompositionPass);
		firstTime = false;
	}
	
	mGBufferInstance = compMan.addCompositor(mViewport, "DeferredShading/GBuffer");

	mInstance[DSM_SHOWLIT] = compMan.addCompositor(mViewport, "DeferredShading/ShowLit");
	mInstance[DSM_SHOWNORMALS] = compMan.addCompositor(mViewport, "DeferredShading/ShowNormals");
	mInstance[DSM_SHOWDSP] = compMan.addCompositor(mViewport, "DeferredShading/ShowDepthSpecular");
	mInstance[DSM_SHOWCOLOUR] = compMan.addCompositor(mViewport, "DeferredShading/ShowColour");
	mInstance[DSM_SHOWGLOW] = compMan.addCompositor(mViewport, "DeferredShading/ShowGlow");
}
