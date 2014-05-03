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
};