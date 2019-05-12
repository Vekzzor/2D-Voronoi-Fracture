
#include <SFML/Graphics.hpp>
#include "voronoi.h"
#include <iostream>
//#include "Fortunes/Data Structures/BinTree.h"
#include <vector>
//#include "Fortunes/Fortunes.h"

#include "FortuneAlgo/Types/Point2D.h"
#include "FortuneAlgo/Voronoi/VoronoiDiagram.hpp"
#include "FortuneAlgo/Datastruct/Beachline.hpp"

Voronoi* vdg;
vector<VoronoiPoint*> ver;
vector<VEdge> edges;

double random_number() {
	return double(rand() % 200 +150);
}

void initEdgePointsVis(bl::HalfEdgePtr h, std::vector<double> &x, std::vector<double> &y,
	const std::vector<Point2D> &points) {

	if (h->vertex != nullptr && h->twin->vertex != nullptr) {

		x[0] = h->vertex->point.x;
		x[1] = h->twin->vertex->point.x;

		y[0] = h->vertex->point.y;
		y[1] = h->twin->vertex->point.y;

	}
	else if (h->vertex != nullptr) {

		x[0] = h->vertex->point.x;
		y[0] = h->vertex->point.y;

		Point2D norm = (points[h->l_index] - points[h->r_index]).normalized().getRotated90CCW();
		x[1] = x[0] + norm.x * 1000;
		y[1] = y[0] + norm.y * 1000;

	}
	else if (h->twin->vertex != nullptr) {

		x[0] = h->twin->vertex->point.x;
		y[0] = h->twin->vertex->point.y;

		Point2D norm = (points[h->twin->l_index] - points[h->twin->r_index]).normalized().getRotated90CCW();
		x[1] = x[0] + norm.x * 1000;
		y[1] = y[0] + norm.y * 1000;

	}
	else {

		Point2D p1 = points[h->l_index], p2 = points[h->r_index];

		Point2D norm = (p1 - p2).normalized().getRotated90CCW();
		Point2D c = 0.5 * (p1 + p2);

		x[0] = c.x + norm.x * 1000;
		x[1] = c.x - norm.x * 1000;

		y[0] = c.y + norm.y * 1000;
		y[1] = c.y - norm.y * 1000;
	}
}

std::vector<Point2D> randomPoint(int number) {
	srand(static_cast<unsigned int>(time(0)));
	std::vector<Point2D> points;
	for (int i = 0; i < number; ++i) {
		double x = random_number(), y = random_number();
		points.push_back(Point2D(x, y));
	}
	std::sort(points.begin(), points.end(), [](const Point2D &p1, const Point2D &p2) {
		return (fabs(p1.y - p2.y) < POINT_EPSILON && p1.x < p2.x) || (fabs(p1.y - p2.y) >= POINT_EPSILON && p1.y < p1.y);
	});
	for (int i = 1; i < number; ++i) {
		if ((points[i - 1] - points[i]).norm() < POINT_EPSILON) {
			points[i - 1].x = random_number();
		}
	}
	for (int i = number - 1; i >= 0; --i) {
		int j = rand() % (i + 1);
		std::swap(points[i], points[j]);
	}
	return points;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(500, 500), "SFML works!");
	//FORTUNES ALGORITHM


	std::vector<Point2D> points = randomPoint(100);
	//std::vector<Point2D> points = readPoints("/Users/dkotsur/Projects/KNU/FortuneAlgo/Data/fail_1.txt");

	std::vector<bl::HalfEdgePtr> halfedges, faces;
	std::vector<bl::VertexPtr> vertices;

	for (size_t i = 0; i < points.size(); ++i) {
		std::vector<double> _x, _y;
		_x.push_back(points[i].x); _y.push_back(points[i].y);
	}

	// Construct Voronoi diagram
	build_voronoi(points, halfedges, vertices, faces);

	std::vector<sf::Vertex*> lines;
	for (size_t i = 0; i < halfedges.size(); ++i) {
		bl::HalfEdgePtr h = halfedges[i];

		std::vector<double> x(2, 0.0), y(2, 0.0);
		initEdgePointsVis(h, x, y, points);
		sf::Vertex* test = new sf::Vertex[2];
		test[0] = sf::Vector2f(float(x[0]), float(y[0]));
		test[1] = sf::Vector2f(float(x[1]), float(y[1]));
		lines.push_back(test);
	}


	//bl::HalfEdgePtr he_end = faces[1], he = he_end;
	//for (auto& he : faces)
	//{
	//	bl::HalfEdgePtr he_end = he;
	//	if (he != nullptr) {
	//		do {
	//			sf::Vertex* test = new sf::Vertex[2];
	//			std::vector<double> x(2, 0.0), y(2, 0.0);
	//			initEdgePointsVis(he, x, y, points);
	//			test[0] = sf::Vector2f((he->vertex0()->x()), (he->vertex0()->y()));
	//			test[1] = sf::Vector2f((he->vertex1()->x()), (he->vertex1()->y()));
	//			lines.push_back(test);
	//			he = he->next;
	//		} while (he != nullptr && he != he_end);
	//	}
	//}

	/*for (int i = 0; i < halfedges.size(); i++)
	{
		sf::Vertex* test = new sf::Vertex[2];
		if (halfedges[i]->vertex0()->point != nullptr && halfedges[i]->vertex1()->point != nullptr)
		{
			test[0] = sf::Vector2f((halfedges[i]->vertex0()->x()), (halfedges[i]->vertex0()->y()));
			test[1] = sf::Vector2f((halfedges[i]->vertex1()->x()), (halfedges[i]->vertex1()->y()));
			lines.push_back(test);
		}
	}
*/
	//sf::ConvexShape shape; 
	//shape.setPointCount(20); 
	//shape.setTextureRect(sf::IntRect(0, 0, 10, 10)); 
	//Fortunes testFortune(shape,10); 

	//sf::Vector2f center = sf::Vector2f(0, 0);
	//testFortune.findCircleCenter(sf::Vector2f(34, 33), sf::Vector2f(3, 5), sf::Vector2f(35,12), center); 

	//std::cout << center.x << std::endl; 
	//std::cout << center.y << std::endl; 

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
		for(auto& line : lines)
			window.draw(line, 2, sf::Lines);
		window.display();

/*
		float arrA[] = { 3,-5,0 };
		float  arrB[] = { 2,6,0 };
		float crossArr[] = { 0,0,0 }; 

		std::cout << "Current vectors: " << std::endl; 
		for (int i = 0; i < 3; i++)
		{
			std::cout << "arrA[" << i << "] : " << arrA[i] << std::endl; 
			std::cout << "arrB[" << i << "] : " << arrB[i] << std::endl;
		}
		system("PAUSE"); 

		sf::Vector3f* crossDone; 
		crossDone = cross(arrA, arrB, crossArr);
		
		std::cout << "Result from the cross:" << std::endl; 
		for (int i = 0; i < 3; i++)
		{
			std::cout << "X: " << crossDone->x << std::endl; 
			std::cout << "Y: " << crossDone->y << std::endl;
			std::cout << "Z: " << crossDone->z << std::endl;
		}
		delete crossDone; */
	}
	
	return 0; 
}