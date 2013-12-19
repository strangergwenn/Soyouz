/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/Rendering/renderer.hpp"
#include "Engine/Rendering/renderoperation.hpp"
#include "Engine/game.hpp"


/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

Renderer::Renderer(Ogre::Viewport* vp, Ogre::SceneManager* sm, tinyxml2::XMLElement* s)
	: mViewport(vp), mScene(sm)
{
	// XML settings
	tinyxml2::XMLElement* config = s->FirstChildElement("renderer");
	assert(config != NULL);
	int res = config->FirstChildElement("shadowRes")->IntAttribute("value");
	int aniso = config->FirstChildElement("anisotropy")->IntAttribute("value");
	size_t mipmaps = (size_t)(config->FirstChildElement("mipmaps")->IntAttribute("value"));
	float distance = config->FirstChildElement("shadowDistance")->FloatAttribute("value");

	// Texture filtering
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(mipmaps);
	Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
	Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);

	// General render settings
	vp->setBackgroundColour(Ogre::ColourValue(0.1f, 0.1f, 0.1f));
	sm->setAmbientLight(Ogre::ColourValue(0,0,0));
	sm->setShadowTextureCount(1);
	sm->setShadowFarDistance(distance);
	sm->setShadowDirectionalLightExtrusionDistance(distance);
	sm->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
	sm->setShadowTextureConfig(0, res, res, Ogre::PF_FLOAT16_R, 2 );
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
	mInstance[DSM_SHOWGBUFFER] =	compMan.addCompositor(mViewport, "DeferredShading/ShowColour");
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
