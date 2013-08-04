/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/meshactor.hpp"


/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

MeshActor::MeshActor(Game* g, String name, String file)
	: Actor(g, name)
{
	mMesh = g->createGameEntity(name + "_mesh", file);
	mMesh->setCastShadows(true);
	mNode->attachObject(mMesh);
}


MeshActor::MeshActor(Game* g, String name, String file, String material)
	: Actor(g, name)
{
	mMesh = g->createGameEntity(name + "_mesh", file);
	mMesh->setCastShadows(true);
	mNode->attachObject(mMesh);
	setMaterial(material);
}


MeshActor::MeshActor(Game* g, String name, String file, String material, bool bCastShadows)
	: Actor(g, name)
{
	mMesh = g->createGameEntity(name + "_mesh", file);
	mMesh->setCastShadows(bCastShadows);
	mNode->attachObject(mMesh);
	setMaterial(material);
}


MeshActor::~MeshActor()
{
}


/*----------------------------------------------
	Materials
----------------------------------------------*/

void MeshActor::setMaterial(String name)
{
	mMesh->setMaterialName(name);
}


void MeshActor::setMaterialParam(String name, Real val)
{
	Pass* pPass = mMesh->getSubEntity(0)->getMaterial()->getTechnique("main")->getPass("main");
	pPass->getFragmentProgramParameters()->setNamedConstant(name, val);
}


void MeshActor::setMaterialParam(String name, Vector3 val)
{
	Pass* pPass = mMesh->getSubEntity(0)->getMaterial()->getTechnique("main")->getPass("main");
	pPass->getFragmentProgramParameters()->setNamedConstant(name, val);
}


void MeshActor::setMaterialParam(String name, Vector4 val)
{
	Pass* pPass = mMesh->getSubEntity(0)->getMaterial()->getTechnique("main")->getPass("main");
	pPass->getFragmentProgramParameters()->setNamedConstant(name, val);
}
