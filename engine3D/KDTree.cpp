/**
*
* k-d tree implementation file
*
* author: Arvind Rao
* license: GNU
*
*
*/
#include "KDTree.h"
#include <iostream>

// implementation of Node Class

Node::Node() { m_left = 0; m_right = 0; }
Node::Node(vecType median) : m_median(median) { m_left = 0; m_right = 0; }

bool Node::isLastInnerNode() 
{
	if (m_left == nullptr && m_right == nullptr)
		return false;
	if (m_left == nullptr && m_right->m_right == nullptr && m_right->m_left == nullptr)
		return true;
	if (m_right == nullptr && m_left->m_right == nullptr && m_left->m_left == nullptr)
		return true;
	if (m_right->m_right == nullptr && m_right->m_left == nullptr && m_left->m_right == nullptr && m_left->m_left == nullptr)
		return true;
	return false;
}

Node::~Node() {}


// implementation of KDTree

KDTree::KDTree() {}
KDTree::~KDTree() {}

/*
* @tbrief Give plist, sort it according to the axis and set the first half in the given left and the rest in the given right list and return the last element in the left list.
* @treturn [myKDtree::vecType] the median of the plist according to to the sorting of the axis.
*/
KDTree::vecType KDTree::splitListByMedian(int axis, std::list<KDTree::vecType> &plist, std::list<KDTree::vecType> &left, std::list<KDTree::vecType> &right)
{
	KDTree::vecType median;
	int size = plist.size();
	int med = ceil(float(size) / 2.0f);
	int count = 0;

	if (size == 1)
		return plist.front();

	// Using lambda function here, to define comparison function parametrized by 'axis'
	plist.sort([&](KDTree::vecType& a, KDTree::vecType& b) {return a[axis] < b[axis]; });

	for (auto& x : plist)
	{
		if (count < med)
			left.push_back(x);
		else
			right.push_back(x);
		++count;
	}
	median = left.back();
	left.pop_back();
	return median;
}

// @tbrief Updates the center, width, height and depth of the obj of this head node.
void KDTree::setHeadVariables(Node* head, std::list<KDTree::vecType>& plist)
{
	float maxX = -INFINITY;
	float maxY = -INFINITY;
	float maxZ = -INFINITY;

	float minX = INFINITY;
	float minY = INFINITY;
	float minZ = INFINITY;

	for (auto& n : plist)
	{
		if (n.x < minX)
			minX = n.x;
		if (n.y < minY)
			minY = n.y;
		if (n.z < minZ)
			minZ = n.z;

		if (n.x > maxX)
			maxX = n.x;
		if (n.y > maxY)
			maxY = n.y;
		if (n.z > maxZ)
			maxZ = n.z;
	}
	head->m_center = glm::vec4((minX + maxX) / 2, (minY + maxY) / 2, (minZ + maxZ) / 2, 1);
	head->m_axisX = glm::vec3(1, 0, 0);
	head->m_axisY = glm::vec3(0, 1, 0);
	head->m_axisZ = glm::vec3(0, 0, 1);
	head->m_halfWidth = head->m_center.x - minX;
	head->m_halfHeight = head->m_center.y - minY;
	head->m_halfDepth = head->m_center.z - minZ;
}

// @tbrief Algorithm is based on http://en.wikipedia.org/wiki/Kd_tree.
void KDTree::makeTree(std::list<KDTree::vecType>& plist)
{
	Node* head = new Node();
	makeTreeRecursivelyImpl(head, plist, 0);
	m_root = head;
}

void KDTree::makeTreeRecursivelyImpl(Node* head, std::list<KDTree::vecType>& plist, int depth)
{
	if (!plist.empty())
	{
		// Divide the given plist according to the current axis, every change of depth splits the remaining nodes in the next axis.
		int axis = depth % 3;

		std::list<KDTree::vecType> left_list;
		std::list<KDTree::vecType> right_list;
		KDTree::vecType median = splitListByMedian(axis, plist, left_list, right_list);
		head->m_median = median;

		setHeadVariables(head, plist);

		Node* left_node = new Node();
		Node* right_node = new Node();

		KDTree::makeTreeRecursivelyImpl(left_node, left_list, depth + 1);
		if (!left_list.empty()) 
			head->m_left = left_node;

		KDTree::makeTreeRecursivelyImpl(right_node, right_list, depth + 1);
		if (!right_list.empty()) 
			head->m_right = right_node;
	}
}

void KDTree::printTree(Node* head)
{
	int depth = 0;

	std::cout << "\n********** Print of Tree **********\n";
	std::queue< Node* > current, next;
	Node * temp = head;
	current.push(temp);

	while (!current.empty())
	{
		temp = current.front();
		current.pop();

		if (temp == nullptr)
			std::cout << "NULL\n";
		else
		{
			std::cout << "median: " << temp->m_median.x << ", " << temp->m_median.y << ", " << temp->m_median.z << std::endl;
			std::cout << "center: " << temp->m_center.x << ", " << temp->m_center.y << ", " << temp->m_center.z << std::endl;
			std::cout << "halfDepth: " << temp->m_halfDepth << std::endl;
			std::cout << "halfHeight: " << temp->m_halfHeight << std::endl;
			std::cout << "halfWidth: " << temp->m_halfWidth << std::endl;

			next.push(temp->m_left);
			next.push(temp->m_right);
		}
		if (current.empty())
		{
			depth++;
			std::cout << "level: " << depth << "\n";
			std::swap(current, next);
		}
	}
}