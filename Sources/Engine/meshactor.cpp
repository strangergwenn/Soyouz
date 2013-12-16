/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/meshactor.hpp"
#include "Engine/componentactor.hpp"

/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

MeshActor::MeshActor(Game* g, String name)
	: Actor(g, name)
{
	mPhysBody = NULL;
}


MeshActor::MeshActor(Game* g, String name, String file)
	: Actor(g, name)
{
	if (file.length() > 0)
	{
		setModel(file);
		setCastShadows(true);
	}
	mPhysBody = NULL;
}


MeshActor::MeshActor(Game* g, String name, String file, String material)
	: Actor(g, name)
{
	if (file.length() > 0)
	{
		setModel(file);
		setCastShadows(true);
		setMaterial(material);
	}
	mPhysBody = NULL;
}


MeshActor::MeshActor(Game* g, String name, String file, String material, bool bCastShadows)
	: Actor(g, name)
{
	if (file.length() > 0)
	{
		setModel(file);
		setCastShadows(bCastShadows);
		setMaterial(material);
	}
	mPhysBody = NULL;
}


MeshActor::MeshActor(Game* g, String name, String file, String material, bool bCastShadows, float mass)
	: Actor(g, name)
{
	if (file.length() > 0)
	{
		setModel(file);
		setCastShadows(bCastShadows);
		setMaterial(material);
		setMass(mass);
	}
}


MeshActor::~MeshActor()
{
	if (mPhysBody)
	{
		mGame->unregisterRigidBody(mPhysBody);
	}
}


/*----------------------------------------------
	Methods
----------------------------------------------*/

void MeshActor::tick(const Ogre::FrameEvent& evt)
{
	if (mPhysBody)
	{
		mPhysTransform = mPhysBody->getWorldTransform();
		btQuaternion rotation(mPhysTransform.getRotation());
		mNode->setOrientation(rotation.getW(), rotation.getX(), rotation.getY(), rotation.getZ());
		const btVector3 &origin = mPhysTransform.getOrigin();
		mNode->setPosition(origin.getX(), origin.getY(), origin.getZ());
	}
	Actor::tick(evt);
}


void MeshActor::setModel(Ogre::String file)
{
	mRootComponent->setModel(file);
	//~ prepareLoad(file);
	//~ mMesh = mGame->createGameEntity(mName + "_mesh", file);
	//~ mMesh->setCastShadows(true);
	//~ mNode->attachObject(mMesh);
}

void MeshActor::setCastShadows(bool bCastShadow)
{
	mRootComponent->setCastShadows(bCastShadow);
	//~ prepareLoad(file);
	//~ mMesh = mGame->createGameEntity(mName + "_mesh", file);
	//~ mMesh->setCastShadows(true);
	//~ mNode->attachObject(mMesh);
}

void MeshActor::setMass(float mass)
{
	mPhysMass = mass;
	//~ prepareLoad(file);
	//~ mMesh = mGame->createGameEntity(mName + "_mesh", file);
	//~ mMesh->setCastShadows(true);
	//~ mNode->attachObject(mMesh);
}

void MeshActor::prepareLoad(Ogre::String name)
{
	unsigned short src, dest;
	Ogre::MeshPtr pMesh = Ogre::MeshManager::getSingleton().load(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	if (!pMesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
	{
		pMesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
	}
}



void MeshActor::setLocation(Vector3 newPos)
{
	if (mPhysBody)
	{
		mPhysTransform.setOrigin(btVector3(
			newPos[0],
			newPos[1],
			newPos[2]));
		mPhysBody->setWorldTransform(mPhysTransform);
	}
	else
	{
		Actor::setLocation(newPos);
	}
}


void MeshActor::setRotation(Quaternion newRot)
{
	if (mPhysBody)
	{
		btQuaternion quat(newRot.x, newRot.y, newRot.z, newRot.w);
		mPhysTransform.setRotation(quat);
		mPhysBody->setWorldTransform(mPhysTransform);
	}
	else
	{
		Actor::setRotation(newRot);
	}
}

void MeshActor::setSpeed(Vector3 newSpeed)
{
	if (mPhysBody)
	{
		mPhysBody->setLinearVelocity(btVector3(newSpeed[0], newSpeed[1], newSpeed[2]));
	}
}


void MeshActor::translate(Vector3 offset, bool bRelative)
{
	if (mPhysBody)
	{
		btVector3 base = mPhysTransform.getOrigin();
		mPhysTransform.setOrigin(btVector3(
			base[0] + offset[0],
			base[1] + offset[1],
			base[2] + offset[2]));
		mPhysBody->setWorldTransform(mPhysTransform);
	}
	else
	{
		Actor::translate(offset, bRelative);
	}
}


void MeshActor::rotate(Quaternion rotator)
{
	if (mPhysBody)
	{
		btQuaternion quat(rotator.x, rotator.y, rotator.z, rotator.w);
		mPhysTransform.setRotation( mPhysTransform.getRotation() * quat);
		mPhysBody->getMotionState()->setWorldTransform(mPhysTransform);
	}
	else
	{
		Actor::rotate(rotator);
	}
}


void MeshActor::applyForce(Vector3 force, Vector3 location)
{
	if (mPhysBody)
	{
		mPhysBody->setActivationState(DISABLE_DEACTIVATION);
		btVector3 f = btVector3(force[0], force[1], force[2]);
		btVector3 p = btVector3(location[0], location[1], location[2]);
		mPhysBody->applyForce(f, p);
	}
}


void MeshActor::applyLocalForce(Vector3 force, Vector3 location)
{
	if (mPhysBody)
	{
		mPhysBody->setActivationState(DISABLE_DEACTIVATION);
		btVector3 f = btVector3(force[0], force[1], force[2]);
		btVector3 p = btVector3(location[0], location[1], location[2]);
		f = mPhysTransform.getBasis() * f;
		p = mPhysTransform.getBasis() * p;
		mPhysBody->applyForce(f, p);
	}
}


void MeshActor::clearForces()
{
	if (mPhysBody)
	{
		mPhysBody->clearForces();
	}
}

void MeshActor::attachComponent(ComponentActor* component) {
	mComponentActors.push_back(component);
	generateCollisions();
}

/*----------------------------------------------
	Getters
----------------------------------------------*/

Vector3 MeshActor::getSpeed(void)
{
	if (mPhysBody)
	{
		btVector3 speed = mPhysBody->getLinearVelocity();
		return Vector3(speed[0], speed[1], speed[2]);
	}
	else
	{
		return Vector3(0, 0, 0);
	}
}


Vector3 MeshActor::getLocalSpeed(void)
{
	if (mPhysBody)
	{
		btVector3 speed = mPhysBody->getLinearVelocity() * mPhysTransform.getBasis();
		return Vector3(speed[0], speed[1], speed[2]);
	}
	else
	{
		return Vector3(0, 0, 0);
	}
}


Vector3 MeshActor::getAngularSpeed(void)
{
	if (mPhysBody)
	{
		btVector3 angularSpeed = mPhysBody->getAngularVelocity();
		return Vector3(angularSpeed[0], angularSpeed[1], angularSpeed[2]);
	}
	else
	{
		return Vector3(0, 0, 0);
	}
}


Vector3 MeshActor::getLocalAngularSpeed(void)
{
	if (mPhysBody)
	{
		btVector3 angularSpeed = mPhysBody->getAngularVelocity() * mPhysTransform.getBasis();
		return Vector3(angularSpeed[0], angularSpeed[1], angularSpeed[2]);
	}
	else
	{
		return Vector3(0, 0, 0);
	}
}


Quaternion MeshActor::getRotation()
{
	if (mPhysBody)
	{
		btQuaternion rotation(mPhysTransform.getRotation());
		return Quaternion(rotation.getW(), rotation.getX(), rotation.getY(), rotation.getZ());
	}
	else
	{
		return Actor::rotation();
	}
}

Vector3 MeshActor::getLocation()
{
	if (mPhysBody)
	{
		const btVector3 &origin = mPhysTransform.getOrigin();
		return Vector3(origin.getX(), origin.getY(), origin.getZ());
	}
	else
	{
		return Actor::location();
	}
}


Vector3 MeshActor::getGlobalPosition(Vector3 position)
{
	return getRotation() * position + getLocation();
}


/*----------------------------------------------
	Collisions
----------------------------------------------*/

void MeshActor::addCollisionMesh(ComponentActor* component)
{
	btTransform transform;
	Quaternion rotation = component->getRotation();
	Vector3 location = component->getLocation();
	
	transform.setIdentity();
	transform.setOrigin(btVector3(
		location[0],
		location[1],
		location[2]));
	
	btQuaternion quat(rotation.x, rotation.y, rotation.z, rotation.w);
	transform.setRotation(quat);
	
	mPhysShape->addChildShape (transform , component->getCollisionMesh(false));
}

void MeshActor::generateCollisions()
{
	// Physics settings
	
	mPhysShape = new btCompoundShape(true);
		
	for (Ogre::list<ComponentActor*>::iterator it = mComponentActors.begin(); it != mComponentActors.end(); it++)
	{
		addCollisionMesh(*it);
	}
	
	mPhysTransform.setIdentity();
	mPhysTransform.setOrigin(btVector3(0, 0, 0));
	btVector3 localInertia(0,0,0);
	
	// Physics setup
	mPhysShape->calculateLocalInertia(mPhysMass, localInertia);
	mPhysMotionState = new btDefaultMotionState(mPhysTransform);
	btRigidBody::btRigidBodyConstructionInfo rbConstruct(
		mPhysMass,
		mPhysMotionState,
		mPhysShape,
		localInertia);

	// End
	mPhysBody = new btRigidBody(rbConstruct);
	mGame->registerRigidBody(mPhysBody);
}

/*----------------------------------------------
	Materials
----------------------------------------------*/

void MeshActor::setMaterial(String name)
{
	mRootComponent->setMaterial(name);
}


void MeshActor::setMaterialParam(int index, Real val)
{
	mRootComponent->setMaterialParam(index, val);
}


void MeshActor::setMaterialParam(int index, Vector3 val)
{
	mRootComponent->setMaterialParam(index, val);
}


void MeshActor::setMaterialParam(int index, Vector4 val)
{
	mRootComponent->setMaterialParam(index, val);
}
