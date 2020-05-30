#include "KDTree.h"
#include <iostream>
#include "shape.h"

std::queue<std::pair<Node*, Node*>> queue;

bool SolveEquision(Node* A, Node* B, glm::vec3 L, glm::mat4 TRTB, glm::mat4 TRTA)
{
	glm::vec4 centerB = TRTB * B->m_center;
	glm::vec4 centerA = TRTA * A->m_center;

	glm::vec3 T = glm::vec3(centerB.x - centerA.x, centerB.y - centerA.y, centerB.z - centerA.z);

	float p1 = glm::abs(glm::dot(A->m_halfWidth * A->m_axisX, L));
	float p2 = glm::abs(glm::dot(A->m_halfHeight * A->m_axisY, L));
	float p3 = glm::abs(glm::dot(A->m_halfDepth * A->m_axisZ, L));
	float p4 = glm::abs(glm::dot(B->m_halfWidth * B->m_axisX, L));
	float p5 = glm::abs(glm::dot(B->m_halfHeight * B->m_axisY, L));
	float p6 = glm::abs(glm::dot(B->m_halfDepth * B->m_axisZ, L));

	float leftSide = glm::abs(glm::dot(T, L));
	float rightSide = p1 + p2 + p3 + p4 + p5 + p6;

	if (leftSide > rightSide)
		return true;
	return false;
}

bool checkCollision(Node* A, Node* B, glm::mat4 TRTB, glm::mat4 TRTA)
{
	bool collison = true;
	glm::vec3 L;

	L = glm::vec3(TRTA * glm::vec4(A->m_axisX, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTA * glm::vec4(A->m_axisY, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTA * glm::vec4(A->m_axisZ, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTB * glm::vec4(B->m_axisX, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTB * glm::vec4(B->m_axisY, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTB * glm::vec4(B->m_axisZ, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTA * glm::vec4(A->m_axisX, 1)) * glm::vec3(TRTB * glm::vec4(B->m_axisX, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTA * glm::vec4(A->m_axisX, 1)) * glm::vec3(TRTB * glm::vec4(B->m_axisY, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTA * glm::vec4(A->m_axisX, 1)) * glm::vec3(TRTB * glm::vec4(B->m_axisZ, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTA * glm::vec4(A->m_axisY, 1)) * glm::vec3(TRTB * glm::vec4(B->m_axisX, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTA * glm::vec4(A->m_axisY, 1)) * glm::vec3(TRTB * glm::vec4(B->m_axisY, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTA * glm::vec4(A->m_axisY, 1)) * glm::vec3(TRTB * glm::vec4(B->m_axisZ, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTA * glm::vec4(A->m_axisZ, 1)) * glm::vec3(TRTB * glm::vec4(B->m_axisX, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTA * glm::vec4(A->m_axisZ, 1)) * glm::vec3(TRTB * glm::vec4(B->m_axisY, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTA * glm::vec4(A->m_axisZ, 1)) * glm::vec3(TRTB * glm::vec4(B->m_axisZ, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	return collison;
}

void print_queue(std::queue<std::pair<Node*, Node*>> q)
{
	while (!q.empty())
	{
		std::cout << q.front().first->m_center.x << ", " << q.front().first->m_center.y << ", " << q.front().first->m_center.z << std::endl;
		std::cout << q.front().second->m_center.x << ", " << q.front().second->m_center.y << ", " << q.front().second->m_center.z << std::endl;

		q.pop();
	}
	std::cout << std::endl;
}

std::pair<Node*, Node*> findCollision(Shape* firstShape, Shape* secondShape)
{
	queue.push(std::pair<Node*, Node*>(firstShape->mesh->kdTree.getRoot(), secondShape->mesh->kdTree.getRoot()));

	std::pair<Node*, Node*> ret;

	while (!queue.empty())
	{
		std::pair<Node*, Node*> curPair = queue.front();
		queue.pop();

		if (checkCollision(curPair.first, curPair.second, firstShape->makeTransScale(glm::mat4(1)), secondShape->makeTransScale(glm::mat4(1))))
		{
			if (curPair.first->isLastInnerNode() && curPair.second->isLastInnerNode())
			{
				ret = curPair;
				break;
			}
			else if (curPair.second->isLastInnerNode() && curPair.first != nullptr)
			{
				queue.push(std::pair<Node*, Node*>(curPair.first->m_left, curPair.second));
				queue.push(std::pair<Node*, Node*>(curPair.first->m_right, curPair.second));
			}
			else if (curPair.first->isLastInnerNode() && curPair.second != nullptr)
			{
				queue.push(std::pair<Node*, Node*>(curPair.first, curPair.second->m_left));
				queue.push(std::pair<Node*, Node*>(curPair.first, curPair.second->m_right));
			}
			else
			{
				queue.push(std::pair<Node*, Node*>(curPair.first->m_left, curPair.second->m_left));
				queue.push(std::pair<Node*, Node*>(curPair.first->m_right, curPair.second->m_left));
				queue.push(std::pair<Node*, Node*>(curPair.first->m_left, curPair.second->m_right));
				queue.push(std::pair<Node*, Node*>(curPair.first->m_right, curPair.second->m_right));
			}
		}
	}
	return ret;
}