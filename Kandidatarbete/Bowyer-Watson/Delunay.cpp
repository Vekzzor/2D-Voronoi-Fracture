#include "Delunay.h"



Delunay::Delunay()
{
}


Delunay::~Delunay()
{
}

std::vector<Triangle>& Delunay::Triangulate(std::vector<DVertex*>& points)
{
	_vertices = points;
	// Determinate the super triangle
	float minX = points[0]->x;
	float minY = points[0]->y;
	float maxX = minX;
	float maxY = minY;

	for (sf::Vector2f* pt : points)
	{
		minX = std::min(minX, pt->x);
		maxX = std::max(maxX, pt->x);
		minY = std::min(minY, pt->y);
		maxY = std::max(maxY, pt->y);
	}

	const float dx = maxX - minX;
	const float dy = maxY - minY;
	const float deltaMax = std::max(dx, dy);
	const float midx = (minX + maxX)*0.5f;
	const float midy = (minY + maxY)*0.5f;

	_triangles.clear();
	_edges.clear();

	DVertex* p1 = new DVertex(midx - 20 * deltaMax, midy - deltaMax);
	DVertex* p2 = new DVertex(midx, midy + 20 * deltaMax);
	DVertex* p3 = new DVertex(midx + 20 * deltaMax, midy - deltaMax);
	superTriangle.push_back(p1);
	superTriangle.push_back(p2);
	superTriangle.push_back(p3);
	// Create a list of triangles, and add the supertriangle in it
	_triangles.push_back({ p1, p2, p3 });
	
	int pointIndex = 0;
	for (DVertex* const pt : points)
	{
		
		//std::cout << "_triangles contains " << _triangles.size() << " elements" << std::endl;
		std::vector<DEdge> edges;
		std::vector<Triangle> tmps;
		for (const Triangle& t : _triangles)
		{
			//std::cout << "Processing " << std::endl << *t << std::endl;
			const auto dist = (t.circle.x - pt->x) * (t.circle.x - pt->x) +
							  (t.circle.y - pt->y) * (t.circle.y - pt->y);
			if ((dist - t.circle.radius) <= eps)
			{
				//std::cout << "Pushing bad triangle " << *t << std::endl;
				//t.isBad = true;
				for(int i = 0; i < 3; i++)
					edges.push_back(t.e[i]);
			}
			else
			{
				tmps.push_back(t);
				//std::cout << " does not contains " << *p << " in his circum center" << std::endl;
			}
		}

		/* Delete duplicate edges. */
		std::vector<bool> remove(edges.size(), false);
		//for (int i = 0; i < edges.size(); ++i)
		//{
		//	for (int k = 0; k < edges.size(); ++k)
		//	{
		//		if (i == k) {
		//			continue;
		//		}
		//		if (edges[i] == edges[k])
		//		{
		//			remove[i] = true;
		//			remove[k] = true;
		//		}
		//	}
		//}

		for (auto it1 = edges.begin(); it1 != edges.end(); ++it1) {
			for (auto it2 = edges.begin(); it2 != edges.end(); ++it2) {
				if (it1 == it2) {
					continue;
				}
				if (*it1 == *it2) {
					remove[std::distance(edges.begin(), it1)] = true;
					remove[std::distance(edges.begin(), it2)] = true;
				}
			}
		}
		auto is_duplicate = [&](auto const& e) { return remove[&e - &edges[0]]; };
		erase_where(edges, is_duplicate);

		/* Update triangulation. */
		for (const DEdge &e : edges)
		{

			tmps.push_back(Triangle( e.v1, e.v2, pt));
		}

		_triangles = tmps;
	}

	auto is_part_STriangle_Old = [&](Triangle const& tri) {
		return ((tri.v1 == p1 || tri.v2 == p1 || tri.v3 == p1) ||
			(tri.v1 == p2 || tri.v2 == p2 || tri.v3 == p2) ||
			(tri.v1 == p3 || tri.v2 == p3 || tri.v3 == p3)); };
/*
	auto is_part_STriangle = [&](Triangle const& tri) {
		return (tri.edgeContainsVertex(p1) ||
			tri.edgeContainsVertex(p2) ||
			tri.edgeContainsVertex(p3)); };
*/
	//erase_where(_triangles, is_part_STriangle_Old);
	int i = 0;


	for (auto & t : _triangles)
	{
		if (t.orientation() == 2)
			if (t.re_OrderEdges())
				int k = 0;
		i++;

		for (int k = 0; k < 3; k++)
			_edges.push_back(t.e[i]);
	}

	
	return _triangles;
}
