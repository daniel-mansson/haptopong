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
        
        //btConvexTriangleMeshShape* convexShape = new btConvexTriangleMeshShape(trimesh, true);
        
        bool useQuantization = false;
        btCollisionShape* shape = new btBvhTriangleMeshShape(trimesh, useQuantization);
        
        return shape;
    }
    
    static btCollisionShape* LoadCollisionShape(const char* filename)
    {
        ConvexDecomposition::WavefrontObj wo;
        
        unsigned int tcount = wo.loadObj(filename);
        
        if (tcount)
        {
            btTriangleMesh* trimesh = new btTriangleMesh();

            for (int i=0; i<wo.mTriCount; i++)
            {
                int index0 = wo.mIndices[i*3];
                int index1 = wo.mIndices[i*3+1];
                int index2 = wo.mIndices[i*3+2];
                
                btVector3 vertex0(wo.mVertices[index0*3], wo.mVertices[index0*3+1], wo.mVertices[index0*3+2]);
                btVector3 vertex1(wo.mVertices[index1*3], wo.mVertices[index1*3+1], wo.mVertices[index1*3+2]);
                btVector3 vertex2(wo.mVertices[index2*3], wo.mVertices[index2*3+1], wo.mVertices[index2*3+2]);
                
                trimesh->addTriangle(vertex0,vertex1,vertex2, false);
            }
            
            /*
            btCollisionShape* shape = 0;
            if (movingDynamic)
            {
                shape = new btConvexTriangleMeshShape(trimesh);
            }
            else
            {
                //static, non-moving world environment geometry
                bool useQuantization = true;
                shape  = new btBvhTriangleMeshShape(trimesh,useQuantization);
            }
            */
            
            btConvexTriangleMeshShape *shape = new btConvexTriangleMeshShape(trimesh);
            shape->setMargin(0.0);
            return shape;
        }
        
        return nullptr;
    }
    
};
