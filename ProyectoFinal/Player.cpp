#include "Player.h"
#include <fstream>
#include <iostream>
#include <string>


Player::Player()
{
}

void Player::setModel(const std::string name)
{
	std::string text;
	std::string data[17];

	// Read from the text file
	std::ifstream file("config/" + name + ".txt");

	// Use a while loop together with the getline() function to read the file line by line
	int i = 0;
	while (getline(file, text)) {
		// Output the text from the file
		std::cout << text<<std::endl;
		if (text.find("#") != std::string::npos) {

		}
		else {
			data[i] = text;
			i++;
		}
	}
	modelScale = std::stof(data[0]);
	path = data[1];
	modelAngle = std::stof(data[2]);
	vectorRot = glm::vec3(std::stof(data[3]), std::stof(data[4]), std::stof(data[5]));
	scaleCol = std::stof(data[6]);
	offsetC = glm::vec3(std::stof(data[7]), std::stof(data[8]), std::stof(data[9]));
	offsetE = glm::vec3(std::stof(data[10]), std::stof(data[11]), std::stof(data[12]));
	angleRotCol = std::stof(data[13]);
	vectorRotCol = glm::vec3(std::stof(data[14]), std::stof(data[15]), std::stof(data[16]));
	// Close the file
	file.close();
}

Player::~Player()
{
}
