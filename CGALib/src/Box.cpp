#include "Headers/Box.h"

Box::Box()
{
	vertexArray.push_back(Vertex(glm::vec3(0.5, 0.5, 0.5), glm::vec2(1.0f, 1.0f), glm::vec3(0.0, 0.0, 1.0)));
	vertexArray.push_back(Vertex(glm::vec3(-0.5, 0.5, 0.5), glm::vec2(0.0f, 1.0f), glm::vec3(0.0, 0.0, 1.0)));
	vertexArray.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.5), glm::vec2(0.0f, 0.0f), glm::vec3(0.0, 0.0, 1.0)));
	vertexArray.push_back(Vertex(glm::vec3(0.5, -0.5, 0.5), glm::vec2(1.0f, 0.0f), glm::vec3(0.0, 0.0, 1.0)));

	vertexArray.push_back(Vertex(glm::vec3(0.5, 0.5, 0.5), glm::vec2(0.0f, 1.0f), glm::vec3(1.0, 0.0, 0.0)));
	vertexArray.push_back(Vertex(glm::vec3(0.5, -0.5, 0.5), glm::vec2(0.0f, 0.0f), glm::vec3(1.0, 0.0, 0.0)));
	vertexArray.push_back(Vertex(glm::vec3(0.5, -0.5, -0.5), glm::vec2(1.0f, 0.0f), glm::vec3(1.0, 0.0, 0.0)));
	vertexArray.push_back(Vertex(glm::vec3(0.5, 0.5, -0.5), glm::vec2(1.0f, 1.0f), glm::vec3(1.0, 0.0, 0.0)));

	vertexArray.push_back(Vertex(glm::vec3(0.5, 0.5, -0.5), glm::vec2(0.0f, 1.0f), glm::vec3(0.0, 0.0, -1.0)));
	vertexArray.push_back(Vertex(glm::vec3(0.5, -0.5, -0.5), glm::vec2(0.0f, 0.0f), glm::vec3(0.0, 0.0, -1.0)));
	vertexArray.push_back(Vertex(glm::vec3(-0.5, -0.5, -0.5), glm::vec2(1.0f, 0.0f), glm::vec3(0.0, 0.0, -1.0)));
	vertexArray.push_back(Vertex(glm::vec3(-0.5, 0.5, -0.5), glm::vec2(1.0f, 1.0f), glm::vec3(0.0, 0.0, -1.0)));

	vertexArray.push_back(Vertex(glm::vec3(-0.5, 0.5, 0.5), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0, 0.0, 0.0)));
	vertexArray.push_back(Vertex(glm::vec3(-0.5, 0.5, -0.5), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0, 0.0, 0.0)));
	vertexArray.push_back(Vertex(glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0, 0.0, 0.0)));
	vertexArray.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.5), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0, 0.0, 0.0)));

	vertexArray.push_back(Vertex(glm::vec3(0.5, 0.5, 0.5), glm::vec2(1.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0)));
	vertexArray.push_back(Vertex(glm::vec3(0.5, 0.5, -0.5), glm::vec2(1.0f, 1.0f), glm::vec3(0.0, 1.0, 0.0)));
	vertexArray.push_back(Vertex(glm::vec3(-0.5, 0.5, -0.5), glm::vec2(0.0f, 1.0f), glm::vec3(0.0, 1.0, 0.0)));
	vertexArray.push_back(Vertex(glm::vec3(-0.5, 0.5, 0.5), glm::vec2(0.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0)));

	vertexArray.push_back(Vertex(glm::vec3(0.5, -0.5, 0.5), glm::vec2(1.0f, 1.0f), glm::vec3(0.0, -1.0, 0.0)));
	vertexArray.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.5), glm::vec2(0.0f, 1.0f), glm::vec3(0.0, -1.0, 0.0)));
	vertexArray.push_back(Vertex(glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0.0f, 0.0f), glm::vec3(0.0, -1.0, 0.0)));
	vertexArray.push_back(Vertex(glm::vec3(0.5, -0.5, -0.5), glm::vec2(1.0f, 0.0f), glm::vec3(0.0, -1.0, 0.0)));

	GLuint indexArray[36] = { 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23 };
	index.insert(index.begin(), indexArray, indexArray + sizeof(indexArray) / sizeof(GLuint));

	aabb = AABB(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
}

Box::~Box() {
}
