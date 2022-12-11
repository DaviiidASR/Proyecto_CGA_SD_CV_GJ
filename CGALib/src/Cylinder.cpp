#include "Headers/Cylinder.h"

Cylinder::Cylinder(int slices, int stacks, float topRadius, float bottomRadius, float height) {
	this->slices = slices;
	this->stacks = stacks;
	float stackHeight = height / stacks;
	float radiusStep = (topRadius - bottomRadius) / stacks;
	int count = 0;

	vertexArray.resize((slices + 1) * (stacks + 1) + 2 * (slices + 1) + 2);
	index.resize(slices * stacks * 2 * 3 + 2 * slices * 3);

	for (int i = 0; i <= stacks; i++) {
		float y = -0.5f * height + i * stackHeight;
		float r = bottomRadius + i * radiusStep;
		float dTheta = float(2.0f * M_PI) / slices;
		float U = i / (float) stacks;
		for (int j = 0; j <= slices; j++) {
			float c = cos(j * dTheta);
			float s = sin(j * dTheta);
			float V = j / (float) slices;
			vertexArray[count++] = Vertex(glm::vec3(r * c, y, r * s), glm::vec2(U, V), glm::vec3(r * c, y, r * s));
		}
	}

	//top cap
	float y = 0.5f * height;
	float dTheta = float(2.0f * M_PI) / slices;

	for (int i = slices; i >= 0; i--) {
		float x = topRadius * cos(i * dTheta);
		float z = topRadius * sin(i * dTheta);
		float U = cos(i * dTheta) / 2.0 + 0.5;
		float V = sin(i * dTheta) / 2.0 + 0.5;
		vertexArray[count++] = Vertex(glm::vec3(x, y, z), glm::vec2(U, V), glm::vec3(0, 1, 0));
	}
	vertexArray[count++] = Vertex(glm::vec3(0, y, 0), glm::vec2(0.5, 0.5), glm::vec3(0, 1, 0));
	//bottom cap
	y = -y;

	for (int i = 0; i <= slices; i++) {
		float x = bottomRadius * cos(i * dTheta);
		float z = bottomRadius * sin(i * dTheta);
		float U = cos(i * dTheta) / 2.0 + 0.5;
		float V = sin(i * dTheta) / 2.0 + 0.5;
		vertexArray[count++] = Vertex(glm::vec3(x, y, z), glm::vec2(U, V), glm::vec3(0, -1, 0));
	}
	vertexArray[count++] = Vertex(glm::vec3(0, y, 0), glm::vec2(0.5, 0.5), glm::vec3(0, -1, 0));

	//fill indices array
	int ringVertexCount = slices + 1;
	int id = 0;
	for (int i = 0; i < stacks; i++) {
		for (int j = 0; j < slices; j++) {
			index[id++] = (i * ringVertexCount + j);
			index[id++] = ((i + 1) * ringVertexCount + j);
			index[id++] = ((i + 1) * ringVertexCount + j + 1);

			index[id++] = (i * ringVertexCount + j);
			index[id++] = ((i + 1) * ringVertexCount + j + 1);
			index[id++] = (i * ringVertexCount + j + 1);
		}
	}

	//top cap
	int baseIndex = (slices + 1) * (stacks + 1);
	int centerIndex = baseIndex + (slices + 1);

	for (int i = 0; i < slices; i++) {
		index[id++] = centerIndex;
		index[id++] = baseIndex + i;
		index[id++] = baseIndex + i + 1;
	}

	//bottom cap
	baseIndex = centerIndex + 1;
	centerIndex = baseIndex + (slices + 1);

	for (int i = 0; i < slices; i++) {
		index[id++] = centerIndex;
		index[id++] = baseIndex + i;
		index[id++] = baseIndex + i + 1;
	}
}

Cylinder::~Cylinder() {

}
