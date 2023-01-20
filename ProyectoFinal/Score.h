#pragma once

#include <iostream>
#include <fstream>

class Score
{
public:
	Score();

	float getScore();
	void setScore(float scoreNum);

	~Score();

private:
	float scoreNum;
};