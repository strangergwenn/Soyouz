/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef H_WJ_MaterialGenerator
#define H_WJ_MaterialGenerator


#include "Engine/Rendering/renderer.hpp"


/*----------------------------------------------
	Light material generator
----------------------------------------------*/

class LightMaterialGenerator
{

public:
	
	/**
	 * @brief Material types
	 **/
	enum MaterialID
	{
		MI_POINT			= 0x01, // Is a point light
		MI_SPOTLIGHT		= 0x02, // Is a spot light
        MI_DIRECTIONAL		= 0x04, // Is a directional light
		MI_ATTENUATED		= 0x08, // Rendered attenuated
		MI_SPECULAR			= 0x10, // Specular component is calculated
		MI_SHADOW_CASTER	= 0x20	// Will cast shadows
	};
	typedef Ogre::uint32 Perm;
	
	/**
	 * @brief Generator constructor
	 **/
	LightMaterialGenerator();
	
	/**
	 * @brief Generator destructor
	 **/
	~LightMaterialGenerator();
	
	/**
	 * @brief Get a material reference with caching
	 * @param permutation		Material mask
	 * @return a pointer to the material
	 **/
	const Ogre::MaterialPtr& getMaterial(Perm permutation);


protected:

	
	/**
	 * @brief Get a fragment shader
	 * @param permutation		Material mask
	 * @return a pointer to the shader
	 **/
	Ogre::GpuProgramPtr generateFragmentShader(Perm permutation);
	
	/**
	 * @brief Get the correct defines for the fragment shader
	 * @param permutation		Material mask
	 * @return the define string
	 **/
	Ogre::String getPPDefines(Perm permutation);
	
	/**
	 * @brief Get a material reference with caching
	 * @param params			Output params
	 **/
	void setUpBaseParameters(const Ogre::GpuProgramParametersSharedPtr& params);


	// Source data
    Ogre::String mBaseName;
    Ogre::String mMasterSource;

	// Masks
	Perm vsMask;
	Perm fsMask;
	Perm matMask;

	// Current storage
	typedef Ogre::map<Perm, Ogre::GpuProgramPtr>::type ProgramMap;
	typedef Ogre::map<Perm, Ogre::MaterialPtr>::type MaterialMap;
	ProgramMap mVs, mFs;
	MaterialMap mTemplateMat, mMaterials;
};


#endif
