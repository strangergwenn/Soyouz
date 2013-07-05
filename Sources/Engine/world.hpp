/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/


#ifndef __WORLD_H_
#define __WORLD_H_

#include "Engine/engine.hpp"
#include "Engine/iomanager.hpp"

class Actor;
class Player;


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
	virtual ~World();

	/**
	 * @brief Run the level (blocking)
	 **/
	virtual void run();

	/**
	 * @brief Run the level (blocking)
	 * @param name					Node name
	 * @return the scene node
	 **/
	SceneNode* createWorldNode(String name);
	
	/**
	 * @brief Run the level (blocking)
	 * @param name					Node name
	 * @param name					File name
	 * @return the scene entity
	 **/
	Entity* World::createWorldEntity(String name, String file);

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
	virtual bool setup();

	/**
	 * @brief Setup resources
	 **/
	virtual void setupResources();

	/**
	 * @brief Setup rendering methods
	 **/
	virtual void setupRender();
	
	/**
	 * @brief Setup the player
	 **/
	virtual void setupPlayer();

#ifdef USE_RTSHADER_SYSTEM

	/**
	 * @brief Start the shader generator
	 **/
	bool setupShaderGenerator();
	
	/**
	 * @brief End the shader generator
	 **/
	void killShaderGenerator();

#endif

	Root *mRoot;
	Camera* mCamera;
	SceneManager* mScene;
	RenderWindow* mWindow;
	OverlaySystem* mOverlaySystem;

	Player* mPlayer;
	IOManager* mIOManager;

#ifdef OGRE_STATIC_LIB
	StaticPluginLoader mStaticPluginLoader;
#endif

#ifdef USE_RTSHADER_SYSTEM
	RTShader::ShaderGenerator* mShaderGenerator;
	ShaderGeneratorTechniqueResolverListener* mMaterialMgrListener;
#endif

};

#endif /* __WORLD_H_ */

