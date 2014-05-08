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
            
            //btVector3 localScaling(1.f, 1.f, 0.4f);
            //btVector3 localOffset(-0.f, -0.f, -0.038f);
            
            int i;
            for (i=0; i<wo.mTriCount; i++)
            {
                int index0 = wo.mIndices[i*3];
                int index1 = wo.mIndices[i*3+1];
                int index2 = wo.mIndices[i*3+2];
                
                btVector3 vertex0(wo.mVertices[index0*3], wo.mVertices[index0*3+1], wo.mVertices[index0*3+2]);
                btVector3 vertex1(wo.mVertices[index1*3], wo.mVertices[index1*3+1], wo.mVertices[index1*3+2]);
                btVector3 vertex2(wo.mVertices[index2*3], wo.mVertices[index2*3+1], wo.mVertices[index2*3+2]);
                
                //vertex0 *= localScaling;
                //vertex1 *= localScaling;
                //vertex2 *= localScaling;
                
                //vertex0 += localOffset;
                //vertex1 += localOffset;
                //vertex2 += localOffset;
                
                trimesh->addTriangle(vertex0,vertex1,vertex2, false);
            }
            
            /*
            btTriangleMesh* trimesh = new btTriangleMesh();
            for ( i=0;i<numTriangles;i++)
            {
                trimesh->addTriangle(triangles[i].vertex0,triangles[i].vertex1,triangles[i].vertex2);
            }
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
            
            //btBvhTriangleMeshShape *shape = new btBvhTriangleMeshShape(trimesh,true);
            btConvexTriangleMeshShape *shape = new btConvexTriangleMeshShape(trimesh);
            shape->setMargin(0.0);
            return shape;
            
            //return new btBvhTriangleMeshShape(trimesh, true);
            //return new btConvexTriangleMeshShape(trimesh);
            
            /*
            btConvexShape* tmpConvexShape = new btConvexTriangleMeshShape(trimesh);
            
            //create a hull approximation
            btShapeHull* hull = new btShapeHull(tmpConvexShape);
            btScalar margin = tmpConvexShape->getMargin();
            hull->buildHull(margin);
            tmpConvexShape->setUserPointer(hull);
            
            btConvexHullShape* convexShape = new btConvexHullShape();
            bool updateLocalAabb = false;
            
            for (i=0;i<hull->numVertices();i++)
            {
                convexShape->addPoint(hull->getVertexPointer()[i],updateLocalAabb);
            }
            convexShape->recalcLocalAabb();
            
            delete tmpConvexShape;
            delete hull;
            
            return convexShape;
            */
            /*
            bool useQuantization = true;
            btCollisionShape* concaveShape = new btBvhTriangleMeshShape(trimesh,useQuantization);
            
            return concaveShape;
            */
        }
        
        return nullptr;
    }
    
};
