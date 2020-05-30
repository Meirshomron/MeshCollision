#pragma once
#include "MovableGLM.h"
#include "mesh.h"
#include "lineMesh.h"
#include "texture.h"
#include <list>

class Shape : public MovableGLM
{
	LineMesh *lineMesh;
	Texture *tex;
	bool isCopy;
public:
	Mesh * mesh;
	Mesh * cubeMesh;
	Vertex * init_cube(float x, float y, float z, float width, float height, float depth);
	//bool checkCollision(Node* A, Node* B, glm::mat4 TRTB, glm::mat4 TRTA);
	//bool SolveEquision(Node* A, Node* B, glm::vec3 L, glm::mat4 TRTB, glm::mat4 TRTA);
	enum{triangles,lines};
	Shape(const Shape& shape);
	Shape(const std::string& fileName);
	Shape(const std::string& fileName, const unsigned maxFaces);
	Shape(const std::string& fileName,const std::string& textureFileName);
	Shape(const std::string& fileName, const std::string& textureFileName, const unsigned maxFaces);
	Shape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	Shape(LineVertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	Shape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, const std::string& textureFileName);
	void addMesh(Vertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices, const std::string& textureFileName);
	void addMesh(Vertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices);
	void addLineMesh(LineVertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices);
	void draw(int mode);
	void getPoints(std::list<glm::vec3> &pList);
	virtual ~Shape(void);
};

