/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef _GEOMETRY_H
#define _GEOMETRY_H


/*----------------------------------------------
	Geometry utility
----------------------------------------------*/

class GeomUtils
{

public:
	
	/**
	 * @brief Create a sphere entity
	 * @param strName				Mesh name
	 * @param radius				Mesh radius
	 * @param nRings				Mesh ring count
	 * @param nSegments				Mesh segment count
	 * @param bNormals				Use normals
	 * @param bTexCoords			Use texture coordinates
	 **/
	static void createSphere(const Ogre::String& strName,
		float radius, int nRings, int nSegments, bool bNormals, bool bTexCoords);
	
	/**
	 * @brief Create a sphere object
	 * @param vertexData			Destination vertex data
	 * @param indexData				Destination index data
	 * @param radius				Mesh radius
	 * @param nRings				Mesh ring count
	 * @param nSegments				Mesh segment count
	 * @param bNormals				Use normals
	 * @param bTexCoords			Use texture coordinates
	 **/
	static void createSphere(Ogre::VertexData*& vertexData, Ogre::IndexData*& indexData,
		float radius, int nRings, int nSegments, bool bNormals, bool bTexCoords);
	
	/**
	 * @brief Create a cone entity
	 * @param strName				Mesh name
	 * @param radius				Mesh radius
	 * @param height				Mesh height
	 * @param nVerticesInBase		Mesh segment count
	 **/
	static void createCone(const Ogre::String& strName, float radius, float height, int nVerticesInBase);
	
	/**
	 * @brief Create a cone object
	 * @param vertexData			Destination vertex data
	 * @param indexData				Destination index data
	 * @param radius				Mesh radius
	 * @param height				Mesh height
	 * @param nVerticesInBase		Mesh segment count
	 **/
	static void createCone(Ogre::VertexData*& vertexData, Ogre::IndexData*& indexData,
		float radius, float height, int nVerticesInBase);
	
	/**
	 * @brief Create a quad object
	 * @param vertexData			Destination vertex data
	 **/
	static void createQuad(Ogre::VertexData*& vertexData);

};


#endif
