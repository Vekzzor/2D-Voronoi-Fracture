#include "Delunay.h"



Delunay::Delunay()
{
}


Delunay::~Delunay()
{
}

std::vector<Triangle>& Delunay::Triangulate(std::vector<sf::Vector2f*> points)
{
	_vertices = points;

	// Determinate the super triangle
	float minX = points[0]->x;
	float minY = points[0]->y;
	float maxX = minX;
	float maxY = minY;

	for (std::size_t i = 0; i < points.size(); ++i)
	{
		if (points[i]->x < minX) minX = points[i]->x;
		if (points[i]->y < minY) minY = points[i]->y;
		if (points[i]->x > maxX) maxX = points[i]->x;
		if (points[i]->y > maxY) maxY = points[i]->y;
	}

	const float dx = maxX - minX;
	const float dy = maxY - minY;
	const float deltaMax = std::max(dx, dy);
	const float midx = (minX + maxX)*0.5f;
	const float midy = (minY + maxY)*0.5f;

	const sf::Vector2f p1(midx - 2 * deltaMax, midy - deltaMax);
	const sf::Vector2f p2(midx, midy + 2 * deltaMax);
	const sf::Vector2f p3(midx + 2 * deltaMax, midy - deltaMax);

	// Create a list of triangles, and add the supertriangle in it
	_triangles.push_back(Triangle(p1, p2, p3));


	for (int p = 0; p < points.size(); p++)
	{
		
		//std::cout << "_triangles contains " << _triangles.size() << " elements" << std::endl;
		std::vector<DEdge*> polygon;

		for (auto & t : _triangles)
		{
			//std::cout << "Processing " << std::endl << *t << std::endl;

			if (t.circumCircleContains(*points[p]))
			{
				//std::cout << "Pushing bad triangle " << *t << std::endl;
				t.isBad = true;
				polygon.push_back(t.e1);
				polygon.push_back(t.e2);
				polygon.push_back(t.e3);
			}
			else
			{
				//std::cout << " does not contains " << *p << " in his circum center" << std::endl;
			}
		}

		_triangles.erase(std::remove_if(begin(_triangles), end(_triangles), [](Triangle &t) 
		{
			return t.isBad;
		}), end(_triangles));


		for (int i = 0; i < polygon.size(); i++)
		{
			for (int k = i + 1; k < polygon.size(); k++)
			{
			
				if (almost_equalEdge(*polygon[i], *polygon[k]))
				{
					polygon[i]->isBad = true;
					polygon[k]->isBad = true;
				}
			}
		}

		polygon.erase(std::remove_if(begin(polygon), end(polygon), [](DEdge *e) {
			return e->isBad;
		}), end(polygon));


		for (DEdge* e : polygon)
			_triangles.push_back(Triangle(e->v1, e->v2, points[p]));
	}

	_triangles.erase(std::remove_if(begin(_triangles), end(_triangles), [p1, p2, p3](Triangle &t) {
		return t.containsVertex(p1) || t.containsVertex(p2) || t.containsVertex(p3);
	}), end(_triangles));

	for (const auto t : _triangles)
	{
		_edges.push_back(t.e1);
		_edges.push_back(t.e2);
		_edges.push_back(t.e3);
	}

	return _triangles;
}

Triangle::Triangle(sf::Vector2f * p1, sf::Vector2f * p2, sf::Vector2f * p3)
{
	this->v1 = p1;
	this->v2 = p2;
	this->v3 = p3;
	if (this->IsCounterClockwise(p1, p2, p3))
	{
		e1 = new DEdge(p1, p2);
		e2 = new DEdge(p2, p3);
		e3 = new DEdge(p3, p1);
	}
	else
	{
		e1 = new DEdge(p1, p3);
		e2 = new DEdge(p3, p2);
		e3 = new DEdge(p2, p1);
	}

}

Triangle::Triangle(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3)
{
	this->v1 = new sf::Vector2f(p1);
	this->v2 = new sf::Vector2f(p2);
	this->v3 = new sf::Vector2f(p3);

	if (this->IsCounterClockwise(p1, p2, p3))
	{
		e1 = new DEdge(p1, p2);
		e2 = new DEdge(p2, p3);
		e3 = new DEdge(p3, p1);
	}
	else
	{
		e1 = new DEdge(p1, p3);
		e2 = new DEdge(p3, p2);
		e3 = new DEdge(p2, p1);
	}
}

void Triangle::UpdateCircumCircle()
{
	sf::Vector2f* p0 = this->e1->v1;
	sf::Vector2f* p1 = this->e2->v1;
	sf::Vector2f* p2 = this->e3->v1;
	float dA = p0->x * p0->x + p0->y * p0->y;
	float dB = p1->x * p1->x + p1->y * p1->y;
	float dC = p2->x * p2->x + p2->y * p2->y;

	float aux1 = (dA * (p2->y - p1->y) + dB * (p0->y - p2->y) + dC * (p1->y - p0->y));
	float aux2 = -(dA * (p2->x - p1->x) + dB * (p0->x - p2->x) + dC * (p1->x - p0->x));
	float div = (2 * (p0->x * (p2->y - p1->y) + p1->x * (p0->y - p2->y) + p2->x * (p1->y - p0->y)));

	if (div == 0)
	{
		return;
	}

	sf::Vector2f center(aux1 / div, aux2 / div);
	Circumcenter = center;
	RadiusSquared = (center.x - p0->x) * (center.x - p0->x) + (center.y - p0->y) * (center.y - p0->y);
}

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

bool almost_equal(float x, float y, int ulp)
{
	// the machine epsilon has to be scaled to the magnitude of the values used
	// and multiplied by the desired precision in ULPs (units in the last place)
	return std::abs(x - y) <= std::numeric_limits<float>::epsilon() * std::abs(x + y) * ulp
		// unless the result is subnormal
		|| std::abs(x - y) < std::numeric_limits<float>::min();
}

bool almost_equalVec(const sf::Vector2f & v1, const sf::Vector2f & v2, int ulp)
{
	return almost_equal(v1.x, v2.x, ulp) && almost_equal(v1.y, v2.y, ulp);
}

bool almost_equalEdge(const DEdge & e1, const DEdge & e2)
{
	return	(almost_equalVec(*e1.v1, *e2.v1) && almost_equalVec(*e1.v2, *e2.v2)) ||
		(almost_equalVec(*e1.v1, *e2.v2) && almost_equalVec(*e1.v2, *e2.v1));
}