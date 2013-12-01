
/**
Implementation of a Deferred Shading engine in OGRE, using Multiple Render Targets and
CG high level language shaders.
	// W.J. :wumpus: van der Laan 2005 / Noam Gat 2009 //

Deferred shading renders the scene to a 'fat' texture format, using a shader that outputs colour, 
normal, depth, and possible other attributes per fragment. Multi Render Target is required as we 
are dealing with many outputs which get written into multiple render textures in the same pass.

After rendering the scene in this format, the shading (lighting) can be done as a post process. 
This means that lighting is done in screen space, using light-representing geometry (sphere for
point light, cone for spot light and quad for directional) to render their contribution.

The wiki article explaining this demo can be found here :
  http://www.ogre3d.org/wiki/index.php/Deferred_Shading
*/

#ifndef H_DeferredShadingSystem
#define H_DeferredShadingSystem

#include "Engine/ogre.hpp"
#include "Engine/DeferredShading/GeomUtils.h"


class GBufferListener : public Ogre::MaterialManager::Listener
{
	public:

	Ogre::Technique* GBufferListener::handleSchemeNotFound(unsigned short schemeIndex, 
		const Ogre::String& schemeName, Ogre::Material* originalMaterial, unsigned short lodIndex, 
		const Ogre::Renderable* rend)
	{
		// Material manager settings
		Ogre::MaterialManager& matMgr = Ogre::MaterialManager::getSingleton();
		Ogre::String curSchemeName = matMgr.getActiveScheme();
		matMgr.setActiveScheme(Ogre::MaterialManager::DEFAULT_SCHEME_NAME);
		Ogre::Technique* originalTechnique = originalMaterial->getBestTechnique(lodIndex, rend);
		matMgr.setActiveScheme(curSchemeName);
	
		// Create GBuffer technique
		Ogre::Technique* gBufferTech = originalMaterial->createTechnique();
		gBufferTech->removeAllPasses();
		gBufferTech->setSchemeName(schemeName);

		// Create post-GBuffer technique
		Ogre::Technique* noGBufferTech = originalMaterial->createTechnique();
		noGBufferTech->removeAllPasses();
		noGBufferTech->setSchemeName("NoGBuffer");

		// Fill new pass with old data
		if (originalTechnique->getNumPasses() > 0)
		{
			Ogre::Pass* newPass = gBufferTech->createPass();
			Ogre::Pass* originalPass = originalTechnique->getPass(0);
		
			*newPass = *(originalMaterial->getTechnique(0)->getPass(0));
			newPass->setAmbient(originalPass->getAmbient());
			newPass->setDiffuse(originalPass->getDiffuse());
			newPass->setSpecular(originalPass->getSpecular());
			newPass->setShininess(originalPass->getShininess());
			newPass->setCullingMode(originalPass->getCullingMode());
			newPass->setLightingEnabled(false);
		}
		return gBufferTech;
	}
	
};


/** System to manage Deferred Shading for a camera/render target.
 *  @note With the changes to the compositor framework, this class just
 *		selects which compositors to enable.
 */
class DeferredShadingSystem : public Ogre::RenderTargetListener
{
public:
	DeferredShadingSystem(Ogre::Viewport *vp, Ogre::SceneManager *sm, Ogre::Camera *cam);
	~DeferredShadingSystem();

	enum DSMode
	{
		DSM_SHOWLIT = 0,     // The deferred shading mode
		DSM_SHOWCOLOUR = 1,  // Show diffuse (for debugging)
		DSM_SHOWNORMALS = 2, // Show normals (for debugging)
		DSM_SHOWDSP = 3,	 // Show depth and specular channel (for debugging)
		DSM_SHOWGLOW = 4,	 // Show glow maps
		DSM_COUNT = 5
	};

	//The first render queue that does get rendered into the GBuffer
	//place objects (like skies) that should be before gbuffer before this one.
	static const Ogre::uint8 PRE_GBUFFER_RENDER_QUEUE;
	
	//The first render queue that does not get rendered into the GBuffer
	//place transparent (or other non gbuffer) objects after this one
	static const Ogre::uint8 POST_GBUFFER_RENDER_QUEUE;

	void initialize();

	/** Set rendering mode (one of DSMode)
	 */
	void setMode(DSMode mode);

	DSMode getMode(void) const;
	
	/** Activate or deactivate system
	 */
	void setActive(bool active);
	
protected:
	Ogre::Viewport *mViewport;
	Ogre::SceneManager *mSceneMgr;
	Ogre::Camera *mCamera;
	
	Ogre::CompositorInstance *mGBufferInstance;
	// Filters
	Ogre::CompositorInstance *mInstance[DSM_COUNT];
	// Active/inactive
	bool mActive;
	DSMode mCurrentMode;

	//Used to unregister compositor logics and free memory
	typedef Ogre::map<Ogre::String, Ogre::CompositorLogic*>::type CompositorLogicMap;
	CompositorLogicMap mCompositorLogics;

	void createResources();
};



class SharedData : public Ogre::Singleton<SharedData>
{
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
