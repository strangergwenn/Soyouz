/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/


#ifndef __WORLD_H_
#define __WORLD_H_

#include "Engine/engine.h"
#include "Engine/actor.h"
#include "Engine/iomanager.h"


/*----------------------------------------------
	Shader generator definition
----------------------------------------------*/

#ifdef USE_RTSHADER_SYSTEM

class ShaderGeneratorTechniqueResolverListener : public MaterialManager::Listener
{
public:

	ShaderGeneratorTechniqueResolverListener(RTShader::ShaderGenerator* pShaderGenerator)
	{
		mShaderGenerator = pShaderGenerator;
	}

	virtual Technique* handleSchemeNotFound(unsigned short schemeIndex, 
		const String& schemeName, Material* originalMaterial, unsigned short lodIndex, 
		const Renderable* rend)
	{
		if (schemeName == RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME)
		{
			MaterialRegisterIterator itFind = mRegisteredMaterials.find(originalMaterial);
			bool techniqueCreated = false;

			if (itFind == mRegisteredMaterials.end())
			{
				techniqueCreated = mShaderGenerator->createShaderBasedTechnique(
					originalMaterial->getName(), 
					MaterialManager::DEFAULT_SCHEME_NAME, 
					schemeName);				
			}
			mRegisteredMaterials[originalMaterial] = techniqueCreated;
		}
		return NULL;
	}

protected:
	typedef std::map<Material*, bool>	MaterialRegisterMap;
	typedef MaterialRegisterMap::iterator	MaterialRegisterIterator;

	MaterialRegisterMap			mRegisteredMaterials;
	RTShader::ShaderGenerator*		mShaderGenerator;
};

#endif


/*----------------------------------------------
	World class definition
----------------------------------------------*/

class World
{

public:
	
	/**
	 * @brief Load the world
	 **/
	World();

	/**
	 * @brief Unload the world
	 **/
	~World();

	/**
	 * @brief Run the level (blocking)
	 **/
	void run();


protected:

	/**
	 * @brief Level construction
	 **/
	virtual void construct() = NULL;

	/**
	 * @brief Level destruction
	 **/
	virtual void destruct() = NULL;
	
	/**
	 * @brief Setup the level
	 **/
	bool setup();

	/**
	 * @brief Setup resources
	 **/
	void setupResources();

	/**
	 * @brief Setup rendering methods
	 **/
	void setupRender();

#ifdef USE_RTSHADER_SYSTEM

	/**
	 * @brief Start the shader generator
	 **/
	bool setupShaderGenerator(SceneManager* sceneMgr);
	
	/**
	 * @brief End the shader generator
	 **/
	void stopShaderGenerator();

#endif

	Root *mRoot;
	Camera* mCamera;
	SceneManager* mScene;
	RenderWindow* mWindow;
	OverlaySystem* mOverlaySystem;
	IOManager* mIOManager;

#ifdef OGRE_STATIC_LIB
	StaticPluginLoader mStaticPluginLoader;
#endif

#ifdef USE_RTSHADER_SYSTEM
	RTShader::ShaderGenerator*			mShaderGenerator;
	ShaderGeneratorTechniqueResolverListener*	mMaterialMgrListener;
#endif

};

#endif /* __WORLD_H_ */

