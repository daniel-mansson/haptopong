#pragma once
#include "PlayerId.h"

class Score
{
public:
	Score(void);
	~Score(void);

	int getScore(PlayerId player) const { return m_score[player - 1]; }
	void addScore(PlayerId player, int amount) { m_score[player - 1] += amount; }

	void reset() { m_score[0] = m_score[1] = 0; }


private:

	int m_score[2];
};

