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
	if (file.length() > 0)
	{
		mMesh = g->createGameEntity(name + "_mesh", file);
		mMesh->setCastShadows(true);
		mNode->attachObject(mMesh);
	}
	mPhysBody = NULL;
}


MeshActor::MeshActor(Game* g, String name, String file, String material)
	: Actor(g, name)
{
	if (file.length() > 0)
	{
		mMesh = g->createGameEntity(name + "_mesh", file);
		mMesh->setCastShadows(true);
		mNode->attachObject(mMesh);
		setMaterial(material);
	}
	mPhysBody = NULL;
}


MeshActor::MeshActor(Game* g, String name, String file, String material, bool bCastShadows)
	: Actor(g, name)
{
	if (file.length() > 0)
	{
		mMesh = g->createGameEntity(name + "_mesh", file);
		mMesh->setCastShadows(bCastShadows);
		mNode->attachObject(mMesh);
		setMaterial(material);
	}
	mPhysBody = NULL;
}


MeshActor::MeshActor(Game* g, String name, String file, String material, bool bCastShadows, float mass)
	: Actor(g, name)
{
	mMesh = g->createGameEntity(name + "_mesh", file);
	mMesh->setCastShadows(bCastShadows);
	mNode->attachObject(mMesh);
	setMaterial(material);
	generateCollisions(mass);
}


MeshActor::~MeshActor()
{
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
	mPhysBody->setActivationState(DISABLE_DEACTIVATION);
	btVector3 f = btVector3(force[0], force[1], force[2]);
	btVector3 p = btVector3(location[0], location[1], location[2]);
	mPhysBody->applyForce(f, p);
}

void MeshActor::applyLocalForce(Vector3 force, Vector3 location)
{
	mPhysBody->setActivationState(DISABLE_DEACTIVATION);
	btVector3 f = btVector3(force[0], force[1], force[2]);
	btVector3 p = btVector3(location[0], location[1], location[2]);
	f = mPhysTransform.getBasis() * f;
	p = mPhysTransform.getBasis() * p;
	mPhysBody->applyForce(f, p);
}

void MeshActor::clearForces()
{
	mPhysBody->clearForces();
}


Vector3 MeshActor::getSpeed(void)
{
	btVector3 speed = mPhysBody->getLinearVelocity();
	return Vector3(speed[0], speed[1], speed[2]);
}

Vector3 MeshActor::getLocalSpeed(void)
{
	btVector3 speed = mPhysBody->getLinearVelocity() * mPhysTransform.getBasis();
	return Vector3(speed[0], speed[1], speed[2]);
}

Vector3 MeshActor::getAngularSpeed(void)
{
	btVector3 angularSpeed = mPhysBody->getAngularVelocity();
	return Vector3(angularSpeed[0], angularSpeed[1], angularSpeed[2]);
}

Vector3 MeshActor::getLocalAngularSpeed(void)
{
	btVector3 angularSpeed = mPhysBody->getAngularVelocity() * mPhysTransform.getBasis();
	return Vector3(angularSpeed[0], angularSpeed[1], angularSpeed[2]);
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

Vector3 MeshActor::getGlobalPosition(Vector3 position) {
	return getRotation() * position + getLocation();
}

/*----------------------------------------------
	Collisions
----------------------------------------------*/

/**
 * @brief Get mesh triangles
 * Thanks to the OGRE community (jacmoe)
 * http://www.ogre3d.org/tikiwiki/tiki-index.php?page=RetrieveVertexData
 * @param mesh			OGRE mesh data
 * @param vertex_count	Output vertices count
 * @param vertices		Output vertices data
 * @param index_count	Output index count
 * @param indices		Output index data
 * @param position		Mesh position offset
 * @param orient		Mesh orientation offset
 * @param scale			Mesh scale offset
 **/
void getMeshInformation(const Ogre::Mesh* const mesh,
                        size_t &vertex_count,
                        Vector3* &vertices,
                        size_t &index_count,
                        unsigned long* &indices,
                        const Vector3 &position,
                        const Quaternion &orient,
                        const Vector3 &scale)
{
    bool added_shared = false;
    size_t current_offset = 0;
    size_t shared_offset = 0;
    size_t next_offset = 0;
    size_t index_offset = 0;
 
    vertex_count = index_count = 0;
 
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);
        if(submesh->useSharedVertices)
        {
            if(!added_shared )
            {
                vertex_count += mesh->sharedVertexData->vertexCount;
                added_shared = true;
            }
        }
        else
        {
            vertex_count += submesh->vertexData->vertexCount;
        }
        index_count += submesh->indexData->indexCount;
    }
 
    vertices = new Vector3[vertex_count];
    indices = new unsigned long[index_count];
    added_shared = false;
 
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);
        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
 
        if ((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
        {
            if(submesh->useSharedVertices)
            {
                added_shared = true;
                shared_offset = current_offset;
            }
 
            const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
            Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
            unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY)); 
            float* pReal;
 
            for(size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
            {
                posElem->baseVertexPointerToElement(vertex, &pReal);
                Vector3 pt(pReal[0], pReal[1], pReal[2]);
                vertices[current_offset + j] = (orient * (pt * scale)) + position;
            }
 
            vbuf->unlock();
            next_offset += vertex_data->vertexCount;
        }
 
        Ogre::IndexData* index_data = submesh->indexData;
        size_t numTris = index_data->indexCount / 3;
        Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
 
        bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
        unsigned long* pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);
        size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;
 
        if (use32bitindexes)
        {
            for (size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
            }
        }
        else
        {
            for (size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = static_cast<unsigned long>(pShort[k]) +
                                          static_cast<unsigned long>(offset);
            }
        }
 
        ibuf->unlock();
        current_offset = next_offset;
    }
}


void MeshActor::generateCollisions(float mass)
{
	// Physics settings
	getCollisionMesh(false);
	mPhysTransform.setIdentity();
	mPhysTransform.setOrigin(btVector3(0, 0, 0));
	btVector3 localInertia(0,0,0);
	mPhysMass = mass;
	
	// Physics setup
	mPhysShape->calculateLocalInertia(mass, localInertia);
	mPhysMotionState = new btDefaultMotionState(mPhysTransform);
	btRigidBody::btRigidBodyConstructionInfo rbConstruct(
		mass,
		mPhysMotionState,
		mPhysShape,
		localInertia);

	// End
	mPhysBody = new btRigidBody(rbConstruct);
	mGame->addRigidBody(mPhysBody);
}


void MeshActor::getCollisionMesh(bool bOptimize)
{
	Vector3* vertices;
	size_t vCount, iCount;
	unsigned long* indices;
	Ogre::Mesh* origin = mMesh->getMesh().get();

	// Trimesh preparation
	btTriangleMesh* trimesh = new btTriangleMesh();
	getMeshInformation(origin, vCount, vertices, iCount, indices,
		Vector3::ZERO, mNode->getOrientation(), mNode->getScale());

	// Triangle copy
    btVector3 vertexPos[3];
	for (size_t n = 0; n < iCount / 3; n++)
	{
         for (unsigned int i = 0; i < 3; ++i)
         {
			const Vector3 &vec = vertices[*indices];
			vertexPos[i][0] = vec.x;
			vertexPos[i][1] = vec.y;
			vertexPos[i][2] = vec.z;
			*indices++;
         }
         trimesh->addTriangle(vertexPos[0], vertexPos[1], vertexPos[2]);
	}
	gameLog("getCollisionMesh : t " + StringConverter::toString(trimesh->getNumTriangles()) + " tris");
	btConvexTriangleMeshShape* trishape = new btConvexTriangleMeshShape(trimesh, true);
	
	// Collision hull generation
	if (bOptimize)
	{
		btShapeHull* hull = new btShapeHull(trishape);
		btScalar margin = trishape->getMargin();
		hull->buildHull(margin);
		mPhysShape = new btConvexHullShape((btScalar*)hull->getVertexPointer(), hull->numVertices());

		gameLog("getCollisionMesh : h " + StringConverter::toString(hull->numTriangles()) + " tris");
		delete trishape;
		delete hull;
	}
	else
	{
		mPhysShape = trishape;
	}
}


/*----------------------------------------------
	Materials
----------------------------------------------*/

void MeshActor::setMaterial(String name)
{
	mMaterialName = name;
	mMesh->setMaterialName(mMaterialName);
}


void MeshActor::setMaterialParam(int index, Real val)
{
	int numSubEnt = mMesh->getNumSubEntities();
	for (int i = 0; i < numSubEnt; i++)
	{
		mMesh->getSubEntity(i)->setCustomParameter(index, Vector4(val, val, val, val));
	}
}


void MeshActor::setMaterialParam(int index, Vector3 val)
{
	int numSubEnt = mMesh->getNumSubEntities();
	for (int i = 0; i < numSubEnt; i++)
	{
		mMesh->getSubEntity(i)->setCustomParameter(index, Vector4(val[0], val[1], val[2], 0));
	}
}


void MeshActor::setMaterialParam(int index, Vector4 val)
{
	int numSubEnt = mMesh->getNumSubEntities();
	for (int i = 0; i < numSubEnt; i++)
	{
		mMesh->getSubEntity(i)->setCustomParameter(index, val);
	}
}
