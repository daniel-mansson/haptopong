#pragma once

class StepTimer
{
public:
	StepTimer(double timeStep) :
		m_timeStep(timeStep),
		m_accumulator(0.0)
	{
	}

	int update(double timeStep)
	{
		m_accumulator += timeStep;
		int count = (int)(m_accumulator / m_timeStep);
		m_accumulator -= count * m_timeStep;
		return count;
	}

	const double& getTimeStep() const { return m_timeStep; }

private:
	double m_timeStep;
	double m_accumulator;
};

