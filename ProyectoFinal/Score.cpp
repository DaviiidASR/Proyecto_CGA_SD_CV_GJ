#include "Score.h"
#include <fstream>
#include <iostream>
#include <string>

Score::Score()
{
}

float Score::getScore()
{
	std::string text;
	float score = 0.0f;

	// Read from the text file
	std::ifstream file("config/score.txt");

	// Use a while loop together with the getline() function to read the file line by line
	int i = 0;
	while (getline(file, text)) {
		// Output the text from the file
		std::cout << "Best Score: " << text << std::endl;
		if (text.find("#") != std::string::npos) {

		}
		else {
			score = std::stof(text);
		}
	}
	return score;
}

void Score::setScore(float score)
{
	std::fstream file;
	file.open("config/score.txt", std::ios::out);
	if (!file) {
		std::cout << "File not created!";
	}
	else {
		std::cout << "File created successfully!";
		file << score;
		file.close();
	}
}

Score::~Score()
{
}

