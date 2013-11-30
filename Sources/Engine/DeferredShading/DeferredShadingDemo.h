

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

#ifndef H_DeferredShadingDemo
#define H_DeferredShadingDemo

#include "Ogre.h"
#include "SdkSample.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#include "SharedData.h"
#include "OgreCompositorInstance.h"
#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "OgreMaterial.h"
#include "OgreRenderTargetListener.h"
#include "GeomUtils.h"

using namespace Ogre;
using namespace OgreBites;

class _OgreSampleClassExport Sample_DeferredShading : public SdkSample, public RenderTargetListener
{
protected:
	DeferredShadingSystem *mSystem;
	SelectMenu* mDisplayModeMenu;
    
public:
    Sample_DeferredShading()
	{
		mInfo["Title"] = "Deferred Shading";
		mInfo["Description"] = "A sample implementation of a deferred renderer using the compositor framework.";
		mInfo["Thumbnail"] = "thumb_deferred.png";
		mInfo["Category"] = "Lighting";
		mInfo["Help"] = "See http://www.ogre3d.org/wiki/index.php/Deferred_Shading for more info";
	}

protected:
        
	void setupControls()
	{
		mTrayMgr->showCursor();
		mDisplayModeMenu = mTrayMgr->createThickSelectMenu(TL_TOPLEFT, "DisplayMode", "Display Mode", 220, 4);
		mDisplayModeMenu->addItem("Regular view");
		mDisplayModeMenu->addItem("Debug colours");
		mDisplayModeMenu->addItem("Debug normals");
		mDisplayModeMenu->addItem("Debug depth / specular");
		
		SharedData::getSingleton().iSystem->setSSAO(true);
		SharedData::getSingleton().iGlobalActivate = true;
		SharedData::getSingleton().iMainLight->setVisible(true);
		SharedData::getSingleton().iSystem->setActive(true);
		mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
	}
    
	void itemSelected(SelectMenu* menu)
	{
		SharedData::getSingleton().iSystem->setMode((DeferredShadingSystem::DSMode)menu->getSelectionIndex());
	}
    

	///////////////////////////////////////////////
	/////////////// CONTENT
	///////////////////////////////////////////////

	
	bool frameRenderingQueued(const FrameEvent& evt)
	{
		if( SdkSample::frameRenderingQueued(evt) == false )
			return false;
		SharedData::getSingleton().iLastFrameTime = evt.timeSinceLastFrame;
        
		if (SharedData::getSingleton().mMLAnimState)
			SharedData::getSingleton().mMLAnimState->addTime(evt.timeSinceLastFrame);
		return true;
	}
    

    //Utility function to help set scene up
    void setEntityHeight(Entity* ent, Real newHeight)
    {
        Real curHeight = ent->getMesh()->getBounds().getSize().y;
        Real scaleFactor = newHeight / curHeight;
        
        SceneNode* parentNode = ent->getParentSceneNode();
        parentNode->setScale(scaleFactor, scaleFactor, scaleFactor);
    }
    
	void createAtheneScene(SceneNode* rootNode)
	{
		// Prepare athene mesh for normalmapping
        MeshPtr pAthene = MeshManager::getSingleton().load("athene.mesh", 
                                                           ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        unsigned short src, dest;
        if (!pAthene->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
            pAthene->buildTangentVectors(VES_TANGENT, src, dest);
        
		//Create an athena statue
        Entity* athena = mSceneMgr->createEntity("Athena", "athene.mesh");
		athena->setMaterialName("DeferredDemo/DeferredAthena");
		SceneNode *aNode = rootNode->createChildSceneNode();
		aNode->attachObject( athena );
		aNode->setPosition(-8.5, 4.5, 0);
        setEntityHeight(athena, 4.0);
        aNode->yaw(Ogre::Degree(90));
		// Create some happy little lights to decorate the athena statue
		createSampleLights();
	}
    
	void createKnotScene(SceneNode* rootNode)
	{
		// Prepare knot mesh for normal mapping
		MeshPtr pKnot = MeshManager::getSingleton().load("knot.mesh", 
                                                         ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		unsigned short src, dest;
        if (!pKnot->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
            pKnot->buildTangentVectors(VES_TANGENT, src, dest);
        
		
		Entity* crate = mSceneMgr->createEntity("Crate", "crate.mesh");
		crate->setMaterialName("DeferredDemo/Crate");
		SceneNode* cratenode = rootNode->createChildSceneNode(Vector3(-8,3.5,1.0));
        cratenode->attachObject(crate);
		cratenode->setScale(0.01 * Vector3(1,1,1));

		// Create a bunch of knots with spotlights hanging from above
		Entity* knotEnt = mSceneMgr->createEntity("Knot", "knot.mesh");
		knotEnt->setMaterialName("DeferredDemo/RockWall");
		//knotEnt->setMeshLodBias(0.25f);
        Vector3 knotStartPos(25.5, 2, 5.5);
        Vector3 knotDiff(-3.7, 0, 0);
        for (int i=0; i < 5; i++)
        {
            char cloneName[16];
			sprintf(cloneName, "Knot%d", i);
            Entity* cloneKnot = knotEnt->clone(cloneName);
            Vector3 clonePos = knotStartPos + knotDiff*i;
			SceneNode* cloneNode = rootNode->createChildSceneNode(clonePos);
            cloneNode->attachObject(cloneKnot);
            setEntityHeight(cloneKnot, 3);
            cloneNode->yaw(Degree(i*17));
            cloneNode->roll(Degree(i*31));
            
            sprintf(cloneName, "KnotLight%d", i);
            Light* knotLight = mSceneMgr->createLight(cloneName);
            knotLight->setType(Light::LT_SPOTLIGHT);
            knotLight->setDiffuseColour(SAMPLE_COLORS[i]);
            knotLight->setSpecularColour(ColourValue::White);
            knotLight->setPosition(clonePos + Vector3(0,3,0));
            knotLight->setDirection(Vector3::NEGATIVE_UNIT_Y);
            knotLight->setSpotlightRange(Degree(25), Degree(45), 1);
            knotLight->setAttenuation(6, 1, 0.2, 0);
        }
	}

    // Just override the mandatory create scene method
    void setupContent(void)
    {
		mCameraMan->setTopSpeed(20.0);
		new SharedData();
		mSystem = 0;

        // Set ambient light
        mSceneMgr->setAmbientLight(ColourValue(0.15, 0.00, 0.00));
        // Skybox
        // Create main, static light
		Light* l1 = mSceneMgr->createLight();
        l1->setType(Light::LT_DIRECTIONAL);
        l1->setDiffuseColour(0.5f, 0.45f, 0.1f);
		l1->setDirection(1, -0.5, -0.2);
		l1->setShadowFarClipDistance(250);
		l1->setShadowFarDistance(75);
		//Turn this on to have the directional light cast shadows
		l1->setCastShadows(false);
		
		mCamera->setPosition(25, 5, 0);
        mCamera->lookAt(0,0,0);
		mCamera->setFarClipDistance(1000.0);
        mCamera->setNearClipDistance(0.5);
		setDragLook(true);
        
		mSystem = new DeferredShadingSystem(mWindow->getViewport(0), mSceneMgr, mCamera);
		SharedData::getSingleton().iSystem = mSystem;
		mSystem->initialize();
        
        // safely setup application's (not postfilter!) shared data
		SharedData::getSingleton().iCamera = mCamera;
		SharedData::getSingleton().iRoot = mRoot;
		SharedData::getSingleton().iWindow = mWindow;
		SharedData::getSingleton().iActivate = true;
		SharedData::getSingleton().iGlobalActivate = true;
		SharedData::getSingleton().iMainLight = l1;
        
		//Create the scene
		// Create "root" node
		SceneNode* rootNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        
        // Create the cathedral - this will be the static scene
		Entity* cathedralEnt = mSceneMgr->createEntity("Cathedral", "sibenik.mesh");
        SceneNode* cathedralNode = rootNode->createChildSceneNode();
        cathedralNode->attachObject(cathedralEnt);
		
        createAtheneScene(rootNode);
		createKnotScene(rootNode);
		setupControls();
	}
    
	void createSampleLights()
	{
		// Create some lights		
		vector<Light*>::type lights;
		SceneNode *parentNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("LightsParent");
		// Create light nodes
		vector<Node*>::type nodes;
        
        Vector4 attParams = Vector4(20,1,0,1);
        Real lightRadius = 100;
        
		Light *a = mSceneMgr->createLight();
		SceneNode *an = parentNode->createChildSceneNode();
		an->attachObject(a);
		a->setAttenuation(attParams.x, attParams.y, attParams.z, attParams.w);
		//a->setAttenuation(1.0f, 0.000f, 0.000f);
		an->setPosition(0,0,lightRadius);
		a->setDiffuseColour(1,0,0);
		//a->setSpecularColour(0.5,0,0);
		lights.push_back(a);
		nodes.push_back(an);
        
		Light *b = mSceneMgr->createLight();
		SceneNode *bn = parentNode->createChildSceneNode();
		bn->attachObject(b);
		b->setAttenuation(attParams.x, attParams.y, attParams.z, attParams.w);
		bn->setPosition(lightRadius,0,0);
		b->setDiffuseColour(1,1,0);
		//b->setSpecularColour(0.5,0.5,0);
		lights.push_back(b);
		nodes.push_back(bn);
        
		Light *c = mSceneMgr->createLight();
		SceneNode *cn = parentNode->createChildSceneNode();
		cn->attachObject(c);
		c->setAttenuation(attParams.x, attParams.y, attParams.z, attParams.w);
		cn->setPosition(0,0,-lightRadius);
		c->setDiffuseColour(0,1,1);
		c->setSpecularColour(0.25,1.0,1.0); // Cyan light has specular component
		lights.push_back(c);
		nodes.push_back(cn);
        
		Light *d = mSceneMgr->createLight();
		SceneNode *dn = parentNode->createChildSceneNode();
		dn->attachObject(d);
		d->setAttenuation(attParams.x, attParams.y, attParams.z, attParams.w);
		dn->setPosition(-lightRadius,0,0);
		d->setDiffuseColour(1,0,1);
		d->setSpecularColour(0.0,0,0.0);
		lights.push_back(d);
		nodes.push_back(dn);
        
		Light *e = mSceneMgr->createLight();
		SceneNode *en = parentNode->createChildSceneNode();
		en->attachObject(e);
		e->setAttenuation(attParams.x, attParams.y, attParams.z, attParams.w);
		en->setPosition(lightRadius,0,lightRadius);
		e->setDiffuseColour(0,0,1);
		e->setSpecularColour(0,0,0);
		lights.push_back(e);
		nodes.push_back(en);
		
		Light *f = mSceneMgr->createLight();
		SceneNode *fn = parentNode->createChildSceneNode();
		fn->attachObject(f);
		f->setAttenuation(attParams.x, attParams.y, attParams.z, attParams.w);
		fn->setPosition(-lightRadius,0,-lightRadius);
		f->setDiffuseColour(0,1,0);
		f->setSpecularColour(0,0.0,0.0);
		lights.push_back(f);
		nodes.push_back(fn);
        
		// Create marker meshes to show user where the lights are
		Entity *ent;
		GeomUtils::createSphere("PointLightMesh", 0.02f, 5, 5, true, true);
		for(vector<Light*>::type::iterator i=lights.begin(); i!=lights.end(); ++i)
		{
			Light* light = *i;
			ent = mSceneMgr->createEntity(light->getName()+"v", "PointLightMesh");
			String matname = light->getName()+"m";
			// Create coloured material
			MaterialPtr mat = MaterialManager::getSingleton().create(matname,
                                                                     ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
            Pass* pass = mat->getTechnique(0)->getPass(0);
            pass->setDiffuse(0.0f,0.0f,0.0f,1.0f);
			pass->setAmbient(0.0f,0.0f,0.0f);
			pass->setSelfIllumination(light->getDiffuseColour());
            
			ent->setMaterialName(matname);
			//ent->setRenderQueueGroup(light->getRenderQueueGroup());
			ent->setRenderQueueGroup(DeferredShadingSystem::POST_GBUFFER_RENDER_QUEUE);
			static_cast<SceneNode*>(light->getParentNode())->attachObject(ent);
			ent->setVisible(true);
		}		
        
		// Store nodes for hiding/showing
		SharedData::getSingleton().mLightNodes = nodes;
        
		// Do some animation for node a-f
		// Generate helix structure
		float seconds_per_station = 1.0f;
		float r = 1.0;
		//Vector3 base(0,-30,0);
		Vector3 base(-8.75, 3.5, 0);
        
		float h=3;
		const size_t s_to_top = 16;
		const size_t stations = s_to_top*2-1;
		float ascend = h/((float)s_to_top);
		float stations_per_revolution = 3.5f;
		size_t skip = 2; // stations between lights
		Vector3 station_pos[stations];
		for(size_t x=0; x<s_to_top; ++x)
		{
			float theta = ((float)x/stations_per_revolution)*2.0f*Math::PI;
			station_pos[x] = base+Vector3(Math::Sin(theta)*r, ascend*x, Math::Cos(theta)*r);
		}
		for(size_t x=s_to_top; x<stations; ++x)
		{
			float theta = ((float)x/stations_per_revolution)*2.0f*Math::PI;
			station_pos[x] = base+Vector3(Math::Sin(theta)*r, h-ascend*(x-s_to_top), Math::Cos(theta)*r);
		}
		// Create a track for the light swarm
		Animation* anim = mSceneMgr->createAnimation("LightSwarmTrack", stations*seconds_per_station);
		// Spline it for nice curves
		anim->setInterpolationMode(Animation::IM_SPLINE);
		for(unsigned int x=0; x<nodes.size(); ++x)
		{
			// Create a track to animate the camera's node
			NodeAnimationTrack* track = anim->createNodeTrack(x, nodes[x]);
			for(size_t y=0; y<=stations; ++y)
			{
				// Setup keyframes
				TransformKeyFrame* key = track->createNodeKeyFrame(y*seconds_per_station); // A start position
				key->setTranslate(station_pos[(x*skip+y)%stations]);
				// Make sure size of light doesn't change
				key->setScale(nodes[x]->getScale());
			}
		}
		// Create a new animation state to track this
		SharedData::getSingleton().mMLAnimState = mSceneMgr->createAnimationState("LightSwarmTrack");
		SharedData::getSingleton().mMLAnimState->setEnabled(true);
        
	}
};

#endif
