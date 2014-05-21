#include "pch.h"
#include "Util.h"

btCollisionShape* Util::LoadCollisionShape(const char* filename)
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

		
		btCollisionShape* shape = 0;
		/*if (false)
		{
		shape = new btConvexTriangleMeshShape(trimesh);
		}
		else
		{*/
		//static, non-moving world environment geometry
		bool useQuantization = true;
		shape  = new btBvhTriangleMeshShape(trimesh,useQuantization);
		//}
		
	
		//btConvexTriangleMeshShape *shape = new btConvexTriangleMeshShape(trimesh);
		shape->setMargin((btScalar)0.);

	//	std::cout<<"Shape: " << shape->isConvex() << "   "<< shape->isCompound() << "   "<< shape->isConcave() << "   "<< shape->isInfinite()<<"\n";

		return shape;
	}

	return nullptr;
}