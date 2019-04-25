#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <limits>


constexpr double eps = 1e-4;
//bool almost_equalTriangle(const Triangle &t1, const Triangle &t2)
//{
//	return	(almost_equal(t1.p1, t2.p1) || almost_equal(t1.p1, t2.v2) || almost_equal(t1.p1, t2.v3)) &&
//		(almost_equal(t1.v2, t2.p1) || almost_equal(t1.v2, t2.v2) || almost_equal(t1.v2, t2.v3)) &&
//		(almost_equal(t1.v3, t2.p1) || almost_equal(t1.v3, t2.v2) || almost_equal(t1.v3, t2.v3));
//}
//class DEdge;
//
//bool almost_equal(float x, float y, int ulp = 2);
//bool almost_equalVec(const sf::Vector2f& v1, const sf::Vector2f& v2, int ulp = 2);
//bool almost_equalEdge(const DEdge & e1, const DEdge & e2);

class DEdge
{
public:
	/*DEdge()
	{
		this->v1 = { 0,0 };
		this->v2 = { 0,0 };
	}*/
	DEdge(sf::Vector2f const& _v1, sf::Vector2f const& _v2) : v1{ _v1 }, v2{ _v2 } {}
	/*DEdge(sf::Vector2f p1, sf::Vector2f p2)
	{
		this->v1 = p1;
		this->v2 = p2;
	}*/
	DEdge(const DEdge &e)
	{
		this->v1 = e.v1;
		this->v2 = e.v2;
	}
	void operator=(const DEdge &e)
	{
		this->v1 = e.v1;
		this->v2 = e.v2;
	}
	bool operator==(const DEdge &other)const
	{
		return ((other.v1 == v1 && other.v2 == v2) ||
			(other.v1 == v2 && other.v2 == v1));
	}
	sf::Vector2f v1;
	sf::Vector2f v2;
private:
	int refCount = 0;
};

struct DCircle {
	float x, y, radius;
	DCircle() = default;
};

class Triangle
{
private:
	bool cmpf(float A, float B, float epsilon = 0.005f) const
	{
		return (fabs(A - B) < epsilon);
	}
	bool cmpfVec(const sf::Vector2f& A, const sf::Vector2f& B) const
	{
		return (cmpf(A.x, B.x) && cmpf(A.y, B.y));
	}
public:
	DEdge e1;
	DEdge e2;
	DEdge e3;
	sf::Vector2f v1;
	sf::Vector2f v2;
	sf::Vector2f v3;
	bool isBad = false;
	DCircle circle;
	//Triangle(sf::Vector2f* p1, sf::Vector2f* p2, sf::Vector2f* p3);
	Triangle(sf::Vector2f _v1, sf::Vector2f _v2, sf::Vector2f _v3)
	  : v1{ _v1 },
		v2{ _v2 },
		v3{ _v3 },
		e1{ _v1, _v2 },
		e2{ _v2, _v3 },
		e3{ _v1, _v3 },
		circle{}
	{
		const float ax = v2.x - v1.x;
		const float ay = v2.y - v1.y;
		const float bx = v3.x - v1.x;
		const float by = v3.y - v1.y;

		const float m = v2.x * v2.x - v1.x * v1.x + v2.y * v2.y - v1.y * v1.y;
		const float u = v3.x * v3.x - v1.x * v1.x + v3.y * v3.y - v1.y * v1.y;
		const float s = 1. / (2. * (ax * by - ay * bx));

		this->circle.x = ((v3.y - v1.y) * m + (v1.y - v2.y) * u) * s;
		this->circle.y = ((v1.x - v3.x) * m + (v2.x - v1.x) * u) * s;

		const float dx = v1.x - this->circle.x;
		const float dy = v1.y - this->circle.y;
		this->circle.radius = dx * dx + dy * dy;
	}
	void operator=(const Triangle &t)
	{
		this->e1 = t.e1;
		this->e2 = t.e2;
		this->e3 = t.e3;

		this->v1 = t.v1;
		this->v2 = t.v2;
		this->v3 = t.v3;

		this->circle = t.circle;

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
		const float ab = (v1.x*v1.x) + (v1.y*v1.y);
		const float cd = (v2.x*v2.x) + (v2.y*v2.y);
		const float ef = (v3.x*v3.x) + (v3.y*v3.y);

		const float circum_x = (ab * (v3.y - v2.y) + cd * (v1.y - v3.y) + ef * (v2.y - v1.y)) / (v1.x * (v3.y - v2.y) + v2.x * (v1.y - v3.y) + v3.x * (v2.y - v1.y));
		const float circum_y = (ab * (v3.x - v2.x) + cd * (v1.x - v3.x) + ef * (v2.x - v1.x)) / (v1.y * (v3.x - v2.x) + v2.y * (v1.x - v3.x) + v3.y * (v2.x - v1.x));

		const sf::Vector2f circum(0.5f*(circum_x), 0.5f*(circum_y));
		
		float dx = v1.x - circum.x;
		float dy = v1.y - circum.y;

		const float circum_radius = dx * dx + dy * dy;

		dx = v.x - circum.x;
		dy = v.y - circum.y;

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
		return cmpfVec(v1, v) || cmpfVec(v2, v) || cmpfVec(v3, v);
	}
	bool edgeContainsVertex(const sf::Vector2f &v) const
	{
		// return p1 == v || p2 == v || p3 == v;
		return (cmpfVec(e1.v1, v) || cmpfVec(e1.v2, v) || 
			    cmpfVec(e2.v1, v) || cmpfVec(e2.v2, v) || 
			    cmpfVec(e3.v1, v) || cmpfVec(e3.v2, v));
	}
	//bool containsVertex(const sf::Vector2f &v) const
	//{
	//	// return p1 == v || p2 == v || p3 == v;
	//	return almost_equalVec(v1, v) || almost_equalVec(v2, v) || almost_equalVec(v3, v);
	//}
	//void UpdateCircumCircle();
};

template<class Container, class F>
auto erase_where(Container& c, F&& f)
{
	return c.erase(std::remove_if(c.begin(),
		c.end(),
		std::forward<F>(f)),
		c.end());
}

class Delunay
{
private:
	std::vector<Triangle> _triangles;
	std::vector<DEdge> _edges;
	std::vector<sf::Vector2f> _vertices;
	std::vector<sf::Vector2f> _edge1;
	std::vector<sf::Vector2f> _edge2;
public:
	Delunay();
	~Delunay();

	std::vector<Triangle>& Triangulate(std::vector<sf::Vector2f>& points);

	const std::vector<Triangle>& getTriangles() const { return _triangles; };
	const std::vector<DEdge>& getEdges() const { return _edges; };
	const std::vector<sf::Vector2f>& getEdge1() const { return _edge1; };
	const std::vector<sf::Vector2f>& getEdge2() const { return _edge2; };
	const std::vector<sf::Vector2f>& getVertices() const { return _vertices; };
};

