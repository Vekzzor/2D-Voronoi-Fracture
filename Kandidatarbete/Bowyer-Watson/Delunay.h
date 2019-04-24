#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <limits>



//bool almost_equalTriangle(const Triangle &t1, const Triangle &t2)
//{
//	return	(almost_equal(t1.p1, t2.p1) || almost_equal(t1.p1, t2.v2) || almost_equal(t1.p1, t2.v3)) &&
//		(almost_equal(t1.v2, t2.p1) || almost_equal(t1.v2, t2.v2) || almost_equal(t1.v2, t2.v3)) &&
//		(almost_equal(t1.v3, t2.p1) || almost_equal(t1.v3, t2.v2) || almost_equal(t1.v3, t2.v3));
//}
class DEdge;

bool almost_equal(float x, float y, int ulp = 2);
bool almost_equalVec(const sf::Vector2f& v1, const sf::Vector2f& v2, int ulp = 2);
bool almost_equalEdge(const DEdge & e1, const DEdge & e2);

class DEdge
{
public:
	DEdge(sf::Vector2f* p1, sf::Vector2f* p2)
	{
		this->v1 = p1;
		this->v2 = p2;
	}
	DEdge(sf::Vector2f p1, sf::Vector2f p2)
	{
		this->v1 = new sf::Vector2f(p1);
		this->v2 = new sf::Vector2f(p2);
	}
	void operator=(const DEdge &e)
	{
		this->v1 = e.v1;
		this->v2 = e.v2;
	}
	bool operator==(const DEdge &e)
	{
		return (this->v1 == e.v1 && this->v1 == e.v2);
	}
	sf::Vector2f* v1;
	sf::Vector2f* v2;
	bool isBad = false;
};

class Triangle
{
public:
	DEdge* e1;
	DEdge* e2;
	DEdge* e3;
	sf::Vector2f* v1;
	sf::Vector2f* v2;
	sf::Vector2f* v3;
	bool isBad = false;
	sf::Vector2f Circumcenter;
	double RadiusSquared;
	Triangle(sf::Vector2f* p1, sf::Vector2f* p2, sf::Vector2f* p3);
	Triangle(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3);
	void operator=(const Triangle &t)
	{
		this->e1 = t.e1;
		this->e2 = t.e2;
		this->e3 = t.e3;
	}
	bool operator==(const Triangle &t)
	{
		return (this->e1 == t.e1 && this->e2 == t.e2 && this->e3 == t.e3);
	}

	bool IsCounterClockwise(sf::Vector2f* point1, sf::Vector2f* point2, sf::Vector2f* point3)
	{
		float result = (point2->x - point1->x) * (point3->y - point1->y) -
			(point3->x - point1->x) * (point2->y - point1->y);
		return result > 0;
	}

	bool IsCounterClockwise(sf::Vector2f point1, sf::Vector2f point2, sf::Vector2f point3)
	{
		float result = (point2.x - point1.x) * (point3.y - point1.y) -
			(point3.x - point1.x) * (point2.y - point1.y);
		return result > 0;
	}

	bool circumCircleContains(const sf::Vector2f &v) const
	{
		const float ab = (v1->x*v1->x) + (v1->y*v1->y);
		const float cd = (v2->x*v2->x) + (v2->y*v2->y);
		const float ef = (v3->x*v3->x) + (v3->y*v3->y);

		const float circum_x = (ab * (v3->y - v2->y) + cd * (v1->y - v3->y) + ef * (v2->y - v1->y)) / (v1->x * (v3->y - v2->y) + v2->x * (v1->y - v3->y) + v3->x * (v2->y - v1->y));
		const float circum_y = (ab * (v3->x - v2->x) + cd * (v1->x - v3->x) + ef * (v2->x - v1->x)) / (v1->y * (v3->x - v2->x) + v2->y * (v1->x - v3->x) + v3->y * (v2->x - v1->x));

		const sf::Vector2f circum(0.5*(circum_x), 0.5*(circum_y));
		
		float dx = v1->x - circum.x;
		float dy = v1->y - circum.y;
		dx * dx + dy * dy;

		const float circum_radius = dx * dx + dy * dy;

		dx = v.x - circum.x;
		dy = v.y - circum.y;
		dx * dx + dy * dy;

		const float dist = dx * dx + dy * dy;

		return dist <= circum_radius;
	}

	/*bool IsPointInsideCircumcircle(sf::Vector2f &point)
	{
		UpdateCircumCircle();
		float d_squared = (point.x - Circumcenter.x) * (point.x - Circumcenter.x) +
			(point.y - Circumcenter.y) * (point.y - Circumcenter.y);
		return d_squared < RadiusSquared;
	}*/
	bool containsVertex(const sf::Vector2f &v) const
	{
		// return p1 == v || p2 == v || p3 == v;
		return almost_equalVec(*v1, v) || almost_equalVec(*v2, v) || almost_equalVec(*v3, v);
	}
	void UpdateCircumCircle();
};

class Delunay
{
private:
	std::vector<Triangle> _triangles;
	std::vector<DEdge*> _edges;
	std::vector<sf::Vector2f*> _vertices;
public:
	Delunay();
	~Delunay();

	std::vector<Triangle>& Triangulate(std::vector<sf::Vector2f*> points);

	const std::vector<Triangle>& getTriangles() const { return _triangles; };
	const std::vector<DEdge*>& getEdges() const { return _edges; };
	const std::vector<sf::Vector2f*>& getVertices() const { return _vertices; };
};

