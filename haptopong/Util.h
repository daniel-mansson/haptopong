#pragma once

class Util
{
public:
	static btVector3 Vec(const chai3d::cVector3d& vec)
	{
		return btVector3((btScalar)vec(0), (btScalar)vec(1), (btScalar)vec(2));
	}

	static chai3d::cVector3d Vec(const btVector3& vec)
	{
		return chai3d::cVector3d((double)vec.x(), (double)vec.y(), (double)vec.z());
	}

	static float RandRange(float min, float max)
	{
		return min + (max - min) * ((float)rand() / (float)RAND_MAX);
	}
    
    static btCollisionShape* CollisionShape(chai3d::cMultiMesh* mesh)
    {
        // convert cshape to btCollisionShape
        btTriangleMesh* trimesh = new btTriangleMesh();
        
        chai3d::cVertexArrayPtr vertices = mesh->getMesh(0)->m_triangles->m_vertices;
        chai3d::cTriangleArrayPtr triangles = mesh->getMesh(0)->m_triangles;
        unsigned numTriangles = mesh->getMesh(0)->getNumTriangles();
        
        chai3d::cVector3d pos; pos.zero();
        chai3d::cMatrix3d rot; rot.identity();
        mesh->getMesh(0)->computeGlobalPositions(false, pos, rot);
        
        // convert every triangle
        for (unsigned i = 0; i < numTriangles; ++i)
        {
            unsigned int index0 = triangles->getVertexIndex0(i);
            unsigned int index1 = triangles->getVertexIndex1(i);
            unsigned int index2 = triangles->getVertexIndex2(i);
            
            trimesh->addTriangle(Util::Vec(vertices->getGlobalPos(index0)), Util::Vec(vertices->getGlobalPos(index1)), Util::Vec(vertices->getGlobalPos(index2)));
            //trimesh->addTriangle(Util::Vec(vertices->getLocalPos(index0)), Util::Vec(vertices->getLocalPos(index1)), Util::Vec(vertices->getLocalPos(index2)));
        }
        
        return new btConvexTriangleMeshShape(trimesh);
    }
};