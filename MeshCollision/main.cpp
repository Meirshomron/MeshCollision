#include <iostream>
#include "display.h"
#include "inputManager.h"
#include "KDTree.h"
#include "CollisionDetection.h"

Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
Scene scn(glm::vec3(0.0f, 0.0f, -15.0f), CAM_ANGLE, relation, NEAR, FAR);

int main(int argc, char** argv)
{
	initCallbacks(display);

	// Initiate 2 shapes, 1 static and one controllable with keyboard arrows.
	Shape* controllableShape = scn.addShape("./res/objs/monkey3.obj", "./res/textures/grass.bmp", 512U);
	Shape* staticShape = scn.addShape("./res/objs/monkey3.obj", "./res/textures/grass.bmp", 512U);
	glm::vec3 controllableMovement = glm::vec3(-0.001f, 0.0f, 0.0f);
	controllableShape->myRotate(180, glm::vec3(0, 1, 0));
	controllableShape->setConstantMovement(controllableMovement);
	controllableShape->myTranslate(glm::vec3(5, 0, 0), 0);

	staticShape->myRotate(180, glm::vec3(0, 1, 0));

	scn.addShader("./res/shaders/basicShader");
	scn.addShader("./res/shaders/pickingShader");

	bool collisionFound = false;
	std::pair<Node*, Node*> innerCollisionNodes;
	while(!display.toClose())
	{
		display.Clear(0.7f, 0.7f, 0.7f, 1.0f);

		// Try and find a colision.
		if (!collisionFound)
		{
			innerCollisionNodes = findCollision(controllableShape, staticShape);
			if (innerCollisionNodes.first)
			{
				controllableShape->setConstantMovement(glm::vec3(0));
				collisionFound = true;
			}
		}
		// Found collision - draw the innerCollisionNode boxes and stop movement.
		else
		{
			scn.draw(0, 0, true, 1, innerCollisionNodes.first);
			scn.draw(0, 0, true, 0, innerCollisionNodes.second);
		}

		controllableMovement = controllableShape->getConstantMovement();
		controllableShape->myTranslate(controllableMovement, 0);

		scn.draw(0, 0, true, 0, nullptr);
		display.SwapBuffers();
		display.pullEvent();
	}

	return 0;
}

