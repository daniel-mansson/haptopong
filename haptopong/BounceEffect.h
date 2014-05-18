#pragma once
class BounceEffect
{
public:
	BounceEffect(chai3d::cWorld* world);
	~BounceEffect(void);

	void start(const btVector3& pos);
	void update(const double& timeStep);

	bool isDone() const;
	void stop();

private:
	chai3d::cShapeCylinder* m_shape;
	double m_time;
	double m_duration;
};

typedef std::shared_ptr<BounceEffect> BounceEffectPtr;