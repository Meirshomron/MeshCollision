#include <iostream>

#include "shape.h"


Vertex * Shape::init_cube(float x, float y, float z, float width, float height, float depth)
{
	Vertex* myVertex = (Vertex*)malloc(sizeof(Vertex) * 24 * 44);

	myVertex[0] = Vertex(glm::vec3(-width + x, -height + y, -depth + z), glm::vec2(1, 0), glm::vec3(0, 0, -1), glm::vec3(1, 1, 0));			//yellow
	myVertex[1] = Vertex(glm::vec3(-width + x, height + y, -depth + z), glm::vec2(0, 0), glm::vec3(0, 0, -1), glm::vec3(1, 1, 0));
	myVertex[2] = Vertex(glm::vec3(width + x, height + y, -depth + z), glm::vec2(0, 1), glm::vec3(0, 0, -1), glm::vec3(1, 1, 0));
	myVertex[3] = Vertex(glm::vec3(width + x, -height + y, -depth + z), glm::vec2(1, 1), glm::vec3(0, 0, -1), glm::vec3(1, 1, 0));

	myVertex[4] = Vertex(glm::vec3(-width + x, -height + y, depth + z), glm::vec2(1, 0), glm::vec3(0, 0, 1), glm::vec3(1, 1, 1));			//white
	myVertex[5] = Vertex(glm::vec3(-width + x, height + y, depth + z), glm::vec2(0, 0), glm::vec3(0, 0, 1), glm::vec3(1, 1, 1));
	myVertex[6] = Vertex(glm::vec3(width + x, height + y, depth + z), glm::vec2(0, 1), glm::vec3(0, 0, 1), glm::vec3(1, 1, 1));
	myVertex[7] = Vertex(glm::vec3(width + x, -height + y, depth + z), glm::vec2(1, 1), glm::vec3(0, 0, 1), glm::vec3(1, 1, 1));

	myVertex[8] = Vertex(glm::vec3(-width + x, -height + y, -depth + z), glm::vec2(0, 1), glm::vec3(0, -1, 0), glm::vec3(1, 0.5, 0));		//orange
	myVertex[9] = Vertex(glm::vec3(-width + x, -height + y, depth + z), glm::vec2(1, 1), glm::vec3(0, -1, 0), glm::vec3(1, 0.5, 0));
	myVertex[10] = Vertex(glm::vec3(width + x, -height + y, depth + z), glm::vec2(1, 0), glm::vec3(0, -1, 0), glm::vec3(1, 0.5, 0));
	myVertex[11] = Vertex(glm::vec3(width + x, -height + y, -depth + z), glm::vec2(0, 0), glm::vec3(0, -1, 0), glm::vec3(1, 0.5, 0));

	myVertex[12] = Vertex(glm::vec3(-width + x, height + y, -depth + z), glm::vec2(0, 1), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0));			//red
	myVertex[13] = Vertex(glm::vec3(-width + x, height + y, depth + z), glm::vec2(1, 1), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0));
	myVertex[14] = Vertex(glm::vec3(width + x, height + y, depth + z), glm::vec2(1, 0), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0));
	myVertex[15] = Vertex(glm::vec3(width + x, height + y, -depth + z), glm::vec2(0, 0), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0));

	myVertex[16] = Vertex(glm::vec3(-width + x, -height + y, -depth + z), glm::vec2(1, 1), glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1));			//blue
	myVertex[17] = Vertex(glm::vec3(-width + x, -height + y, depth + z), glm::vec2(1, 0), glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1));
	myVertex[18] = Vertex(glm::vec3(-width + x, height + y, depth + z), glm::vec2(0, 0), glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1));
	myVertex[19] = Vertex(glm::vec3(-width + x, height + y, -depth + z), glm::vec2(0, 1), glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1));

	myVertex[20] = Vertex(glm::vec3(width + x, -height + y, -depth + z), glm::vec2(1, 1), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));			//green
	myVertex[21] = Vertex(glm::vec3(width + x, -height + y, depth + z), glm::vec2(1, 0), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));
	myVertex[22] = Vertex(glm::vec3(width + x, height + y, depth + z), glm::vec2(0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));
	myVertex[23] = Vertex(glm::vec3(width + x, height + y, -depth + z), glm::vec2(0, 1), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));

	return myVertex;
}

unsigned int indices[] = { 0, 1, 2,
0, 2, 3,

6, 5, 4,
7, 6, 4,

10, 9, 8,
11, 10, 8,

12, 13, 14,
12, 14, 15,

16, 17, 18,
16, 18, 19,

22, 21, 20,
23, 22, 20
};

Shape::Shape(const Shape& shape)
{
	if(shape.mesh)
		mesh = shape.mesh;
	if(shape.lineMesh)
		lineMesh = shape.lineMesh;
	tex = shape.tex;
	isCopy = true;
}
Shape::Shape(const std::string& fileName) {
	mesh = new Mesh(fileName,true);
	tex = 0;
	isCopy = false;
}

Shape::Shape(const std::string & fileName, const unsigned maxFaces)
{
	mesh = new Mesh(fileName, maxFaces, true);
	tex = 0;
	isCopy = false;
	
}

Shape::Shape(const std::string& fileName,const std::string& textureFileName){
	mesh = new Mesh(fileName, true);
	tex = new Texture(textureFileName);
	isCopy = false;
}

Shape::Shape(const std::string & fileName, const std::string & textureFileName, const unsigned maxFaces)
{
	mesh = new Mesh(fileName, maxFaces, true);
	tex = new Texture(textureFileName);
	isCopy = false;

	// Create bounding cube mesh.
	Vertex * cubeVertices = init_cube(mesh->kdTree.getRoot()->m_center.x, mesh->kdTree.getRoot()->m_center.y, mesh->kdTree.getRoot()->m_center.z, mesh->kdTree.getRoot()->m_halfWidth, mesh->kdTree.getRoot()->m_halfHeight, mesh->kdTree.getRoot()->m_halfDepth);
	cubeMesh = new Mesh(cubeVertices, sizeof(cubeVertices) * 44 * 24 / sizeof((cubeVertices)[0]), indices, sizeof(indices) / sizeof(indices[0]), false);
}

Shape::Shape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
	mesh = new Mesh(vertices,numVertices,indices,numIndices, true);
	tex = 0; 
	isCopy = false;
}

Shape::Shape(LineVertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{

	lineMesh = new LineMesh(vertices,numVertices,indices,numIndices);
	tex = 0; 
	isCopy = false;
}

Shape::Shape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,const std::string& textureFileName)
{
	mesh = new Mesh(vertices,numVertices,indices,numIndices, true);
	tex = new Texture(textureFileName);
	isCopy = false;
}

void Shape::addMesh(Vertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices, const std::string& textureFileName)
{
	mesh = new Mesh(vertices,numVertices,indices,numIndices, true);
	tex = new Texture(textureFileName);
}

void Shape::addMesh(Vertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices)
{
	mesh = new Mesh(vertices,numVertices,indices,numIndices, true);
}

void Shape::addLineMesh(LineVertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices)
{
	lineMesh = new LineMesh(vertices,numVertices,indices,numIndices);
}

void Shape::draw(int mode)
{
	if (tex)
		tex->Bind();

	if (mesh) 
	{
		mesh->Draw(mode);

		// Draw the cube mesh.
		cubeMesh->Draw(1);
	}
	if (lineMesh)
		lineMesh->Draw();
}

Shape::~Shape(void)
{
	if(!isCopy)
	{
		if(mesh)
 			delete mesh;
		if(lineMesh)
			delete lineMesh;
		if(tex)
			delete tex;
	}
}

void Shape::getPoints(std::list<glm::vec3> &pList)
{

}

