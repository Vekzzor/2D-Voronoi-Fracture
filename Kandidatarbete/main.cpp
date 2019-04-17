
#include <SFML/Graphics.hpp>
#include "voronoi.h"
#include <chrono>
#include <limits>
Voronoi* vdg;
vector<VoronoiPoint*> ver;
vector<VEdge> edges;

#include <iostream> 
#include <stack> 
#include <stdlib.h> 
struct VertexPoint : sf::Vector2f
{
	VertexPoint() : sf::Vector2f(){}

	VertexPoint(const sf::Vector2f& point) : sf::Vector2f(point)
	{
		//this->point = new sf::Vector2f(point);
		//connectedPoints = new VertexPoint*[2];
	}
	~VertexPoint()
	{
		/*if (connectedPoints != nullptr);
			delete connectedPoints;
		if (point != nullptr);
			delete point;*/
	}
	void addConnection(VertexPoint* point)
	{
		if (nrConnectedPoints < 2)
		{
			connectedPoints[nrConnectedPoints] = point;
			nrConnectedPoints++;
		}
	}
	//sf::Vector2f* point = nullptr;
	int nrConnectedPoints = 0;
	VertexPoint** connectedPoints;
};

namespace ConvexHull
{
	using namespace std;

	// A global point needed for  sorting points with reference 
	// to  the first point Used in compare function of qsort() 
	VertexPoint p0;

	// A utility function to find next to top in a stack 
	VertexPoint nextToTop(stack<VertexPoint> &S)
	{
		VertexPoint p = S.top();
		S.pop();
		VertexPoint res = S.top();
		S.push(p);
		return res;
	}

	// A utility function to swap two points 
	void swap(VertexPoint &p1, VertexPoint &p2)
	{
		VertexPoint temp = p1;
		p1 = p2;
		p2 = temp;
	}

	// A utility function to return square of distance 
	// between p1 and p2 
	int distSq(VertexPoint p1, VertexPoint p2)
	{
		return (p1.x - p2.x)*(p1.x - p2.x) +
			(p1.y - p2.y)*(p1.y - p2.y);
	}

	// To find orientation of ordered triplet (p, q, r). 
	// The function returns following values 
	// 0 --> p, q and r are colinear 
	// 1 --> Clockwise 
	// 2 --> Counterclockwise 
	int orientation(VertexPoint p, VertexPoint q, VertexPoint r)
	{
		int val = (q.y - p.y) * (r.x - q.x) -
			(q.x - p.x) * (r.y - q.y);

		if (val == 0) return 0;  // colinear 
		return (val > 0) ? 1 : 2; // clock or counterclock wise 
	}

	// A function used by library function qsort() to sort an array of 
	// points with respect to the first point 
	int compare(const void *vp1, const void *vp2)
	{
		VertexPoint *p1 = (VertexPoint *)vp1;
		VertexPoint *p2 = (VertexPoint *)vp2;

		// Find orientation 
		int o = orientation(p0, *p1, *p2);
		if (o == 0)
			return (distSq(p0, *p2) >= distSq(p0, *p1)) ? -1 : 1;

		return (o == 2) ? -1 : 1;
	}

	// Prints convex hull of a set of n points. 
	void convexHull(std::vector<VertexPoint>& points, int n)
	{
		// Find the bottommost point 
		int ymin = points[0].y, min = 0;
		for (int i = 1; i < n; i++)
		{
			int y = points[i].y;

			// Pick the bottom-most or chose the left 
			// most point in case of tie 
			if ((y < ymin) || (ymin == y &&
				points[i].x < points[min].x))
				ymin = points[i].y, min = i;
		}

		// Place the bottom-most point at first position 
		swap(points[0], points[min]);

		// Sort n-1 points with respect to the first point. 
		// A point p1 comes before p2 in sorted ouput if p2 
		// has larger polar angle (in counterclockwise 
		// direction) than p1 
		p0 = points[0];
		qsort(&points[1], n - 1, sizeof(VertexPoint), compare);

		// If two or more points make same angle with p0, 
		// Remove all but the one that is farthest from p0 
		// Remember that, in above sorting, our criteria was 
		// to keep the farthest point at the end when more than 
		// one points have same angle. 
		int m = 1; // Initialize size of modified array 
		for (int i = 1; i < n; i++)
		{
			// Keep removing i while angle of i and i+1 is same 
			// with respect to p0 
			while (i < n - 1 && orientation(p0, points[i],
				points[i + 1]) == 0)
				i++;


			points[m] = points[i];
			m++;  // Update size of modified array 
		}

		// If modified array of points has less than 3 points, 
		// convex hull is not possible 
		if (m < 3) return;

		// Create an empty stack and push first three points 
		// to it. 
		stack<VertexPoint> S;
		S.push(points[0]);
		S.push(points[1]);
		S.push(points[2]);

		// Process remaining n-3 points 
		for (int i = 3; i < m; i++)
		{
			// Keep removing top while the angle formed by 
			// points next-to-top, top, and points[i] makes 
			// a non-left turn 
			while (orientation(nextToTop(S), S.top(), points[i]) != 2)
				S.pop();
			S.push(points[i]);
		}

		// Now stack has the output points, print contents of stack 
		points.clear();
		while (!S.empty())
		{
			VertexPoint p = S.top();
			//cout << "(" << p.x << ", " << p.y << ")" << endl;
			points.push_back(p);
			S.pop();
		}
	}
}



class Polygon : public sf::Drawable, public sf::Transformable
{
public:
	void append(VertexPoint point)
	{
		m_pointList.push_back(point);
	}
	void addSite(sf::Vertex site)
	{
		if (siteEmpty)
		{
			m_site = site;
			siteEmpty = false;
		}
	}
	void addTexture(sf::Texture texture)
	{
		this->m_texture = texture;
	}
	std::vector<sf::Vertex> m_vertices;
	std::vector<VertexPoint> m_pointList;
	std::vector<std::tuple<sf::Vector2f*, sf::Vector2f*>> m_connections;
	sf::Vertex m_center;
	bool siteEmpty = true;
	sf::Vertex m_site;
	sf::PrimitiveType m_type = sf::TriangleFan;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the entity's transform -- combine it with the one that was passed by the caller
		states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

		// apply the texture
		//states.texture = &m_texture;

		// you may also override states.shader or states.blendMode if you want
		// draw the vertex array
		target.draw(m_vertices.data(), m_vertices.size(), m_type, states);
		target.draw(&m_center, 1, sf::Points, states);
		//target.draw(&m_site, 1, sf::Points, states);
	}
	sf::Transform m_transformation;
	sf::Texture m_texture;
};

sf::Vector2f normalize(sf::Vector2f vector);
sf::Vector2f* lineIntersection(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4);
bool clockwiseLessThan(sf::Vector2f a, sf::Vector2f b, sf::Vector2f center);

int main()
{	
	//FORTUNES ALGORITHM
	for (vector<VoronoiPoint*>::iterator i = ver.begin(); i != ver.end(); i++)
		delete((*i));
	ver.clear();
	edges.clear();
	const double minY = 200;
	const double maxY = 800;

	for (int i = 0; i < 100; i++)
	{
		ver.push_back(new VoronoiPoint(rand() % (int)(maxY-minY)+ minY, rand() % (int)(maxY - minY) + minY));
	}
	auto start = std::chrono::system_clock::now();

	vdg = new Voronoi();
	edges = vdg->ComputeVoronoiGraph(ver, minY, maxY);

	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

	delete vdg;
	
	std::vector<Polygon> polygons;
	polygons.resize(ver.size());

	//Collects points from Voronoi Tesselation
	for (int i = 0; i < edges.size(); i++)
	{
		VertexPoint point1(sf::Vector2f(edges[i].VertexA.x, edges[i].VertexA.y));
		VertexPoint point2(sf::Vector2f(edges[i].VertexB.x, edges[i].VertexB.y));
		for (int k = 0; k < polygons.size(); k++)
		{
			VoronoiPoint* site = ver[k];
			if ((edges[i].Left_Site.x == site->x && edges[i].Left_Site.y == site->y) || edges[i].Right_Site.x == site->x && edges[i].Right_Site.y == site->y)
			{
				polygons[k].append(point1);
				polygons[k].append(point2);
				polygons[k].addSite(sf::Vertex(sf::Vector2f(site->x, site->y), sf::Color(150, 50, 250)));
			}
		}
	}

	for (auto &polygon : polygons)
	{
		//Generate a convex hull 
		ConvexHull::convexHull(polygon.m_pointList, polygon.m_pointList.size());
		const sf::Color polyColor(rand() % 200 + 50, rand() % 200 + 55, rand() % 200 + 50);
		for (auto &point : polygon.m_pointList)
		{
			polygon.m_vertices.push_back(sf::Vertex(point, polyColor));
		}
		polygon.m_vertices.push_back(polygon.m_vertices[0]);
	
		//Find the center and set that as origin for drawable
		{
			float xCenter = 0;
			float yCenter = 0;
			int k = 0;
			for (k = 0; k < polygon.m_vertices.size(); k++)
			{
				xCenter += polygon.m_vertices[k].position.x;
				yCenter += polygon.m_vertices[k].position.y;
			}
			xCenter /= k;
			yCenter /= k;
			polygon.m_center = sf::Vertex(sf::Vector2f(xCenter, yCenter));

			polygon.setOrigin(polygon.m_center.position);
			polygon.setPosition(polygon.m_center.position);
		}
		polygon.m_vertices.insert(polygon.m_vertices.begin(),polygon.m_center);
	}

	int offset = 1;
	Polygon rectangle;
	{
		rectangle.m_vertices.push_back(sf::Vertex(sf::Vector2f(minY+ offset, minY+ offset)));
		
		rectangle.m_vertices.push_back(sf::Vertex(sf::Vector2f(minY+ offset, maxY- offset)));

		rectangle.m_vertices.push_back(sf::Vertex(sf::Vector2f(maxY- offset, maxY- offset)));

		rectangle.m_vertices.push_back(sf::Vertex(sf::Vector2f(maxY- offset, minY+ offset)));

		rectangle.m_vertices.push_back(sf::Vertex(sf::Vector2f(minY+ offset, minY+ offset)));
	}


	//Calculate the direction to move away from center
	std::vector<sf::Vector2f> distances;
	for (auto &polygon: polygons)
	{
		
		distances.push_back(normalize(polygon.m_site.position - sf::Vector2f(maxY-minY, maxY - minY)));
	}

	sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Escape)
						window.close();
					break;
			}

		}

		window.clear();
		//window.draw(rectangle);
		for (int i = 0; i < polygons.size(); i++)
		{
			polygons[i].move(distances[i] * 0.01f);
			window.draw(polygons[i]);
		}
		window.display();
	}

	return 0; 
}

sf::Vector2f* lineIntersection(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4)
{
	double epsilon = std::numeric_limits<double>::epsilon();
	// Store the values for fast access and easy
	// equations-to-code conversion
	float x1 = p1.x, x2 = p2.x, x3 = p3.x, x4 = p4.x;
	float y1 = p1.y, y2 = p2.y, y3 = p3.y, y4 = p4.y;

	float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	// If d is zero, there is no intersection
	if (d == 0) return NULL;

	// Get the x and y
	float pre = (x1*y2 - y1 * x2), post = (x3*y4 - y3 * x4);
	float x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
	float y = (pre * (y3 - y4) - (y1 - y2) * post) / d;
	
	// Check if the x and y coordinates are within both lines
	if (x < (min(x1, x2) - epsilon) || x >(max(x1, x2) + epsilon) ||
		x < (min(x3, x4) - epsilon) || x >(max(x3, x4) + epsilon))
		return NULL;
	if (y < (min(y1, y2) - epsilon) || y >(max(y1, y2) + epsilon) ||
		y < (min(y3, y4) - epsilon) || y >(max(y3, y4) + epsilon))
		return NULL;

	// Return the point of intersection
	sf::Vector2f* ret = new sf::Vector2f();
	ret->x = x;
	ret->y = y;
	return ret;
}

bool clockwiseLessThan(sf::Vector2f a, sf::Vector2f b, sf::Vector2f center)
{
	if (a.x - center.x >= 0 && b.x - center.x < 0)
		return true;
	if (a.x - center.x < 0 && b.x - center.x >= 0)
		return false;
	if (a.x - center.x == 0 && b.x - center.x == 0) {
		if (a.y - center.y >= 0 || b.y - center.y >= 0)
			return a.y > b.y;
		return b.y > a.y;
	}

	// compute the cross product of vectors (center -> a) x (center -> b)
	int det = (a.x - center.x) * (b.y - center.y) - (b.x - center.x) * (a.y - center.y);
	if (det < 0)
		return true;
	if (det > 0)
		return false;

	// points a and b are on the same line from the center
	// check which point is closer to the center
	int d1 = (a.x - center.x) * (a.x - center.x) + (a.y - center.y) * (a.y - center.y);
	int d2 = (b.x - center.x) * (b.x - center.x) + (b.y - center.y) * (b.y - center.y);
	return d1 > d2;
}

sf::Vector2f normalize(sf::Vector2f vector)
{
	float mx = vector.x;
	float my = vector.y;

	float multiplier = 1.0f / sqrt(mx*mx + my * my);
	mx *= multiplier;
	my *= multiplier;

	return sf::Vector2f(mx, my);
}