
#include "GBufferMaterialGenerator.h"

#include "OgreMaterialManager.h"
#include "OgreGpuProgramManager.h"
#include "OgreRoot.h"
#include "OgreStringConverter.h"
#include "OgreHighLevelGpuProgramManager.h"
#include "OgreTechnique.h"

using namespace Ogre;

//This is the concrete implementation of the material generator.
class GBufferMaterialGeneratorImpl : public MaterialGenerator::Impl
{
public:
	GBufferMaterialGeneratorImpl(const String& baseName) : 
      mBaseName(baseName)
      {
      }
	
protected:
	String mBaseName;
	virtual GpuProgramPtr generateVertexShader(MaterialGenerator::Perm permutation);
	virtual GpuProgramPtr generateFragmentShader(MaterialGenerator::Perm permutation);
	virtual MaterialPtr generateTemplateMaterial(MaterialGenerator::Perm permutation);

};

GBufferMaterialGenerator::GBufferMaterialGenerator() {
    vsMask = VS_MASK;
	fsMask = FS_MASK;
	matMask = MAT_MASK;
	materialBaseName = "DeferredShading/GBuffer/";
	mImpl = new GBufferMaterialGeneratorImpl(materialBaseName);
}

GpuProgramPtr GBufferMaterialGeneratorImpl::generateVertexShader(MaterialGenerator::Perm permutation)
{
	StringStream ss;

    int shadingLangVersion = Root::getSingleton().getRenderSystem()->getNativeShadingLanguageVersion();
    const char *inSemantic = shadingLangVersion >= 150 ? "in" : "attribute";
    const char *outSemantic = shadingLangVersion >= 150 ? "out" : "varying";

	ss << "#version " << shadingLangVersion << std::endl;

    ss << inSemantic << " vec4 vertex;" << std::endl;
    ss << inSemantic << " vec3 normal;" << std::endl;

    uint32 numTexCoords = (permutation & GBufferMaterialGenerator::GBP_TEXCOORD_MASK) >> 8;
    for (uint32 i = 0; i < numTexCoords; i++)
    {
        ss << inSemantic << " vec2 uv" << i << ';' << std::endl;
    }

    if (permutation & GBufferMaterialGenerator::GBP_NORMAL_MAP)
    {
        ss << inSemantic << " vec3 tangent;" << std::endl;
    }

    ss << std::endl;

    ss << outSemantic << " vec3 oViewPos;" << std::endl;
    ss << outSemantic << " vec3 oNormal;" << std::endl;
    if (permutation & GBufferMaterialGenerator::GBP_NORMAL_MAP)
    {
        ss << outSemantic << " vec3 oTangent;" << std::endl;
        ss << outSemantic << " vec3 oBiNormal;" << std::endl;
    }
    for (uint32 i = 0; i < numTexCoords; i++)
    {
        ss << outSemantic << " vec2 oUv" << i << ";" << std::endl;
    }

    ss << std::endl;

    ss << "uniform mat4 cWorldViewProj;" << std::endl;
    ss << "uniform mat4 cWorldView;" << std::endl;

    ss << "void main()" << std::endl;

    ss << "{" << std::endl;
    ss << "	gl_Position = cWorldViewProj * vertex;" << std::endl;
    ss << "	oNormal = (cWorldView * vec4(normal,0)).xyz;" << std::endl;
    if (permutation & GBufferMaterialGenerator::GBP_NORMAL_MAP)
    {
        ss << "	oTangent = (cWorldView * vec4(tangent,0)).xyz;" << std::endl;
        ss << "	oBiNormal = cross(oNormal, oTangent);" << std::endl;
    }

    ss << "	oViewPos = (cWorldView * vertex).xyz;" << std::endl;

    for (uint32 i=0; i<numTexCoords; i++) {
        ss << "	oUv" << i << " = uv" << i << ';' << std::endl;
    }

    ss << "}" << std::endl;

    String programSource = ss.str();
    String programName = mBaseName + "VP_" + StringConverter::toString(permutation);

	Ogre::Log* msg = LogManager::getSingleton().getDefaultLog();
	//msg->logMessage(programSource);

    // Create shader object
    HighLevelGpuProgramPtr ptrProgram;
    ptrProgram = HighLevelGpuProgramManager::getSingleton().createProgram(
		programName,
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		"glsl", GPT_VERTEX_PROGRAM);
    
    ptrProgram->setSource(programSource);

    const GpuProgramParametersSharedPtr& params = ptrProgram->getDefaultParameters();
    params->setNamedAutoConstant("cWorldViewProj", GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
    params->setNamedAutoConstant("cWorldView", GpuProgramParameters::ACT_WORLDVIEW_MATRIX);
    ptrProgram->load();
        
    return GpuProgramPtr(ptrProgram);
}

GpuProgramPtr GBufferMaterialGeneratorImpl::generateFragmentShader(MaterialGenerator::Perm permutation)
{
	StringStream ss;

    int shadingLangVersion = Root::getSingleton().getRenderSystem()->getNativeShadingLanguageVersion();
    const char *inSemantic = shadingLangVersion >= 150 ? "in" : "varying";
    const char *outData = shadingLangVersion >= 150 ? "fragData" : "gl_FragData";
    const char *textureFunc = shadingLangVersion >= 150 ? "texture" : "texture2D";

    ss << "#version " << shadingLangVersion << std::endl;
    if(shadingLangVersion >= 150)
    {
        ss << "out vec4 fragData[3];" << std::endl;
    }
    
    ss << inSemantic << " vec3 oViewPos;" << std::endl;
    ss << inSemantic << " vec3 oNormal;" << std::endl;

    if (permutation & GBufferMaterialGenerator::GBP_NORMAL_MAP)
    {
        ss << inSemantic << " vec3 oTangent;" << std::endl;
        ss << inSemantic << " vec3 oBiNormal;" << std::endl;
    }

    uint32 numTexCoords = (permutation & GBufferMaterialGenerator::GBP_TEXCOORD_MASK) >> 8;
    for (uint32 i = 0; i < numTexCoords; i++)
    {
        ss << inSemantic << " vec2 oUv" << i << ';' << std::endl;
    }

    ss << std::endl;

    if (permutation & GBufferMaterialGenerator::GBP_NORMAL_MAP)
    {
        ss << "uniform sampler2D sNormalMap;" << std::endl;
    }

    uint32 numTextures = permutation & GBufferMaterialGenerator::GBP_TEXTURE_MASK;

    ss << "uniform sampler2D sTex0;" << std::endl;
	ss << "uniform sampler2D sGlowMap;" << std::endl;

    if (numTextures == 0 || permutation & GBufferMaterialGenerator::GBP_HAS_DIFFUSE_COLOUR)
    {
        ss << "uniform vec4 cDiffuseColour;" << std::endl;
    }

    ss << "uniform float cFarDistance;" << std::endl;
    ss << "uniform float cSpecularity;" << std::endl;
    ss << "void main()" << std::endl;
    ss << "{" << std::endl;

    if (numTexCoords > 0 && numTextures > 0)
    {
        ss << outData << "[0].rgb = " << textureFunc << "(sTex0, oUv0).rgb;" << std::endl;
        if (permutation & GBufferMaterialGenerator::GBP_HAS_DIFFUSE_COLOUR)
        {
            ss << outData << "[0].rgb *= cDiffuseColour.rgb;" << std::endl;
        }
		ss << outData << "[2].rgb = " << textureFunc << "(sGlowMap, oUv0).rgb;" << std::endl;
    }
    else
    {
        ss << outData << "[0].rgb = cDiffuseColour.rgb;" << std::endl;
		ss << outData << "[2].rgb = vec3(0, 0, 0);" << std::endl;
    }
	 
    ss << outData << "[0].a = cSpecularity;" << std::endl;
    if (permutation & GBufferMaterialGenerator::GBP_NORMAL_MAP)
    {
        ss << "vec3 texNormal = (" << textureFunc << "(sNormalMap, oUv0).rgb-0.5)*2.0;" << std::endl;
        ss << "mat3 normalRotation = mat3(oTangent, oBiNormal, oNormal);" << std::endl;
        ss << outData << "[1].rgb = normalize(texNormal * normalRotation);" << std::endl;
    }
    else
    {
        ss << outData << "[1].rgb = normalize(oNormal);" << std::endl;
    }
    ss << outData << "[1].rgb = normalize(oNormal);" << std::endl;
    ss << outData << "[1].a = length(oViewPos) / cFarDistance;" << std::endl;

    ss << "}" << std::endl;

    String programSource = ss.str();
    String programName = mBaseName + "FP_" + StringConverter::toString(permutation);
	
	Ogre::Log* msg = LogManager::getSingleton().getDefaultLog();
	msg->logMessage("vvvvvvvvvvvvvvvvvv BEGIN SOURCE vvvvvvvvvvvvvvvvvv");
	msg->logMessage("vvvvvvvvvvvvvvvvvv" + programName + " vvvvvvvvvvvvvvvvvv");
	msg->logMessage(programSource);
	msg->logMessage("^^^^^^^^^^^^^^^^^^ END SOURCE ^^^^^^^^^^^^^^^^^^");

    // Create shader object
    HighLevelGpuProgramPtr ptrProgram;

        ptrProgram = HighLevelGpuProgramManager::getSingleton().createProgram(
			programName,
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            "glsl", GPT_FRAGMENT_PROGRAM);
    
    ptrProgram->setSource(programSource);

    const GpuProgramParametersSharedPtr& params = ptrProgram->getDefaultParameters();
    params->setNamedAutoConstant("cSpecularity", GpuProgramParameters::ACT_SURFACE_SHININESS);
    if (numTextures == 0 || permutation & GBufferMaterialGenerator::GBP_HAS_DIFFUSE_COLOUR)
    {
        params->setNamedAutoConstant("cDiffuseColour", GpuProgramParameters::ACT_SURFACE_DIFFUSE_COLOUR);
    }

    // Bind samplers
    int samplerNum = 0;
    if (permutation & GBufferMaterialGenerator::GBP_NORMAL_MAP)
    {
        params->setNamedConstant("sNormalMap", samplerNum++);
		msg->logMessage("binding sNormalMap to " + StringConverter::toString(samplerNum));
    }
	msg->logMessage("binding sTex0 to " + StringConverter::toString(samplerNum));
    params->setNamedConstant("sTex0", samplerNum);	

	samplerNum++;

	msg->logMessage("binding sGlowMap to " + StringConverter::toString(samplerNum));
    params->setNamedConstant("sGlowMap", samplerNum);

    params->setNamedAutoConstant("cFarDistance", GpuProgramParameters::ACT_FAR_CLIP_DISTANCE);
        
    ptrProgram->load();
    return GpuProgramPtr(ptrProgram);
}

MaterialPtr GBufferMaterialGeneratorImpl::generateTemplateMaterial(MaterialGenerator::Perm permutation)
{
	String matName = mBaseName + "Mat_" + StringConverter::toString(permutation);

	MaterialPtr matPtr = MaterialManager::getSingleton().create
		(matName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Pass* pass = matPtr->getTechnique(0)->getPass(0);
	pass->setName(mBaseName + "Pass_" + StringConverter::toString(permutation));
	pass->setLightingEnabled(false);
	if (permutation & GBufferMaterialGenerator::GBP_NORMAL_MAP)
	{
		pass->createTextureUnitState();
	}
	uint32 numTextures = permutation & GBufferMaterialGenerator::GBP_TEXTURE_MASK;
	for (uint32 i=0; i<numTextures; i++)
	{
		pass->createTextureUnitState();
	}

	return matPtr;
}
