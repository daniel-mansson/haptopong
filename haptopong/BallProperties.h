#pragma once
class BallProperties
{
public:
	BallProperties(void);
	~BallProperties(void);
	
	double getFriction() const { return m_friction; }
	double getElasticity() const { return m_elasticity; }
	double getRestitution() const { return m_restitution; }
	double getWeight() const { return m_weight; }
	double getRadius() const { return m_radius; }
	double getArea() const { return m_area; }
	double getVolume() const { return m_volume; }
	double getGravity() const { return m_gravity; }
	double getLinDamping() const { return m_linDamping; }
	double getAngDamping() const { return m_angDamping; }
	
	void setFriction(const double& friction) { m_friction = friction; }
	void setElasticity(const double& elasticity) { m_elasticity = elasticity; }
	void setRestitution(const double& restitution) { m_restitution = restitution; }
	void setWeight(const double& weight) { m_weight = weight; }
	void setRadius(const double& radius) { m_radius = radius; }
	void setArea(const double& area) { m_area = area; }
	void setVolume(const double& volume) { m_volume = volume; }
	void setGravity(const double& gravity) { m_gravity = gravity; }
	void setLinDamping(const double& linDamping) { m_linDamping = linDamping; }
	void setAngDamping(const double& angDamping) { m_angDamping = angDamping; }

private:
	double m_friction;
	double m_elasticity;
	double m_restitution;
	double m_weight;
	double m_radius;
	double m_area;
	double m_volume;
	double m_gravity;
    double m_linDamping;
    double m_angDamping;
};

