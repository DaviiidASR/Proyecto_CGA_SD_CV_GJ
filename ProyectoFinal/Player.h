#pragma once
#include <string>
#include "Headers/Model.h"

class Player:public Model
{
public:
	Player();
	
	void setModel(const std::string name);
	float getModelScale() { return modelScale; }
	std::string getPath() { return path; }
	float getModelAngle() { return modelAngle; }
	glm::vec3 getVectorRot() { return vectorRot; }
	float getScaleCol() { return scaleCol; }
	glm::vec3 getOffsetC() { return offsetC; }
	glm::vec3 getOffsetE() { return offsetE; }
	float getAngleRotCol() { return angleRotCol; }
	glm::vec3 getVectorRotCol() { return vectorRotCol; }

	~Player();
private:

	float modelScale;
	std::string path;
	float modelAngle;
	glm::vec3 vectorRot;
	float scaleCol;
	glm::vec3 offsetC;
	glm::vec3 offsetE;
	float angleRotCol;
	glm::vec3 vectorRotCol;
};

