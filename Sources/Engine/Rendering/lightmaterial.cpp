/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/


#include "Engine/Rendering/lightmaterial.hpp"
#include "Engine/Rendering/renderer.hpp"


/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

LightMaterialGenerator::LightMaterialGenerator()
{
	vsMask = 0x00000004;
	fsMask = 0x0000003F;
	matMask =	LightMaterialGenerator::MI_DIRECTIONAL | 
				LightMaterialGenerator::MI_SHADOW_CASTER;
	
	mBaseName = "Light/";
}


LightMaterialGenerator::~LightMaterialGenerator()
{
}


/*----------------------------------------------
	Methods
----------------------------------------------*/

const Ogre::MaterialPtr& LightMaterialGenerator::getMaterial(Perm permutation)
{
	MaterialMap::iterator i = mMaterials.find(permutation);
	if (i != mMaterials.end())
	{
		return i->second;
	}
	else
	{
		// Create it
		Ogre::String materialName = mBaseName;
		materialName += (permutation & matMask & LightMaterialGenerator::MI_DIRECTIONAL) ? "Quad" : "Geometry";
		if (permutation & matMask & LightMaterialGenerator::MI_SHADOW_CASTER)
		{
			materialName += "Shadow";
		}
		Ogre::MaterialPtr templ = Ogre::MaterialManager::getSingleton().getByName(materialName);

		// Vertex shader
		Ogre::GpuProgramPtr vs;
		ProgramMap::iterator i = mVs.find(permutation & vsMask);
		if (i != mVs.end())
		{
			vs = i->second;
		}
		else
		{
			Ogre::String programName = (permutation & vsMask & LightMaterialGenerator::MI_DIRECTIONAL) ? "VS_Ambient" : "VS_LightMaterial";
			mVs[permutation] = Ogre::HighLevelGpuProgramManager::getSingleton().getByName(programName);
			vs = mVs[permutation];
		}
		assert (!vs.isNull());

		// Fragment shader
		Ogre::GpuProgramPtr fs;
		i = mFs.find(permutation & fsMask);
		if (i != mFs.end())
		{
			fs = i->second;
		}
		else
		{
			mFs[permutation] = generateFragmentShader(permutation & fsMask);
			fs = mFs[permutation];
		}
		assert (!fs.isNull());
		
		// Create material name
		Ogre::String name = mBaseName + Ogre::StringConverter::toString(permutation);
		std::cerr << name << " " << vs->getName() << " " << fs->getName() << std::endl;

		// Create material from template, and set shaders
		Ogre::MaterialPtr mat = templ->clone(name);
		Ogre::Technique *tech = mat->getTechnique(0);
		Ogre::Pass *pass = tech->getPass(0);
		pass->setFragmentProgram(fs->getName());
		pass->setVertexProgram(vs->getName());
	
		// And store it
		mMaterials[permutation] = mat;
		return mMaterials[permutation];
	}
}


Ogre::GpuProgramPtr LightMaterialGenerator::generateFragmentShader(Perm permutation)
{
    int numSamplers = 0;

	/// Load shader master source
	if (mMasterSource.empty())
	{
		Ogre::DataStreamPtr ptrMasterSource = Ogre::ResourceGroupManager::getSingleton().openResource(
			"PS_LightMaterial.glsl",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		assert (ptrMasterSource.isNull()==false);
		mMasterSource = ptrMasterSource->getAsString();
	}
	assert (mMasterSource.empty()==false);

	// Create new shader
	Ogre::String name = mBaseName+ "PS_" + Ogre::StringConverter::toString(permutation)+"_ps";
	Ogre::HighLevelGpuProgramPtr ptrProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
		name,
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		"glsl", Ogre::GPT_FRAGMENT_PROGRAM);

	// Set shader parameters
	ptrProgram->setSource(mMasterSource);
	ptrProgram->setParameter("profiles", "glsl150");
	ptrProgram->setParameter("preprocessor_defines", getPPDefines(permutation));
	setUpBaseParameters(ptrProgram->getDefaultParameters());
	Ogre::GpuProgramParametersSharedPtr params = ptrProgram->getDefaultParameters();
    params->setNamedConstant("Tex0", (int)numSamplers++);
    params->setNamedConstant("Tex1", (int)numSamplers++);

	// Add shadow
    if (permutation & LightMaterialGenerator::MI_SHADOW_CASTER)
	{
        params->setNamedConstant("ShadowTex", (int)numSamplers++);
	}

	return Ogre::GpuProgramPtr(ptrProgram);
}



Ogre::String LightMaterialGenerator::getPPDefines(Perm permutation)
{
    Ogre::String strPPD;
    Ogre::uint lightType;

    // Set the type of light
    if (permutation & LightMaterialGenerator::MI_POINT)
    {
        lightType = 1;
    }
    else if (permutation & LightMaterialGenerator::MI_SPOTLIGHT)
    {
        lightType = 2;
    }
    else if (permutation & LightMaterialGenerator::MI_DIRECTIONAL)
    {
        lightType = 3;
    }
    else
    {
        assert (false && "Permutation must have a light type");
    }
    strPPD += "LIGHT_TYPE=" + Ogre::StringConverter::toString(lightType);

    // Optional parameters
    if (permutation & LightMaterialGenerator::MI_SPECULAR)
    {
        strPPD += ",IS_SPECULAR=1";
    }
    if (permutation & LightMaterialGenerator::MI_ATTENUATED)
    {
        strPPD += ",IS_ATTENUATED=1";
    }
    if (permutation & LightMaterialGenerator::MI_SHADOW_CASTER)
    {
        strPPD += ",IS_SHADOW_CASTER=1";
    }
    return strPPD;
}


void LightMaterialGenerator::setUpBaseParameters(const Ogre::GpuProgramParametersSharedPtr& params)
{
	assert (params.isNull()==false);
	struct AutoParamPair {Ogre::String name; Ogre::GpuProgramParameters::AutoConstantType type;};

	// A list of auto params that might be present in the shaders generated
	static const AutoParamPair AUTO_PARAMS[] = {
		{ "vpWidth",            Ogre::GpuProgramParameters::ACT_VIEWPORT_WIDTH },
		{ "vpHeight",           Ogre::GpuProgramParameters::ACT_VIEWPORT_HEIGHT },
		{ "worldView",          Ogre::GpuProgramParameters::ACT_WORLDVIEW_MATRIX },
		{ "invProj",            Ogre::GpuProgramParameters::ACT_INVERSE_PROJECTION_MATRIX },
		{ "invView",            Ogre::GpuProgramParameters::ACT_INVERSE_VIEW_MATRIX },
		{ "flip",               Ogre::GpuProgramParameters::ACT_RENDER_TARGET_FLIPPING },
		{ "lightDiffuseColor",  Ogre::GpuProgramParameters::ACT_LIGHT_DIFFUSE_COLOUR },
		{ "lightSpecularColor", Ogre::GpuProgramParameters::ACT_LIGHT_SPECULAR_COLOUR },
		{ "lightFalloff",       Ogre::GpuProgramParameters::ACT_LIGHT_ATTENUATION },
		{ "lightPos",           Ogre::GpuProgramParameters::ACT_LIGHT_POSITION_VIEW_SPACE },
		{ "lightDir",           Ogre::GpuProgramParameters::ACT_LIGHT_DIRECTION_VIEW_SPACE },
		{ "spotParams",         Ogre::GpuProgramParameters::ACT_SPOTLIGHT_PARAMS },
		{ "farClipDistance",    Ogre::GpuProgramParameters::ACT_FAR_CLIP_DISTANCE },
		{ "shadowViewProjMat",  Ogre::GpuProgramParameters::ACT_TEXTURE_VIEWPROJ_MATRIX }
	};
	int numParams = sizeof(AUTO_PARAMS) / sizeof(AutoParamPair);
    
	// Bind parameters
	for (int i=0; i<numParams; i++)
	{
		if (params->_findNamedConstantDefinition(AUTO_PARAMS[i].name))
		{
			params->setNamedAutoConstant(AUTO_PARAMS[i].name, AUTO_PARAMS[i].type);
		}
	}
}
