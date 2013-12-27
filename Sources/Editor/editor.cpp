/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Editor/editor.hpp"
#include "Editor/user.hpp"
#include "Engine/game.hpp"
#include "Engine/player.hpp"
#include "Engine/meshactor.hpp"


/*----------------------------------------------
	Methods
----------------------------------------------*/


void Editor::rotateLights(Ogre::Degree pitch, Ogre::Degree roll)
{
}


void Editor::construct()
{
	// Lighting and background
	new MeshActor(this, "Background", "testbed.mesh", "Default");
	Ogre::Light* l1 = mScene->createLight();
    l1->setType(Ogre::Light::LT_DIRECTIONAL);
    l1->setDiffuseColour(1.95f, 1.95f, 1.95f);
    l1->setSpecularColour(1.95f, 1.95f, 1.95f);
	l1->setDirection(0, -0.1f, -1.0f);
	l1->setCastShadows(false);

	// Resources preparation
	int i = 0;
	mSphereGap = 20.0f;
	Ogre::MaterialManager &matMan = Ogre::MaterialManager::getSingleton();
	Ogre::ResourceManager::ResourceMapIterator it = matMan.getResourceIterator();

	// Main resource loop to get all materials
	while (it.hasMoreElements())
    {
		Ogre::ResourcePtr data = it.getNext();
		Ogre::String matGroup = data.getPointer()->getGroup();
		if (matGroup != "Core")
		{
			Ogre::String matName = data.getPointer()->getName();
			MeshActor* sphere = new MeshActor(this, "Sphere_" + Ogre::StringConverter::toString(i), "sphere.mesh", matName);
			sphere->translate(Vector3(i * mSphereGap, 0, 0));
			mSpheres.push_back(sphere);
			i++;
		}
	}
}

void Editor::setupPlayer()
{
	mPlayer = new User(this, "LocalPlayer");
}


void Editor::step(bool bRight)
{
	for (Ogre::vector<MeshActor*>::iterator it = mSpheres.begin(); it != mSpheres.end(); it++)
	{
		(*it)->translate(Vector3((bRight ? 1 : -1) * mSphereGap, 0, 0));
	}
}
