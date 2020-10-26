#pragma once
#include "BVH.h"
#include <cmath>
#include <glad/glad.h>
#include "Point.h"
#include "Sphere.h"

struct ConstructNode
{
	BoundingBox m_box;
	std::vector<ConstructNode*> m_children;
	Point* m_payload = nullptr;
	ConstructNode() = default;
	~ConstructNode()
	{
		for (unsigned int k = 0; k < m_children.size(); k++)
		{
			delete m_children[k];
		}
	}
};

void setBoundingBoxEnclosing(BoundingBox& a_box, const std::vector<ConstructNode*>& a_toEnclose)
{
	a_box.m_min = glm::vec3(INFINITY);
	a_box.m_max = glm::vec3(-INFINITY);
	for (size_t k = 0; k < a_toEnclose.size(); k++)
	{
		for (size_t axis = 0; axis < 3; axis++)
		{
			a_box.m_min[axis] = std::min(a_box.m_min[axis], a_toEnclose[k]->m_box.m_min[axis]);
			a_box.m_max[axis] = std::max(a_box.m_max[axis], a_toEnclose[k]->m_box.m_max[axis]);
		}
	}
}

//partitions the shapes into the referenced vectors along the given axis
void partitionShapes(const ConstructNode& a_source, std::vector<ConstructNode*>& a_left, std::vector<ConstructNode*>& a_right, const unsigned int a_axis)
{

	const float halfVal = a_source.m_box.m_min[a_axis] + 0.5f * (a_source.m_box.m_max[a_axis] - a_source.m_box.m_min[a_axis]);

	for (unsigned int k = 0; k < a_source.m_children.size(); k++)
	{
		const float currentCenterVal = a_source.m_children[k]->m_box.getCenter()[a_axis];
		if (currentCenterVal <= halfVal)
		{
			a_left.push_back(a_source.m_children[k]);
		}
		else
		{
			a_right.push_back(a_source.m_children[k]);
		}
	}
}

BVH::BVH(const std::vector<Point*>& a_data)
{
	ConstructNode root;

	std::vector<ConstructNode*> constructNodes;
	constructNodes.reserve(a_data.size());
	for (unsigned int k = 0; k < a_data.size(); k++)
	{
		constructNodes.push_back(new ConstructNode());
		constructNodes.back()->m_box = BoundingBox(a_data[k]->getPos() - glm::vec3(0.001f, 0.001f, 0.001f), a_data[k]->getPos() + glm::vec3(0.001f, 0.001f, 0.001f));
		constructNodes.back()->m_payload = a_data[k];
		root.m_children.push_back(constructNodes.back());
	}
	setBoundingBoxEnclosing(root.m_box, constructNodes);

	if (constructNodes.size() > 0)
	{
		if (constructNodes.size() == 1)
		{
			m_nodes.emplace_back(root.m_box);
			auto& root = m_nodes[0];
			root.m_payload = constructNodes[0]->m_payload;
		}
		else
		{
			m_nodes.emplace_back(root.m_box);
			recursivePartition(root, new size_t(0));
		}
	}
}

//recursively partitions nodes to construct the tree
void BVH::recursivePartition(ConstructNode& a_node, size_t* a_finalNode)
{
	if (a_finalNode != nullptr)
	{
		if (a_node.m_children.size() > 2)
		{

			//gets the largest axis
			unsigned int largestAxis = 0;
			float axisSize = 0;
			for (unsigned int axis = 0; axis < 3; axis++)
			{
				const float currentAxisSize = a_node.m_box.m_max[axis] - a_node.m_box.m_min[axis];
				if (currentAxisSize > axisSize)
				{
					axisSize = currentAxisSize;
					largestAxis = axis;
				}
			}

			//partitions the node in that axis
			ConstructNode* newLeftChild = new ConstructNode();
			ConstructNode* newRightChild = new ConstructNode();
			partitionShapes(a_node, newLeftChild->m_children, newRightChild->m_children, largestAxis);

			//sets the new children
			a_node.m_children.clear();
			a_node.m_children.push_back(newLeftChild);
			a_node.m_children.push_back(newRightChild);

			//adds final child nodes to the node and continues partitioning
			if (newLeftChild->m_children.size() > 0)
			{
				setBoundingBoxEnclosing(newLeftChild->m_box, newLeftChild->m_children);
				m_nodes.emplace_back(newLeftChild->m_box);
				m_nodes[*a_finalNode].m_children[0] = m_nodes.size() - 1;
			}
			if (newRightChild->m_children.size() > 0)
			{
				setBoundingBoxEnclosing(newRightChild->m_box, newRightChild->m_children);
				m_nodes.emplace_back(newRightChild->m_box);
				m_nodes[*a_finalNode].m_children[1] = m_nodes.size() - 1;
			}

			//continues partitioning recursively
			recursivePartition(*newLeftChild, new size_t(m_nodes[*a_finalNode].m_children[0]));
			recursivePartition(*newRightChild, new size_t(m_nodes[*a_finalNode].m_children[1]));

		}
		else
		{
			for (unsigned int k = 0; k < a_node.m_children.size(); k++)
			{
				m_nodes.emplace_back(a_node.m_children[k]->m_box);
				m_nodes[*a_finalNode].m_children[k] = m_nodes.size() - 1;
				m_nodes[m_nodes[*a_finalNode].m_children[k]].m_payload = a_node.m_children[k]->m_payload;
			}
		}
	}
	delete a_finalNode;
}

void BVH::recursiveFindPoints(const size_t a_nodeToSearch, const BoundingBox& a_box, std::vector<Point*>& a_target)
{
	auto& node = m_nodes[a_nodeToSearch];
	if (node.m_box.intersectsBoundingBox(a_box))
	{
		if (node.m_payload)
		{
			a_target.push_back(node.m_payload);
		}
		else
		{
			for (auto& child : node.m_children)
			{
				if (child != 0)
				{
					recursiveFindPoints(child, a_box, a_target);
				}
			}
		}
	}
}

void BVH::recursiveFindPoints(const size_t a_nodeToSearch, const Sphere& a_sphere, std::vector<Point*>& a_target)
{
	auto& node = m_nodes[a_nodeToSearch];
	if (node.m_box.intersectsSphere(a_sphere.getPos(), a_sphere.getRadius()))
	{
		if (node.m_payload)
		{
			a_target.push_back(node.m_payload);
		}
		else
		{
			for (auto& child : node.m_children)
			{
				if (child != 0)
				{
					recursiveFindPoints(child, a_sphere, a_target);
				}
			}
		}
	}
}

void BVH::recursiveUpdateNodes(const size_t a_nodeToSearch)
{
	auto& node = m_nodes[a_nodeToSearch];

	//non-leaf nodes update their child boxes and then their own boxes based on the result
	if (!node.m_payload)
	{
		for (auto& child : node.m_children)
		{
			if (child != 0)
			{
				recursiveUpdateNodes(child);
			}
		}
		for (size_t axis = 0; axis < 3; axis++)
		{
			float min = INFINITY;
			float max = -INFINITY;
			for (size_t childIndex = 0; childIndex < 2; childIndex++)
			{
				size_t currentChild = node.m_children[childIndex];
				if (currentChild != 0)
				{
					min = std::min(min, m_nodes[currentChild].m_box.m_min[axis]);
					min = std::min(min, m_nodes[currentChild].m_box.m_max[axis]);
					max = std::max(max, m_nodes[currentChild].m_box.m_min[axis]);
					max = std::max(max, m_nodes[currentChild].m_box.m_max[axis]);
				}
			}
			node.m_box.m_min[axis] = min;
			node.m_box.m_max[axis] = max;
		}
	}
	//leaf nodes update their own box according to the current state of their points
	else
	{
		for (size_t axis = 0; axis < 3; axis++)
		{
			node.m_box.m_min = node.m_payload->getPos() - glm::vec3(0.001f, 0.001f, 0.001f);
			node.m_box.m_max = node.m_payload->getPos() + glm::vec3(0.001f, 0.001f, 0.001f);
		}
	}
	
}

BVH::~BVH()
{}

std::vector<Point*> BVH::getPayloadsWithinBox(const BoundingBox& a_box)
{
	std::vector<Point*> toReturn;
	recursiveFindPoints(0, a_box, toReturn);
	return toReturn;
}

std::vector<Point*> BVH::getPayloadsWithinSphere(const Sphere& a_sphere)
{
	std::vector<Point*> toReturn;
	recursiveFindPoints(0, a_sphere, toReturn);
	return toReturn;
}

void BVH::update()
{
	recursiveUpdateNodes(0);
}

void BVH::draw(const Camera& a_camera, bool a_persistent)const
{
	std::vector<float> data;
	data.reserve(m_nodes.size() * 12 * 6);
	for (const auto& node : m_nodes)
	{
		std::pair<glm::vec3, glm::ivec3> points[8]{
			{ node.m_box.m_min, { -1, -1, -1 } },
			{ { node.m_box.m_max.x, node.m_box.m_min.y, node.m_box.m_min.z }, { 1, -1, -1 } },
			{ { node.m_box.m_min.x, node.m_box.m_max.y, node.m_box.m_min.z }, { -1, 1, -1 } },
			{ { node.m_box.m_min.x, node.m_box.m_min.y, node.m_box.m_max.z }, { -1, -1, 1 } },
			{ { node.m_box.m_max.x, node.m_box.m_max.y, node.m_box.m_min.z }, { 1, 1, -1 } },
			{ { node.m_box.m_min.x, node.m_box.m_max.y, node.m_box.m_max.z }, { -1, 1, 1 } },
			{ { node.m_box.m_max.x, node.m_box.m_min.y, node.m_box.m_max.z }, { 1, -1, 1 } },
			{ node.m_box.m_max, { 1, 1, 1 } }
		};

		for (int k = 0; k < 8; k++)
		{
			for (int i = 0; i < 8; i++)
			{
				std::pair<glm::vec3, glm::ivec3>* currentPoints[2] { &points[k], &points[i] };
				int totalInCommon = 0;
				for (int axis = 0; axis < 3; axis++)
				{
					if (currentPoints[0]->second[axis] == currentPoints[1]->second[axis])
					{
						totalInCommon++;
					}
				}
				if (totalInCommon == 2)
				{
					for (size_t point = 0; point < 2; point++)
					{
						for (size_t axis = 0; axis < 3; axis++)
						{
							data.push_back(currentPoints[point]->first[axis]);
						}
						data.push_back(1.f);
						data.push_back(0.f);
						data.push_back(1.f);
					}
				}
			}
		}
	
	}
	if (a_persistent)
	{
		glDisable(GL_DEPTH_TEST);
	}
	drawData(a_camera, GL_LINES, sizeof(float) * data.size(), data.data(), data.size() / 6);
	if (a_persistent)
	{
		glEnable(GL_DEPTH_TEST);
	}
}

BVH::Node::Node(const BoundingBox& a_box)
	: m_box(a_box)
	, m_payload(nullptr)
{}

BVH::Node::~Node()
{}
