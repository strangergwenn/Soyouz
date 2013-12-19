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

Renderer::Renderer(Ogre::Viewport* vp, Ogre::SceneManager* sm, Ogre::Camera* cam, tinyxml2::XMLElement* s)
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
	sm->setAmbientLight(Ogre::ColourValue(0,0,0));
	sm->setShadowFarDistance(distance);
	sm->setShadowDirectionalLightExtrusionDistance(distance);
	sm->setShadowTextureCasterMaterial("Render/ShadowCaster");
	
	// GBuffer creation
	Ogre::RenderTarget* target = cam->getLastViewport()->getTarget();
	Ogre::CompositorManager2* compMan = Ogre::Root::getSingleton().getCompositorManager2();
	//compMan->registerCustomCompositionPass("DeferredLight", new DeferredRenderPass);
	Ogre::CompositorWorkspace* gbuffer = compMan->addWorkspace(sm, target, cam, "DeferredShading/GBuffer", true);
	gbuffer->setEnabled(true);
	
	// Prepare instances
	for (int i = 0; i < DSM_NONE; ++i)
	{
		mInstance[i] = 0;
	}

	// Load instances
	mInstance[DSM_SHOWLIT] =		compMan->addWorkspace(sm, target, cam, "DeferredShading/ShowGBuffer", true);
	mInstance[DSM_SHOWGBUFFER] =	compMan->addWorkspace(sm, target, cam, "DeferredShading/ShowGBuffer", true);
	mInstance[DSM_SHOWSSAO] =		compMan->addWorkspace(sm, target, cam, "DeferredShading/ShowSSAO", true);

	// Ready up
	mCurrentMode = DSM_NONE;
	setMode(mCurrentMode);
}


Renderer::~Renderer()
{
	//TODO
	/*
	Ogre::CompositorChain *chain = Ogre::CompositorManager::getSingleton().getCompositorChain(mViewport);
	for(int i = 0; i < DSM_NONE; ++i)
	{
		chain->_removeInstance(mInstance[i]);
	}
	Ogre::CompositorManager::getSingleton().removeCompositorChain(mViewport);
	*/
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
