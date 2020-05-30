/*
*
* k-d tree declaration file
*
* author: Arvind Rao
* license: GNU
*
*
*/

#ifndef KDTREE_
#define KDTREE_
#define _USE_MATH_DEFINES

#include <memory>
#include <vector>
#include <list>
#include <cmath>
#include <queue>
#include <utility>
#include <glm\glm.hpp>

class Node
{
public:
	typedef glm::vec4 vecType;
	Node* m_left;
	Node* m_right;
	vecType m_median;

	glm::vec4 m_center;
	glm::vec3 m_axisX;
	glm::vec3 m_axisY;
	glm::vec3 m_axisZ;

	float m_halfWidth;
	float m_halfHeight;
	float m_halfDepth;

	// C'tor.
	Node();
	Node(vecType median);

	// Return true if one of the sons or both are leafs.
	bool isLastInnerNode();

	// D'tor.
	~Node();
};

class KDTree
{

public:

	typedef Node::vecType vecType;

	// C'tor.
	KDTree();

	// D'tor.
	~KDTree();

	// Return the tree root node.
	Node* getRoot() const { return m_root; };

	// Prints the tree.
	void printTree(Node* head);

	// Find the median of a list of points, split the list by it and return it.
	vecType splitListByMedian(int axis, std::list<vecType> &plist, std::list<vecType> &left, std::list<vecType> &right);

	// Making the tree.
	void makeTree(std::list<vecType> &plist);

	void setHeadVariables(Node* head, std::list<vecType> &plist);

private:
	// Helper for makeTree.
	void makeTreeRecursivelyImpl(Node* head, std::list<vecType> &plist, int depth);

	Node* m_root;
};


#endif