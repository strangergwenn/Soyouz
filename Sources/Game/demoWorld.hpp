/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __DEMOWORLD_H_
#define __DEMOWORLD_H_

#include "Engine/ogre.hpp"
#include "Engine/game.hpp"
#include "Engine/actor.hpp"
#include "Engine/player.hpp"
#include "Engine/meshactor.hpp"
#include "Engine/lightactor.hpp"
#include "Game/demoPlayer.hpp"

#include "Engine/DeferredShading/SharedData.h"


/*----------------------------------------------
Class definitions
----------------------------------------------*/

class DemoWorld : public Game
{
	
	void construct()
	{/*
		// Sun & space
		mScene->setSkyBox(true, "Skybox");
		Ogre::Light* sun = mScene->createLight("Sun");
		sun->setType(Ogre::Light::LT_DIRECTIONAL);
		sun->setDirection(Vector3(-0.2f, 0.1f, -1.0f));
		sun->setDiffuseColour(Ogre::ColourValue(1.5f, 1.2f, 1.0f));
		sun->setSpecularColour(Ogre::ColourValue(1.5f, 1.2f, 1.0f));
		
		// Back sun
		Ogre::Light* sun2 = mScene->createLight("Sun2");
		sun2->setType(Ogre::Light::LT_DIRECTIONAL);
		sun2->setDirection(Vector3(0.2f, -0.1f, 1.0f));
		sun2->setDiffuseColour(Ogre::ColourValue(0.80f, 0.80f, 1.0f));
		sun2->setSpecularColour(Ogre::ColourValue(0.80f, 0.80f, 1.0f));

		// Collision crate
		MeshActor* crate = new MeshActor(this, "crate", "crate.mesh", "Default", true, 1.0f);
		crate->setLocation(Vector3(0, 0, -50));
		
		Ogre::Real distance = 2000000;
		Ogre::Real scale = 30000;

		Ogre::SceneNode* mSceneNode = mScene->getRootSceneNode()->createChildSceneNode();
		Ogre::Entity* mEntity = mScene->createEntity("sphere1", Ogre::SceneManager::PT_SPHERE);
		mEntity->setMaterialName("Default");
		mEntity->setCastShadows(true);
		mSceneNode->attachObject(mEntity);
		mSceneNode->setScale(scale, scale, scale);
		mSceneNode->setPosition(Vector3(0, -distance, 0));

		// Airlock
		//MeshActor* act2 = new MeshActor(this, "DemoMesh", "Airlock.mesh", "MI_Airlock", true, 0.0f);
		//act2->setLocation(Vector3(-256,-128,0));
		//act2->setRotation(Vector3(0,90,0));
		
		// zCrate
		Ogre::SceneNode* mSceneNode2 = mScene->getRootSceneNode()->createChildSceneNode();
		Ogre::Entity* mEntity2 = mScene->createEntity("zcrate", Ogre::SceneManager::PT_CUBE);
		mEntity2->setMaterialName("MI_zCrate");
		mEntity2->setCastShadows(true);
		mSceneNode2->attachObject(mEntity2);
		mSceneNode2->setPosition(Vector3(-128, -64, -64));*/
		
		new SharedData();

		Ogre::Light* l1 = mScene->createLight();
        l1->setType(Ogre::Light::LT_DIRECTIONAL);
        l1->setDiffuseColour(0.5f, 0.45f, 0.1f);
		l1->setDirection(1, -0.5f, -0.2f);
		l1->setShadowFarClipDistance(250);
		l1->setShadowFarDistance(75);
		l1->setCastShadows(false);
		
		Ogre::Camera* mCamera = mPlayer->getCamera();
		mCamera->setPosition(-4, 7, 0);
        mCamera->lookAt(-8,5,0);
		mCamera->setFarClipDistance(1000.0);
        mCamera->setNearClipDistance(0.5);
        
		DeferredShadingSystem* mSystem = new DeferredShadingSystem(mWindow->getViewport(0), mScene, mCamera);
		SharedData::getSingleton().iSystem = mSystem;
		mSystem->initialize();
        
		SharedData::getSingleton().iCamera = mCamera;
		SharedData::getSingleton().iRoot = mRoot;
		SharedData::getSingleton().iWindow = mWindow;
		SharedData::getSingleton().iActivate = true;
		SharedData::getSingleton().iGlobalActivate = true;
		SharedData::getSingleton().iSystem->setSSAO(true);
		SharedData::getSingleton().iSystem->setActive(true);
		SharedData::getSingleton().iMainLight = l1;
		SharedData::getSingleton().iMainLight->setVisible(true);

		setupContent();
	}

	void destruct(){}

	/*
	void setupPlayer()
	{
		mPlayer = new DemoPlayer(this, "LocalPlayer");
	}
	*/

    // Just override the mandatory create scene method
    void setupContent(void)
    {
		Ogre::SceneNode* rootNode = mScene->getRootSceneNode()->createChildSceneNode();
		Ogre::Entity* cathedralEnt = mScene->createEntity("Cathedral", "sibenik.mesh");
        Ogre::SceneNode* cathedralNode = rootNode->createChildSceneNode();
        cathedralNode->attachObject(cathedralEnt);
		
        createAtheneScene(rootNode);
		createKnotScene(rootNode);
	}
	    
	/*void itemSelected(SelectMenu* menu)
	{
		SharedData::getSingleton().iSystem->setMode((DeferredShadingSystem::DSMode)menu->getSelectionIndex());
	}*/
    
	
    //Utility function to help set scene up
    void setEntityHeight(Ogre::Entity* ent, Real newHeight)
    {
        Real curHeight = ent->getMesh()->getBounds().getSize().y;
        Real scaleFactor = newHeight / curHeight;
        
        Ogre::SceneNode* parentNode = ent->getParentSceneNode();
        parentNode->setScale(scaleFactor, scaleFactor, scaleFactor);
    }
    
	void createAtheneScene(Ogre::SceneNode* rootNode)
	{
		// Prepare athene mesh for normalmapping
        Ogre::MeshPtr pAthene = Ogre::MeshManager::getSingleton().load("athene.mesh", 
                                                           Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        unsigned short src, dest;
        if (!pAthene->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
            pAthene->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
        
		//Create an athena statue
        Ogre::Entity* athena = mScene->createEntity("Athena", "athene.mesh");
		athena->setMaterialName("DeferredDemo/DeferredAthena");
		Ogre::SceneNode *aNode = rootNode->createChildSceneNode();
		aNode->attachObject( athena );
		aNode->setPosition(-8.5, 4.5, 0);
        setEntityHeight(athena, 4.0);
        aNode->yaw(Ogre::Degree(90));
		// Create some happy little lights to decorate the athena statue
		createSampleLights();
	}
    
	void createKnotScene(Ogre::SceneNode* rootNode)
	{
		// Prepare knot mesh for normal mapping
		Ogre::MeshPtr pKnot = Ogre::MeshManager::getSingleton().load("knot.mesh", 
                                                         Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		unsigned short src, dest;
        if (!pKnot->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
            pKnot->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
        
		
		Ogre::Entity* crate = mScene->createEntity("Crate", "crate.mesh");
		crate->setMaterialName("DeferredDemo/Crate");
		Ogre::SceneNode* cratenode = rootNode->createChildSceneNode(Vector3(-8,3.5,1.0));
        cratenode->attachObject(crate);
		cratenode->setScale(0.01f * Vector3(1,1,1));

		// Create a bunch of knots with spotlights hanging from above
		Ogre::Entity* knotEnt = mScene->createEntity("Knot", "knot.mesh");
		knotEnt->setMaterialName("DeferredDemo/RockWall");
		//knotEnt->setMeshLodBias(0.25f);
        Vector3 knotStartPos(25.5f, 2, 5.5f);
        Vector3 knotDiff(-3.7f, 0, 0);
        for (int i=0; i < 5; i++)
        {
            char cloneName[16];
			sprintf(cloneName, "Knot%d", i);
            Ogre::Entity* cloneKnot = knotEnt->clone(cloneName);
            Vector3 clonePos = knotStartPos + knotDiff * (float)i;
			Ogre::SceneNode* cloneNode = rootNode->createChildSceneNode(clonePos);
            cloneNode->attachObject(cloneKnot);
            setEntityHeight(cloneKnot, 3);
            cloneNode->yaw(Degree((float)i * 17));
            cloneNode->roll(Degree((float)i * 31));
            
            sprintf(cloneName, "KnotLight%d", i);
            Ogre::Light* knotLight = mScene->createLight(cloneName);
            knotLight->setType(Ogre::Light::LT_SPOTLIGHT);
            knotLight->setDiffuseColour(SAMPLE_COLORS[i]);
            knotLight->setSpecularColour(Ogre::ColourValue::White);
            knotLight->setPosition(clonePos + Vector3(0,3,0));
            knotLight->setDirection(Vector3::NEGATIVE_UNIT_Y);
            knotLight->setSpotlightRange(Degree(25), Degree(45), 1);
            knotLight->setAttenuation(6, 1, 0.2f, 0);
        }
	}
    
	void createSampleLights()
	{
		// Create some lights		
		Ogre::vector<Ogre::Light*>::type lights;
		Ogre::SceneNode *parentNode = mScene->getRootSceneNode()->createChildSceneNode("LightsParent");
		// Create light nodes
		Ogre::vector<Ogre::Node*>::type nodes;
        
        Vector4 attParams = Vector4(20,1,0,1);
        Real lightRadius = 100;
        
		Ogre::Light *a = mScene->createLight();
		Ogre::SceneNode *an = parentNode->createChildSceneNode();
		an->attachObject(a);
		a->setAttenuation(attParams.x, attParams.y, attParams.z, attParams.w);
		//a->setAttenuation(1.0f, 0.000f, 0.000f);
		an->setPosition(0,0,lightRadius);
		a->setDiffuseColour(1,0,0);
		//a->setSpecularColour(0.5,0,0);
		lights.push_back(a);
		nodes.push_back(an);
        
		Ogre::Light *b = mScene->createLight();
		Ogre::SceneNode *bn = parentNode->createChildSceneNode();
		bn->attachObject(b);
		b->setAttenuation(attParams.x, attParams.y, attParams.z, attParams.w);
		bn->setPosition(lightRadius,0,0);
		b->setDiffuseColour(1,1,0);
		//b->setSpecularColour(0.5,0.5,0);
		lights.push_back(b);
		nodes.push_back(bn);
        
		Ogre::Light *c = mScene->createLight();
		Ogre::SceneNode *cn = parentNode->createChildSceneNode();
		cn->attachObject(c);
		c->setAttenuation(attParams.x, attParams.y, attParams.z, attParams.w);
		cn->setPosition(0,0,-lightRadius);
		c->setDiffuseColour(0,1,1);
		c->setSpecularColour(0.25,1.0,1.0); // Cyan light has specular component
		lights.push_back(c);
		nodes.push_back(cn);
        
		Ogre::Light *d = mScene->createLight();
		Ogre::SceneNode *dn = parentNode->createChildSceneNode();
		dn->attachObject(d);
		d->setAttenuation(attParams.x, attParams.y, attParams.z, attParams.w);
		dn->setPosition(-lightRadius,0,0);
		d->setDiffuseColour(1,0,1);
		d->setSpecularColour(0.0,0,0.0);
		lights.push_back(d);
		nodes.push_back(dn);
        
		Ogre::Light *e = mScene->createLight();
		Ogre::SceneNode *en = parentNode->createChildSceneNode();
		en->attachObject(e);
		e->setAttenuation(attParams.x, attParams.y, attParams.z, attParams.w);
		en->setPosition(lightRadius,0,lightRadius);
		e->setDiffuseColour(0,0,1);
		e->setSpecularColour(0,0,0);
		lights.push_back(e);
		nodes.push_back(en);
		
		Ogre::Light *f = mScene->createLight();
		Ogre::SceneNode *fn = parentNode->createChildSceneNode();
		fn->attachObject(f);
		f->setAttenuation(attParams.x, attParams.y, attParams.z, attParams.w);
		fn->setPosition(-lightRadius,0,-lightRadius);
		f->setDiffuseColour(0,1,0);
		f->setSpecularColour(0,0.0,0.0);
		lights.push_back(f);
		nodes.push_back(fn);
        
		// Create marker meshes to show user where the lights are
		Ogre::Entity *ent;
		GeomUtils::createSphere("PointLightMesh", 0.02f, 5, 5, true, true);
		for(Ogre::vector<Ogre::Light*>::type::iterator i=lights.begin(); i!=lights.end(); ++i)
		{
			Ogre::Light* light = *i;
			ent = mScene->createEntity(light->getName()+"v", "PointLightMesh");
			String matname = light->getName()+"m";
			// Create coloured material
			Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create(matname,
                                                                     Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
            Ogre::Pass* pass = mat->getTechnique(0)->getPass(0);
            pass->setDiffuse(0.0f,0.0f,0.0f,1.0f);
			pass->setAmbient(0.0f,0.0f,0.0f);
			pass->setSelfIllumination(light->getDiffuseColour());
            
			ent->setMaterialName(matname);
			//ent->setRenderQueueGroup(light->getRenderQueueGroup());
			ent->setRenderQueueGroup(DeferredShadingSystem::POST_GBUFFER_RENDER_QUEUE);
			static_cast<Ogre::SceneNode*>(light->getParentNode())->attachObject(ent);
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
		Ogre::Animation* anim = mScene->createAnimation("LightSwarmTrack", stations*seconds_per_station);
		// Spline it for nice curves
		anim->setInterpolationMode(Ogre::Animation::IM_SPLINE);
		for(unsigned int x=0; x<nodes.size(); ++x)
		{
			// Create a track to animate the camera's node
			Ogre::NodeAnimationTrack* track = anim->createNodeTrack(x, nodes[x]);
			for(size_t y=0; y<=stations; ++y)
			{
				// Setup keyframes
				Ogre::TransformKeyFrame* key = track->createNodeKeyFrame(y*seconds_per_station); // A start position
				key->setTranslate(station_pos[(x*skip+y)%stations]);
				// Make sure size of light doesn't change
				key->setScale(nodes[x]->getScale());
			}
		}
		// Create a new animation state to track this
		SharedData::getSingleton().mMLAnimState = mScene->createAnimationState("LightSwarmTrack");
		SharedData::getSingleton().mMLAnimState->setEnabled(true);
        
	}
};

#endif /* __DEMOWORLD_H_ */
