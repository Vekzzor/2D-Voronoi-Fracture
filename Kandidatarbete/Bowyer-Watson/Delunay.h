#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <limits>
#include "../Fortunes/Data Structures/DCEL.h"

constexpr double eps = 1e-4;
class DEdge
{
public:
	DEdge(HALF_EDGE::HE_Vertex* const& _v1, HALF_EDGE::HE_Vertex* const& _v2) : v1{ _v1 }, v2{ _v2 } {}
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
	void flipEdges()
	{
		std::swap(v1, v2);
	}
	HALF_EDGE::HE_Vertex* v1;
	HALF_EDGE::HE_Vertex* v2;
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
	DEdge e[3];
	HALF_EDGE::HE_Vertex* v1;
	HALF_EDGE::HE_Vertex* v2;
	HALF_EDGE::HE_Vertex* v3;
	DCircle circle;
	bool isBad = false;
	Triangle(HALF_EDGE::HE_Vertex* _v1, HALF_EDGE::HE_Vertex* _v2, HALF_EDGE::HE_Vertex* _v3)
	  : v1{ _v1 },
		v2{ _v2 },
		v3{ _v3 },
		e{ {_v2, _v1}, { _v3, _v2 }, { _v1, _v3 } },
		circle{}
	{
		/*if (this->orientation() == 2)
			this->flipOrientation();*/

		sf::Vector2f* p1 = v1->point;
		sf::Vector2f* p2 = v2->point;
		sf::Vector2f* p3 = v3->point;

		const float ax = p2->x - p1->x;
		const float ay = p2->y - p1->y;
		const float bx = p3->x - p1->x;
		const float by = p3->y - p1->y;

		const float m = p2->x * p2->x - p1->x * p1->x + p2->y * p2->y - p1->y * p1->y;
		const float u = p3->x * p3->x - p1->x * p1->x + p3->y * p3->y - p1->y * p1->y;
		const float s = 1. / (2. * (ax * by - ay * bx));

		this->circle.x = ((p3->y - p1->y) * m + (p1->y - p2->y) * u) * s;
		this->circle.y = ((p1->x - p3->x) * m + (p2->x - p1->x) * u) * s;

		const float dx = p1->x - this->circle.x;
		const float dy = p1->y - this->circle.y;
		this->circle.radius = dx * dx + dy * dy;


		/*this->face = new HALF_EDGE::HE_Face();
		this->face->circumCenter = sf::Vector2f(circle.x, circle.y);
		this->face->radius = circle.radius;

		for (int i = 0; i < 3; i++)
		{
			this->he[i] = new HALF_EDGE::HE_Edge();
			this->he[i]->face = this->face;
		}
		this->face->edge = this->he[0];
		this->he[0]->vert = v1;
		this->he[1]->vert = v2;
		this->he[2]->vert = v3;

		this->he[0]->setNext(this->he[1]);
		this->he[1]->setNext(this->he[2]);
		this->he[2]->setNext(this->he[0]);*/

	}
	~Triangle()
	{
		 v1	= nullptr;
		 v2	= nullptr;
		 v3	= nullptr;

	}
	void operator=(const Triangle &t)
	{
		this->e[0] = t.e[0];
		this->e[1] = t.e[1];
		this->e[2] = t.e[2];

		this->v1 = t.v1;
		this->v2 = t.v2;
		this->v3 = t.v3;

		this->circle = t.circle;
	}
	bool operator==(const Triangle &t)
	{
		return (this->e[0] == t.e[0] && this->e[1] == t.e[1] && this->e[2] == t.e[2]);
	}
	sf::Vector2f CalculateCircleCenter()
	{
		sf::Vector2f center;
		sf::Vector2f* p1 = v1->point;
		sf::Vector2f* p2 = v2->point;
		sf::Vector2f* p3 = v3->point;

		float ma = (p2->y - p1->y) / (p2->x - p1->x);
		float mb = (p3->y - p2->y) / (p3->x - p2->x);

		center.x = (ma * mb * (p1->y - p3->y) + mb * (p1->x + p2->x) - ma * (p2->x + p3->x)) / (2 * (mb - ma));

		center.y = (-1 / ma) * (center.x - (p1->x + p2->x) / 2) + (p1->y + p2->y) / 2;

		return center;
	}
	int orientation()
	{
		int val = (v2->point->y - v1->point->y) * (v3->point->x - v2->point->x) -
			(v2->point->x - v1->point->x) * (v3->point->y - v2->point->y);

		if (val == 0) return 0;  // colinear 

		return (val > 0) ? 1 : 2; // clock or counterclock wise 
	}

	int edgeOrientation()
	{
		int val = 0;
		if (e[0].v2 == e[1].v1 && e[1].v2 == e[2].v1 && e[2].v2 == e[0].v1)
		{
			val = 1;
		}
		else if (e[0].v2 == e[2].v1 && e[2].v2 == e[1].v1 && e[1].v2 == e[0].v1)
		{
			val = 2;
		}

		return val;
	}

	float counterClockwiseResult()
	{
		float result = (v2->point->x - v1->point->x) * (v3->point->y - v1->point->y) -
			(v3->point->x - v1->point->x) * (v2->point->y - v1->point->y);
		return result;
	}
	void flipOrientation()
	{
		std::swap(v1, v2);
	}
	bool re_OrderEdges()
	{
		std::swap(v1, v2);

		e[0].v1 = v2;
		e[0].v2 = v1;

		e[1].v1 = v3;
		e[1].v2 = v2;

		e[2].v1 = v1;
		e[2].v2 = v3;

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
auto erase_where(Container& c, F&& f, int offset = 0)
{
	return c.erase(std::remove_if(c.begin()+offset,
		c.end(),
		std::forward<F>(f)),
		c.end());
}

class Delunay
{
private:
	std::vector<Triangle> _triangles;
	std::vector<DEdge> _edges;
	std::vector<HALF_EDGE::HE_Vertex*> _vertices;
public:
	Delunay();
	~Delunay();

	std::vector<Triangle>& Triangulate(std::vector<HALF_EDGE::HE_Vertex*>& points);
	const std::vector<Triangle>& getTriangles() const { return _triangles; };
	const std::vector<DEdge>& getEdges() const { return _edges; };
	const std::vector<HALF_EDGE::HE_Vertex*>& getVertices() const { return _vertices; };
};


#if 0
bool findCircleCenter(const Point2D &p1, const Point2D &p2, const Point2D &p3, Point2D &center) 
{

	// get normalized vectors
	Point2D u1 = (p1 - p2).normalized(), u2 = (p3 - p2).normalized();

	double cross = crossProduct(u1, u2);

	// check if vectors are collinear
	if (fabs(cross) < CIRCLE_CENTER_EPSILON) {
		return false;
	}

	// get cental points
	Point2D pc1 = 0.5 * (p1 + p2), pc2 = 0.5 * (p2 + p3);

	// get free components
	double b1 = dotProduct(u1, pc1), b2 = dotProduct(u2, pc2);

	// calculate the center of a circle
	center.x = (b1 * u2.y - b2 * u1.y) / cross;
	center.y = (u1.x * b2 - u2.x * b1) / cross;

	return true;
}

inline sf::Vector3f* cross(float * arrA, float * arrB, float* crossArr)
{
	crossArr[0] = arrA[1] * arrB[2] - arrA[2] * arrB[1];
	crossArr[1] = arrA[0] * arrB[2] - arrA[2] * arrB[0];
	crossArr[2] = arrA[0] * arrB[1] - arrA[1] * arrB[0];

	sf::Vector3f* crossDone = new sf::Vector3f{ crossArr[0], crossArr[1], crossArr[2] };
	return crossDone;
}

inline int dotProduct(std::vector<double> vecA, std::vector<double> vecB, int nrOfElements)
{
	double product = 0.0;

	for (int i = 0; i < nrOfElements; i++)
	{
		product = product + vecA[i] * vecB[i];
	}
	return product;
}
#endif