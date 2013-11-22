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

void MeshActor::tick(const FrameEvent& evt)
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


void MeshActor::setRotation(Vector3 newRot)
{
	if (mPhysBody)
	{
		btQuaternion quat;
		quat.setEuler(
			Degree(newRot[1]).valueRadians(),
			Degree(newRot[0]).valueRadians(),
			Degree(newRot[2]).valueRadians());
		mPhysTransform.setRotation(quat);
		mPhysBody->setWorldTransform(mPhysTransform);
	}
	else
	{
		Actor::setRotation(newRot);
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


void MeshActor::rotate(Vector3 rotator)
{
	if (mPhysBody)
	{
		btQuaternion quat(mPhysTransform.getRotation());
		quat.setEuler(
			Degree(rotator[1]).valueRadians(),
			Degree(rotator[0]).valueRadians(),
			Degree(rotator[2]).valueRadians());
		mPhysTransform.setRotation(quat);
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
void getMeshInformation(const Mesh* const mesh,
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
        SubMesh* submesh = mesh->getSubMesh(i);
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
        SubMesh* submesh = mesh->getSubMesh(i);
        VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
 
        if ((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
        {
            if(submesh->useSharedVertices)
            {
                added_shared = true;
                shared_offset = current_offset;
            }
 
            const VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(VES_POSITION);
            HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
            unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(HardwareBuffer::HBL_READ_ONLY)); 
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
 
        IndexData* index_data = submesh->indexData;
        size_t numTris = index_data->indexCount / 3;
        HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
 
        bool use32bitindexes = (ibuf->getType() == HardwareIndexBuffer::IT_32BIT);
        unsigned long* pLong = static_cast<unsigned long*>(ibuf->lock(HardwareBuffer::HBL_READ_ONLY));
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
	getCollisionMesh(true);
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
	Mesh* origin = mMesh->getMesh().get();

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
