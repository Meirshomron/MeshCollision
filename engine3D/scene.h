#pragma once
#include "shader.h"
#include "shape.h"
#include "camera.h"
#include <vector>

class Scene : public MovableGLM
{
	std::vector<Shader*> shaders;

	Shape *axisMesh;

protected:
	int pickedShape;

public:

	std::vector<Shape*> shapes;
	std::vector<Camera*> cameras; //light will have the properties of camera
	enum axis{xAxis,yAxis,zAxis};
	enum transformations{xLocalTranslate,yLocalTranslate,zLocalTranslate,xGlobalTranslate,yGlobalTranslate,zGlobalTranslate,
		xLocalRotate,yLocalRotate,zLocalRotate,xGlobalRotate,yGlobalRotate,zGlobalRotate,xScale,yScale,zScale,xCameraTranslate,yCameraTranslate,zCameraTranslate};
	Scene();
	Scene(glm::vec3 position,float angle,float hwRelation,float near, float far);
	void addShape(int type);
	void addShape(const std::string& fileName);
	void addShape(const std::string & fileName, const unsigned maxFaces);
	void addShape(const std::string& fileName,const std::string& textureFileName);
	Shape* addShape(const std::string & fileName, const std::string & textureFileName, const unsigned maxFaces);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,const std::string& textureFileName);
	void addShader(const std::string& fileName);

	glm::mat4 GetViewProjection(int indx) const;
	glm::mat4 GetShapeTransformation() const;
	void draw(int shaderIndx,int cameraIndx,bool drawAxis, int index, Node* smallest);
	void shapeTransformation(int type,float amt);
	void inline setPicked(int pickID){pickedShape = pickID;}
	void resize(int width,int hight,int near,int far);
	virtual ~Scene(void);
};

