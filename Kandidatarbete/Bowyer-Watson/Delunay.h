#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <limits>


constexpr double eps = 1e-4;
struct DVertex;
class DEdge
{
public:
	DEdge(DVertex* const& _v1, DVertex* const& _v2) : v1{ _v1 }, v2{ _v2 } {}
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
	DVertex* v1;
	DVertex* v2;
private:
	//int refCount = 0;
};

struct DCircle {
	float x, y, radius;
	DCircle() = default;
};

struct DVertex : public sf::Vector2f {
	DVertex(int x, int y, int index = -1) : sf::Vector2f(x,y)
	{
		arrayIndex = index;
	}
	DVertex(sf::Vector2f vector, int index = -1) : sf::Vector2f(vector)
	{
		arrayIndex = index;
	}
	sf::Vector2f getCoordinates()
	{
		return sf::Vector2f(x, y);
	}
	int arrayIndex = -1;
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
	DVertex* v1;
	DVertex* v2;
	DVertex* v3;
	DCircle circle;
	Triangle(DVertex* _v1, DVertex* _v2, DVertex* _v3)
	  : v1{ _v1 },
		v2{ _v2 },
		v3{ _v3 },
		e{ {_v2, _v1}, { _v3, _v2 }, { _v1, _v3 } },
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
		//sf::Vector2f test = CalculateCircleCenter();
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

		float ma = (v2->y - v1->y) / (v2->x - v1->x);
		float mb = (v3->y - v2->y) / (v3->x - v2->x);

		center.x = (ma * mb * (v1->y - v3->y) + mb * (v1->x + v2->x) - ma * (v2->x + v3->x)) / (2 * (mb - ma));

		center.y = (-1 / ma) * (center.x - (v1->x + v2->x) / 2) + (v1->y + v2->y) / 2;

		return center;
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
		float result = (v2->x - v1->x) * (v3->y - v1->y) -
			(v3->x - v1->x) * (v2->y - v1->y);
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
	std::vector<DVertex*> _vertices;
	std::vector<DVertex*> superTriangle;
public:
	Delunay();
	~Delunay();

	std::vector<Triangle>& Triangulate(std::vector<DVertex*>& points);

	const std::vector<Triangle>& getTriangles() const { return _triangles; };
	const std::vector<DEdge>& getEdges() const { return _edges; };
	const std::vector<DVertex*>& getVertices() const { return _vertices; };
	const std::vector<DVertex*>& getSuperTriangle() const { return superTriangle; };
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