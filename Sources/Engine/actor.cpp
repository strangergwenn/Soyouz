/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/actor.hpp"


/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

Actor::Actor(Game* g, String name)
	: mMesh(NULL)
{
	mGame = g;
	mNode = g->createGameNode(name);
}


Actor::Actor(Game* g, String name, String file)
{
	mGame = g;
	mNode = g->createGameNode(name);
	mMesh = g->createGameEntity(name + "_mesh", file);
	mNode->attachObject(mMesh);
	mMesh->setCastShadows(true);
}


Actor::Actor(Game* g, String name, String file, String material)
{
	mGame = g;
	mNode = g->createGameNode(name);
	mMesh = g->createGameEntity(name + "_mesh", file);
	mNode->attachObject(mMesh);
	mMesh->setCastShadows(true);
	setMaterial(material);
}


Actor::Actor(Game* g, String name, String file, String material, bool bCastShadows)
{
	mGame = g;
	mNode = g->createGameNode(name);
	mMesh = g->createGameEntity(name + "_mesh", file);
	mNode->attachObject(mMesh);
	mMesh->setCastShadows(bCastShadows);
	setMaterial(material);
}


Actor::~Actor()
{
}


/*----------------------------------------------
	Methods
----------------------------------------------*/

void Actor::attachObject(MovableObject* obj)
{
	mNode->attachObject(obj);
}


void Actor::attachActor(Actor* obj)
{
	SceneNode* target = obj->getNode();
	Log("Actor::attachActor " + target->getName() + " to " + mNode->getName());
	target->getParent()->removeChild(target);
	mNode->addChild(target);
}


void Actor::setLocation(Vector3 newPos)
{
	mNode->setPosition(newPos);
}


void Actor::translate(Vector3 offset, bool bRelative)
{
	mNode->translate(offset[0], offset[1], offset[2], bRelative ? Node::TS_LOCAL : Node::TS_WORLD);
}


void Actor::rotate(Vector3 rotator)
{
	mNode->pitch(Degree(rotator[0]));
	mNode->yaw(Degree(rotator[1]));
	mNode->roll(Degree(rotator[2]));
}


/*----------------------------------------------
	Materials
----------------------------------------------*/

void Actor::setMaterial(String name)
{
	mMesh->setMaterialName(name);
}


void Actor::setMaterialParam(String name, Real val)
{
	Pass* pPass = mMesh->getSubEntity(0)->getMaterial()->getTechnique("main")->getPass("main");
	pPass->getFragmentProgramParameters()->setNamedConstant(name, val);
}


void Actor::setMaterialParam(String name, Vector3 val)
{
	Pass* pPass = mMesh->getSubEntity(0)->getMaterial()->getTechnique("main")->getPass("main");
	pPass->getFragmentProgramParameters()->setNamedConstant(name, val);
}


void Actor::setMaterialParam(String name, Vector4 val)
{
	Pass* pPass = mMesh->getSubEntity(0)->getMaterial()->getTechnique("main")->getPass("main");
	pPass->getFragmentProgramParameters()->setNamedConstant(name, val);
}


/*----------------------------------------------
	Getters
----------------------------------------------*/

void Actor::Log(String text)
{
	mGame->Log(text);
}

Vector3 Actor::location()
{
	return mNode->getPosition();
}


Vector3 Actor::rotation()
{
	return mNode->getOrientation() * Ogre::Vector3(0, 1, 0);
}


Vector3 Actor::speed()
{
	return mSpeed;
}


SceneNode* Actor::getNode()
{
	return mNode;
}
