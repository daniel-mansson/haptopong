#pragma once

class RacketProperties
{
public:
	RacketProperties(void);
	~RacketProperties(void);
		
	double getFriction() const { return m_friction; }
	double getElasticity() const { return m_elasticity; }
	double getRestitution() const { return m_restitution; }
	double getWeight() const { return m_weight; }
	
	void setFriction(const double& friction) { m_friction = friction; }
	void setElasticity(const double& elasticity) { m_elasticity = elasticity; }
	void setRestitution(const double& restitution) { m_restitution = restitution; }
	void setWeight(const double& weight) { m_weight = weight; }

private:
	double m_elasticity;
	double m_weight;
	double m_friction;
	double m_restitution;
};

