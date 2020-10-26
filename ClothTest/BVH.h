#pragma once
#include "DebugDrawable.h"
#include <array>
#include <vector>
#include <type_traits>
#include "BoundingBox.h"

class Point;
class Sphere;
struct ConstructNode;

class BVH : DebugDrawable
{
public:
	BVH(const std::vector<Point*>& a_data);
	~BVH();

	void update();
	void draw(const Camera& a_camera, bool a_persistent)const;

	std::vector<Point*> getPayloadsWithinBox(const BoundingBox& a_box);
	std::vector<Point*> getPayloadsWithinSphere(const Sphere& a_sphere);

private:
	//definition of a node in the hierarchy
	struct Node
	{
		BoundingBox m_box;
		std::array<size_t, 2> m_children{ 0, 0 }; //both are zero if root, since root itself is zero
		Point* m_payload;

		Node(const BoundingBox& a_box);
		~Node();
	};

	//all the nodes in the bvh; first one is root
	std::vector<Node> m_nodes;

	//recursively partitions nodes to construct the tree
	void recursivePartition(ConstructNode&, size_t*);
	void recursiveFindPoints(const size_t a_nodeToSearch, const BoundingBox& a_box, std::vector<Point*>& a_target);
	void recursiveFindPoints(const size_t a_nodeToSearch, const Sphere& a_sphere, std::vector<Point*>& a_target);
	void recursiveUpdateNodes(const size_t a_nodeToSearch);
};
