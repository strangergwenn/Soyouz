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
	mPhysBody = NULL;
}


MeshActor::MeshActor(Game* g, String name, String file, String material)
	: Actor(g, name)
{
	mMesh = g->createGameEntity(name + "_mesh", file);
	mMesh->setCastShadows(true);
	mNode->attachObject(mMesh);
	setMaterial(material);
	mPhysBody = NULL;
}


MeshActor::MeshActor(Game* g, String name, String file, String material, bool bCastShadows)
	: Actor(g, name)
{
	mMesh = g->createGameEntity(name + "_mesh", file);
	mMesh->setCastShadows(bCastShadows);
	mNode->attachObject(mMesh);
	setMaterial(material);
	mPhysBody = NULL;
}


MeshActor::MeshActor(Game* g, String name, String file, String material, bool bCastShadows, float mass)
	: Actor(g, name)
{
	// Render
	mMesh = g->createGameEntity(name + "_mesh", file);
	mMesh->setCastShadows(bCastShadows);
	mNode->attachObject(mMesh);
	setMaterial(material);

	// Physics settings
	mPhysShape = new btBoxShape(btVector3(10,10,10));
	mPhysTransform.setIdentity();
	mPhysTransform.setOrigin(btVector3(0, 0, 0));
	btVector3 localInertia(0,0,0);
	mPhysMass = 0.5f;
	
	// Physics setup
	mPhysShape->calculateLocalInertia(mass, localInertia);
	mPhysMotionState = new btDefaultMotionState(mPhysTransform);
	btRigidBody::btRigidBodyConstructionInfo rbConstruct(
		mass,
		mPhysMotionState,
		mPhysShape,
		localInertia);
	mPhysBody = new btRigidBody(rbConstruct);
	mGame->addRigidBody(mPhysBody);
}


MeshActor::~MeshActor()
{
}


/*----------------------------------------------
	Methods
----------------------------------------------*/

void MeshActor::tick(const FrameEvent& evt)
{
	if (mPhysBody)
	{
		mPhysBody->getMotionState()->getWorldTransform(mPhysTransform);
		btQuaternion rotation(mPhysTransform.getRotation());
		mNode->setOrientation(rotation.getW(), rotation.getX(), rotation.getY(), rotation.getZ());
		const btVector3 &origin = mPhysTransform.getOrigin();
		mNode->setPosition(origin.getX(), origin.getY(), origin.getZ());
	}
}


void MeshActor::setLocation(Vector3 newPos)
{
	Vector3 location;
	btTransform trans;

	Actor::setLocation(newPos);

	if (mPhysBody)
	{
		location = mNode->getPosition();
		mPhysBody->getMotionState()->getWorldTransform(trans);
		mPhysBody->translate(btVector3(
			location[0] - trans.getOrigin().getX(),
			location[1] - trans.getOrigin().getY(),
			location[2] - trans.getOrigin().getZ()));
	}
}


void MeshActor::translate(Vector3 offset, bool bRelative)
{
	Vector3 location;
	btTransform trans;

	Actor::translate(offset, bRelative);
	
	if (mPhysBody)
	{
		location = mNode->getPosition();
		mPhysBody->getMotionState()->getWorldTransform(trans);
		mPhysBody->translate(btVector3(
			location[0] - trans.getOrigin().getX(),
			location[1] - trans.getOrigin().getY(),
			location[2] - trans.getOrigin().getZ()));
	}
}


void MeshActor::rotate(Vector3 rotator)
{
	Actor::rotate(rotator);
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
