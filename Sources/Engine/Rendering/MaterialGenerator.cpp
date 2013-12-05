/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2013 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
-----------------------------------------------------------------------------
*/


#include "Engine/Rendering/renderer.hpp"


#include "DLight.h"


#include "MaterialGenerator.h"

#include "OgreStringConverter.h"
#include "OgreException.h"

#include "OgrePass.h"
#include "OgreTechnique.h"

#include "OgreHighLevelGpuProgram.h"
#include "OgreHighLevelGpuProgramManager.h"

using namespace Ogre;

MaterialGenerator::MaterialGenerator():
	vsMask(0), fsMask(0), matMask(0), mImpl(0)
{
}
MaterialGenerator::~MaterialGenerator()
{
	delete mImpl;
}

const MaterialPtr &MaterialGenerator::getMaterial(Perm permutation)
{
	/// Check if material/shader permutation already was generated
	MaterialMap::iterator i = mMaterials.find(permutation);
	if(i != mMaterials.end())
	{
		return i->second;
	}
	else
	{
		/// Create it
		MaterialPtr templ = getTemplateMaterial(permutation & matMask);
		GpuProgramPtr vs = getVertexShader(permutation & vsMask);
		GpuProgramPtr fs = getFragmentShader(permutation & fsMask);
		
		/// Create material name
		String name = materialBaseName + StringConverter::toString(permutation);

		std::cerr << name << " " << vs->getName() << " " << fs->getName() << std::endl;
		/// Create material from template, and set shaders
		MaterialPtr mat = templ->clone(name);
		Technique *tech = mat->getTechnique(0);
		Pass *pass = tech->getPass(0);
		pass->setFragmentProgram(fs->getName());
		pass->setVertexProgram(vs->getName());
	
		/// And store it
		mMaterials[permutation] = mat;
		return mMaterials[permutation];
	}
}

const GpuProgramPtr &MaterialGenerator::getVertexShader(Perm permutation)
{
	ProgramMap::iterator i = mVs.find(permutation);
	if(i != mVs.end())
	{
		return i->second;
	}
	else
	{
		/// Create it
		mVs[permutation] = mImpl->generateVertexShader(permutation);
		return mVs[permutation];
	}
}

const GpuProgramPtr &MaterialGenerator::getFragmentShader(Perm permutation)
{
	ProgramMap::iterator i = mFs.find(permutation);
	if(i != mFs.end())
	{
		return i->second;
	}
	else
	{
		/// Create it
		mFs[permutation] = mImpl->generateFragmentShader(permutation);
		return mFs[permutation];
	}
}

const MaterialPtr &MaterialGenerator::getTemplateMaterial(Perm permutation)
{
	MaterialMap::iterator i = mTemplateMat.find(permutation);
	if(i != mTemplateMat.end())
	{
		return i->second;
	}
	else
	{
		/// Create it
		mTemplateMat[permutation] = mImpl->generateTemplateMaterial(permutation);
		return mTemplateMat[permutation];
	}
}

MaterialGenerator::Impl::~Impl()
{
}


//GLSL
class LightMaterialGeneratorGLSL : public MaterialGenerator::Impl
{
public:
	typedef MaterialGenerator::Perm Perm;
	LightMaterialGeneratorGLSL(const String &baseName):
    mBaseName(baseName)
	{

	}
	virtual ~LightMaterialGeneratorGLSL()
	{

	}

	virtual GpuProgramPtr generateVertexShader(Perm permutation)
	{
        String programName = "";

		if (permutation & LightMaterialGenerator::MI_DIRECTIONAL)
		{
			programName += "VS_Ambient";
		}
		else
		{
			programName += "VS_LightMaterial";
		}

		GpuProgramPtr ptr = HighLevelGpuProgramManager::getSingleton().getByName(programName);
		assert(!ptr.isNull());
		return ptr;
	}

	virtual GpuProgramPtr generateFragmentShader(Perm permutation)
	{
		/// Create shader
		if (mMasterSource.empty())
		{
			DataStreamPtr ptrMasterSource = ResourceGroupManager::getSingleton().openResource(
				"PS_LightMaterial.glsl",
				ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

			assert(ptrMasterSource.isNull()==false);
			mMasterSource = ptrMasterSource->getAsString();
		}

		assert(mMasterSource.empty()==false);

		String name = mBaseName+ "PS_" + StringConverter::toString(permutation)+"_ps";
		HighLevelGpuProgramPtr ptrProgram = HighLevelGpuProgramManager::getSingleton().createProgram(
			name,
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			"glsl", GPT_FRAGMENT_PROGRAM);

		ptrProgram->setSource(mMasterSource);
	    ptrProgram->setParameter("profiles", "glsl150");
		ptrProgram->setParameter("preprocessor_defines", getPPDefines(permutation));

		setUpBaseParameters(ptrProgram->getDefaultParameters());

		GpuProgramParametersSharedPtr params = ptrProgram->getDefaultParameters();
        int numSamplers = 0;
        params->setNamedConstant("Tex0", (int)numSamplers++);
        params->setNamedConstant("Tex1", (int)numSamplers++);

        if(permutation & LightMaterialGenerator::MI_SHADOW_CASTER)
            params->setNamedConstant("ShadowTex", (int)numSamplers++);

		return GpuProgramPtr(ptrProgram);
	}

	virtual MaterialPtr generateTemplateMaterial(Perm permutation)
	{
		String materialName = mBaseName;

        if(permutation & LightMaterialGenerator::MI_DIRECTIONAL)
		{
			materialName += "Quad";
		}
		else
		{
			materialName += "Geometry";
		}

		if(permutation & LightMaterialGenerator::MI_SHADOW_CASTER)
		{
			materialName += "Shadow";
		}
		return MaterialManager::getSingleton().getByName(materialName);
	}

protected:
    String mBaseName;
    String mMasterSource;

    String getPPDefines(Perm permutation)
    {
        String strPPD;

        //Get the type of light
        Ogre::uint lightType;
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
            assert(false && "Permutation must have a light type");
        }
        strPPD += "LIGHT_TYPE=" + StringConverter::toString(lightType);

        //Optional parameters
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

    void setUpBaseParameters(const GpuProgramParametersSharedPtr& params)
    {
        assert(params.isNull()==false);

        struct AutoParamPair { String name; GpuProgramParameters::AutoConstantType type; };

        //A list of auto params that might be present in the shaders generated
        static const AutoParamPair AUTO_PARAMS[] = {
            { "vpWidth",            GpuProgramParameters::ACT_VIEWPORT_WIDTH },
            { "vpHeight",           GpuProgramParameters::ACT_VIEWPORT_HEIGHT },
            { "worldView",          GpuProgramParameters::ACT_WORLDVIEW_MATRIX },
            { "invProj",            GpuProgramParameters::ACT_INVERSE_PROJECTION_MATRIX },
            { "invView",            GpuProgramParameters::ACT_INVERSE_VIEW_MATRIX },
            { "flip",               GpuProgramParameters::ACT_RENDER_TARGET_FLIPPING },
            { "lightDiffuseColor",  GpuProgramParameters::ACT_LIGHT_DIFFUSE_COLOUR },
            { "lightSpecularColor", GpuProgramParameters::ACT_LIGHT_SPECULAR_COLOUR },
            { "lightFalloff",       GpuProgramParameters::ACT_LIGHT_ATTENUATION },
            { "lightPos",           GpuProgramParameters::ACT_LIGHT_POSITION_VIEW_SPACE },
            { "lightDir",           GpuProgramParameters::ACT_LIGHT_DIRECTION_VIEW_SPACE },
            { "spotParams",         GpuProgramParameters::ACT_SPOTLIGHT_PARAMS },
            { "farClipDistance",    GpuProgramParameters::ACT_FAR_CLIP_DISTANCE },
            { "shadowViewProjMat",  GpuProgramParameters::ACT_TEXTURE_VIEWPROJ_MATRIX }
        };
        int numParams = sizeof(AUTO_PARAMS) / sizeof(AutoParamPair);
        
        for (int i=0; i<numParams; i++)
        {
            if (params->_findNamedConstantDefinition(AUTO_PARAMS[i].name))
            {
                params->setNamedAutoConstant(AUTO_PARAMS[i].name, AUTO_PARAMS[i].type);
            }
        }
    }
};

LightMaterialGenerator::LightMaterialGenerator()
{
	vsMask = 0x00000004;
	fsMask = 0x0000003F;
	matMask =	LightMaterialGenerator::MI_DIRECTIONAL | 
				LightMaterialGenerator::MI_SHADOW_CASTER;
	
	materialBaseName = "Light/";
	mImpl = new LightMaterialGeneratorGLSL("Light/");
}

LightMaterialGenerator::~LightMaterialGenerator()
{
}
