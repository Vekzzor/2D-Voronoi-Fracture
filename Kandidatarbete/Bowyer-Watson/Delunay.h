#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <limits>


constexpr double eps = 1e-4;

class DEdge
{
public:
	DEdge(sf::Vector2f* const& _v1, sf::Vector2f* const& _v2) : v1{ _v1 }, v2{ _v2 } {}
	DEdge(const DEdge &e)
	{
		this->v1 = e.v1;
		this->v2 = e.v2;
	}
	~DEdge()
	{
		v1 = nullptr;
		v2 = nullptr;
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
	sf::Vector2f* v1;
	sf::Vector2f* v2;
private:
	//int refCount = 0;
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
	sf::Vector2f* v1;
	sf::Vector2f* v2;
	sf::Vector2f* v3;
	DCircle circle;
	Triangle(sf::Vector2f* _v1, sf::Vector2f* _v2, sf::Vector2f* _v3)
	  : v1{ _v1 },
		v2{ _v2 },
		v3{ _v3 },
		e1{ _v2, _v1 },
		e2{ _v3, _v2 },
		e3{ _v1, _v3 },
		circle{}
	{

		//if (this->edgeOrientation() == 2)
		//{
		//	//if(this->re_OrderEdges())
		//		int k = 0;
		//}
		const float ax = v2->x - v1->x;
		const float ay = v2->y - v1->y;
		const float bx = v3->x - v1->x;
		const float by = v3->y - v1->y;

		const float m = v2->x * v2->x - v1->x * v1->x + v2->y * v2->y - v1->y * v1->y;
		const float u = v3->x * v3->x - v1->x * v1->x + v3->y * v3->y - v1->y * v1->y;
		const float s = 1. / (2. * (ax * by - ay * bx));

		this->circle.x = ((v3->y - v1->y) * m + (v1->y - v2->y) * u) * s;
		this->circle.y = ((v1->x - v3->x) * m + (v2->x - v1->x) * u) * s;

		const float dx = v1->x - this->circle.x;
		const float dy = v1->y - this->circle.y;
		this->circle.radius = dx * dx + dy * dy;
	}
	~Triangle()
	{
		 v1	= nullptr;
		 v2	= nullptr;
		 v3	= nullptr;
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

	int orientation()
	{
		int val = (v2->y - v1->y) * (v3->x - v2->x) -
			(v2->x - v1->x) * (v3->y - v2->y);

		if (val == 0) return 0;  // colinear 

		return (val > 0) ? 1 : 2; // clock or counterclock wise 
	}

	int edgeOrientation()
	{
		int val = 0;
		if (e1.v2 == e2.v1 && e2.v2 == e3.v1 && e3.v2 == e1.v1)
		{
			val = 1;
		}
		else if (e1.v2 == e3.v1 && e3.v2 == e2.v1 && e2.v2 == e1.v1)
		{
			val = 2;
		}

		return val;
	}

	float counterClockwiseResult()
	{
		float result = (v2->x - v1->x) * (v3->y - v1->y) -
			(v3->x - v1->x) * (v2->y - v1->y);
		return result;
	}
	bool re_OrderEdges()
	{
		std::swap(v1, v2);

		e1.v1 = v2;
		e1.v2 = v1;

		e2.v1 = v3;
		e2.v2 = v2;

		e3.v1 = v1;
		e3.v2 = v3;

	

		//sf::Vector2f* tempV;
		////swap Dir
		//tempV = e1.v1;
		//e1.v1 = e1.v2;
		//e1.v2 = tempV;

		//tempV = e2.v1;
		//e2.v1 = e2.v2;
		//e2.v2 = tempV;

		//tempV = e3.v1;
		//e3.v1 = e3.v2;
		//e3.v2 = tempV;

		//////swap Edges
		////DEdge tempE(e1);
		////e1 = e3;
		////e3 = e2;
		////e2 = tempE;

		return this->orientation() == 1;
	}

	bool IsCounterClockwise(sf::Vector2f point1, sf::Vector2f point2, sf::Vector2f point3)
	{
		float result = (point2.x - point1.x) * (point3.y - point1.y) -
			(point3.x - point1.x) * (point2.y - point1.y);
		return result > 0;
	}
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
	std::vector<sf::Vector2f*> _vertices;
	std::vector<sf::Vector2f*> superTriangle;
public:
	Delunay();
	~Delunay();

	std::vector<Triangle>& Triangulate(std::vector<sf::Vector2f*>& points);

	const std::vector<Triangle>& getTriangles() const { return _triangles; };
	const std::vector<DEdge>& getEdges() const { return _edges; };
	const std::vector<sf::Vector2f*>& getVertices() const { return _vertices; };
	const std::vector<sf::Vector2f*>& getSuperTriangle() const { return superTriangle; };
};

