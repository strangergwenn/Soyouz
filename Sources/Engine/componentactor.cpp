/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Frédéric Bertolus
**/

#include "Engine/componentactor.hpp"


/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

ComponentActor::ComponentActor(Game* g, String name)
	: Actor(g, name)
{
	mMesh = NULL;
}


ComponentActor::ComponentActor(Game* g, String name, String file)
	: Actor(g, name)
{
	mMesh = NULL;
	if (file.length() > 0)
	{
		setModel(file);
	}
}


ComponentActor::ComponentActor(Game* g, String name, String file, String material)
	: Actor(g, name)
{
	mMesh = NULL;
	if (file.length() > 0)
	{
		setModel(file);
		setMaterial(material);
	}
}


ComponentActor::ComponentActor(Game* g, String name, String file, String material, bool bCastShadows)
	: Actor(g, name)
{
	mMesh = NULL;
	if (file.length() > 0)
	{
		setModel(file);
		setMaterial(material);
		mMesh->setCastShadows(bCastShadows);
	}
}

ComponentActor::~ComponentActor()
{
}


/*----------------------------------------------
	Methods
----------------------------------------------*/

void ComponentActor::tick(const Ogre::FrameEvent& evt)
{
	Actor::tick(evt);
}


void ComponentActor::setModel(Ogre::String file)
{
	prepareLoad(file);
	mMesh = mGame->createGameEntity(mName + "_mesh", file);
	mMesh->setCastShadows(true);
	mNode->attachObject(mMesh);
}

void ComponentActor::setCastShadows(bool bCastShadow)
{
	mCastShadow = bCastShadow;
	if(mMesh) {
		mMesh->setCastShadows(true);
	}
}

void ComponentActor::prepareLoad(Ogre::String name)
{
	unsigned short src, dest;
	Ogre::MeshPtr pMesh = Ogre::MeshManager::getSingleton().load(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	if (!pMesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
	{
		pMesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
	}
}



void ComponentActor::setLocation(Vector3 newPos)
{
	Actor::setLocation(newPos);
}


void ComponentActor::setRotation(Quaternion newRot)
{
	Actor::setRotation(newRot);
}

void ComponentActor::translate(Vector3 offset, bool bRelative)
{
	Actor::translate(offset, bRelative);
}


void ComponentActor::rotate(Quaternion rotator)
{
	Actor::rotate(rotator);
}


/*----------------------------------------------
	Getters
----------------------------------------------*/


Quaternion ComponentActor::getRotation()
{
	return Actor::rotation();
}

Vector3 ComponentActor::getLocation()
{
	return Actor::location();
}


Vector3 ComponentActor::getGlobalPosition(Vector3 position)
{
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

btCollisionShape* ComponentActor::getCollisionMesh(bool bOptimize)
{
	if(!mMesh) {
		return NULL;
	}
	
	
	Vector3* vertices;
	size_t vCount, iCount;
	unsigned long* indices;
	Ogre::Mesh* origin = mMesh->getMesh().get();
	btCollisionShape* shape;

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
	btConvexTriangleMeshShape* trishape = new btConvexTriangleMeshShape(trimesh, true);
	
	// Collision hull generation
	if (bOptimize)
	{
		btShapeHull* hull = new btShapeHull(trishape);
		btScalar margin = trishape->getMargin();
		hull->buildHull(margin);
		shape = new btConvexHullShape((btScalar*)hull->getVertexPointer(), hull->numVertices());

		delete trishape;
		delete hull;
	}
	else
	{
		shape = trishape;
	}
	
	return shape;
}


/*----------------------------------------------
	Materials
----------------------------------------------*/

void ComponentActor::setMaterial(String name)
{
	mMaterialName = name;
	mMesh->setMaterialName(mMaterialName);
}


void ComponentActor::setMaterialParam(int index, Real val)
{
	int numSubEnt = mMesh->getNumSubEntities();
	for (int i = 0; i < numSubEnt; i++)
	{
		mMesh->getSubEntity(i)->setCustomParameter(index, Vector4(val, val, val, val));
	}
}


void ComponentActor::setMaterialParam(int index, Vector3 val)
{
	int numSubEnt = mMesh->getNumSubEntities();
	for (int i = 0; i < numSubEnt; i++)
	{
		mMesh->getSubEntity(i)->setCustomParameter(index, Vector4(val[0], val[1], val[2], 0));
	}
}


void ComponentActor::setMaterialParam(int index, Vector4 val)
{
	int numSubEnt = mMesh->getNumSubEntities();
	for (int i = 0; i < numSubEnt; i++)
	{
		mMesh->getSubEntity(i)->setCustomParameter(index, val);
	}
}
