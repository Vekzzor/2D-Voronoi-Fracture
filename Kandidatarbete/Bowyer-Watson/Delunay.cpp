#include "Delunay.h"



Delunay::Delunay()
{
}


Delunay::~Delunay()
{
}

std::vector<Triangle>& Delunay::Triangulate(std::vector<sf::Vector2f>& points)
{
	_vertices = points;
	// Determinate the super triangle
	float minX = points[0].x;
	float minY = points[0].y;
	float maxX = minX;
	float maxY = minY;

	for (auto const& pt : points)
	{
		minX = std::min(minX, pt.x);
		maxX = std::max(maxX, pt.x);
		minY = std::min(minY, pt.y);
		maxY = std::max(maxY, pt.y);
	}

	const float dx = maxX - minX;
	const float dy = maxY - minY;
	const float deltaMax = std::max(dx, dy);
	const float midx = (minX + maxX)*0.5f;
	const float midy = (minY + maxY)*0.5f;

	_triangles.clear();
	_edges.clear();

	const sf::Vector2f p1(midx - 20 * deltaMax, midy - deltaMax);
	const sf::Vector2f p2(midx, midy + 20 * deltaMax);
	const sf::Vector2f p3(midx + 20 * deltaMax, midy - deltaMax);

	// Create a list of triangles, and add the supertriangle in it
	_triangles.push_back({ p1, p2, p3 });
	

	for (auto const& pt : points)
	{
		
		//std::cout << "_triangles contains " << _triangles.size() << " elements" << std::endl;
		std::vector<DEdge> edges;
		std::vector<Triangle> tmps;
		for (const Triangle& t : _triangles)
		{
			//std::cout << "Processing " << std::endl << *t << std::endl;
			const auto dist = (t.circle.x - pt.x) * (t.circle.x - pt.x) +
							  (t.circle.y - pt.y) * (t.circle.y - pt.y);
			if ((dist - t.circle.radius) <= eps)
			{
				//std::cout << "Pushing bad triangle " << *t << std::endl;
				//t.isBad = true;
				edges.push_back(t.e1);
				edges.push_back(t.e2);
				edges.push_back(t.e3);
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
		for (auto const& e : edges)
		{

			tmps.push_back({ e.v1, e.v2, pt });
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
	erase_where(_triangles, is_part_STriangle_Old);

	for (auto const& t : _triangles)
	{
		_edges.push_back(t.e1);
		_edges.push_back(t.e2);
		_edges.push_back(t.e3);


	}

	return _triangles;
}

//Triangle::Triangle(sf::Vector2f * p1, sf::Vector2f * p2, sf::Vector2f * p3)
//{
//	this->v1 = p1;
//	this->v2 = p2;
//	this->v3 = p3;
//	if (this->IsCounterClockwise(p1, p2, p3))
//	{
//		e1 = new DEdge(p1, p2);
//		e2 = new DEdge(p2, p3);
//		e3 = new DEdge(p3, p1);
//	}
//	else
//	{
//		e1 = new DEdge(p1, p3);
//		e2 = new DEdge(p3, p2);
//		e3 = new DEdge(p2, p1);
//	}
//
//}



//void Triangle::UpdateCircumCircle()
//{
//	sf::Vector2f* p0 = this->v1;
//	sf::Vector2f* p1 = this->v1;
//	sf::Vector2f* p2 = this->v1;
//	float dA = p0->x * p0->x + p0->y * p0->y;
//	float dB = p1->x * p1->x + p1->y * p1->y;
//	float dC = p2->x * p2->x + p2->y * p2->y;
//
//	float aux1 = (dA * (p2->y - p1->y) + dB * (p0->y - p2->y) + dC * (p1->y - p0->y));
//	float aux2 = -(dA * (p2->x - p1->x) + dB * (p0->x - p2->x) + dC * (p1->x - p0->x));
//	float div = (2 * (p0->x * (p2->y - p1->y) + p1->x * (p0->y - p2->y) + p2->x * (p1->y - p0->y)));
//
//	if (div == 0)
//	{
//		return;
//	}
//
//	sf::Vector2f center(aux1 / div, aux2 / div);
//	Circumcenter = center;
//	RadiusSquared = (center.x - p0->x) * (center.x - p0->x) + (center.y - p0->y) * (center.y - p0->y);
//}

//void Triangle::UpdateCircumCircle()
//{
//	sf::Vector2f p0 = this->e1->v1;
//	sf::Vector2f p1 = this->e2->v1;
//	sf::Vector2f p2 = this->e3->v1;
//	float dA = p0.x * p0.x + p0.y * p0.y;
//	float dB = p1.x * p1.x + p1.y * p1.y;
//	float dC = p2.x * p2.x + p2.y * p2.y;
//
//	float aux1 = (dA * (p2.y - p1.y) + dB * (p0.y - p2.y) + dC * (p1.y - p0.y));
//	float aux2 = -(dA * (p2.x - p1.x) + dB * (p0.x - p2.x) + dC * (p1.x - p0.x));
//	float div = (2 * (p0.x * (p2.y - p1.y) + p1.x * (p0.y - p2.y) + p2.x * (p1.y - p0.y)));
//
//	if (div == 0)
//	{
//		return;
//	}
//
//	sf::Vector2f center(aux1 / div, aux2 / div);
//	Circumcenter = center;
//	RadiusSquared = (center.x - p0.x) * (center.x - p0.x) + (center.y - p0.y) * (center.y - p0.y);
//}

//bool almost_equal(float x, float y, int ulp)
//{
//	// the machine epsilon has to be scaled to the magnitude of the values used
//	// and multiplied by the desired precision in ULPs (units in the last place)
//	return fabs(x - y) <= std::numeric_limits<float>::epsilon() * fabs(x + y) * ulp
//		// unless the result is subnormal
//		|| fabs(x - y) < std::numeric_limits<float>::min();
//}
//
//bool almost_equalVec(const sf::Vector2f & v1, const sf::Vector2f & v2, int ulp)
//{
//	return almost_equal(v1.x, v2.x, ulp) && almost_equal(v1.y, v2.y, ulp);
//}
//
//bool almost_equalEdge(const DEdge & e1, const DEdge & e2)
//{
//	return	(almost_equalVec(e1.v1, e2.v1) && almost_equalVec(e1.v2, e2.v2)) ||
//		(almost_equalVec(e1.v1, e2.v2) && almost_equalVec(e1.v2, e2.v1));
//}
