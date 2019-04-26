
#include <SFML/Graphics.hpp>
#include "voronoi.h"
#include <iostream>
#include "Fortunes/Data Structures/BinTree.h"
#include "Bowyer-Watson/Delunay.h"
#include "Bowyer-Watson/delaunay.hpp"
#include "Fortunes/Data Structures/DCEL.h"

Voronoi* vdg;
vector<VoronoiPoint*> ver;
vector<VEdge> edges;

#define CUSTOM 1;

int main()
{

	//FORTUNES ALGORITHM
	for (vector<VoronoiPoint*>::iterator i = ver.begin(); i != ver.end(); i++)
		delete((*i));
	ver.clear();
	edges.clear();
	for (int i = 0; i < 100; i++)
	{
		ver.push_back(new VoronoiPoint(rand() % 390 + 55, rand() % 390 + 55));
	}

	vector<sf::Vector2f*> BWpoints;
	std::vector<delaunay::Point<float>> points;
	for (size_t i = 0; i < 10; i++)
	{
		float x = float(rand() % 390 + 55);
		float y = float(rand() % 390 + 55);
		points.push_back(delaunay::Point<float>(x,y));
		BWpoints.push_back(new sf::Vector2f(x, y));
	}
	
	/*BWpoints.push_back(new sf::Vector2f(50, 50));
	BWpoints.push_back(new sf::Vector2f(400 + 50, 50));
	BWpoints.push_back(new sf::Vector2f(400 + 50, 400 + 50));
	BWpoints.push_back(new sf::Vector2f(50, 400 + 50));*/

#if CUSTOM

	Delunay triangulation;
	const std::vector<Triangle> triangles = triangulation.Triangulate(BWpoints);
	std::cout << triangles.size() << " triangles generated\n";
	std::vector<DEdge> triEdges = triangulation.getEdges();

	std::vector<pair<unsigned int, unsigned int>> EdgeIndex;
	int vert1 = -1;
	int vert2 = -1;
	for (const DEdge &edge : triEdges)
	{
		vert1 = -1;
		vert2 = -1;
		for (int i = 0; i < BWpoints.size(); i++)
		{
			if (vert1 == -1 && edge.v1 == BWpoints[i])
			{
				vert1 = i;
			}
			if (vert2 == -1 && edge.v2 == BWpoints[i])
			{
				vert2 = i;
			}
		}
		EdgeIndex.push_back(make_pair(vert1, vert2));
	}

	map< pair<unsigned int, unsigned int>, HALF_EDGE::HE_Edge* > Edges;
	size_t triangleCount = 0;
	for (int i = 0; i < triangles.size(); i++)
	{
		int offset = i * 3;
		HALF_EDGE::HE_Face* tempF = new HALF_EDGE::HE_Face();
		tempF->circumCenter = sf::Vector2f(triangles[i].circle.x, triangles[i].circle.y);
		for (int k = offset; k < offset + 3; k++)
		{
			Edges[EdgeIndex[k]] = new HALF_EDGE::HE_Edge();
			Edges[EdgeIndex[k]]->face = tempF;
			tempF->edge = Edges[EdgeIndex[k]];
			Edges[EdgeIndex[k]]->vert = BWpoints[EdgeIndex[k].first];
			/*Edges[EdgeIndex[k]]->vert = new HALF_EDGE::HE_Vertex();
			Edges[EdgeIndex[k]]->vert->edge = Edges[EdgeIndex[k]];
			Edges[EdgeIndex[k]]->vert->coords = BWpoints[EdgeIndex[k].first];*/
		}
		for (int k = offset; k < offset + 3; k++)
		{
			Edges[EdgeIndex[k]]->setNext(
				Edges[EdgeIndex[offset + (k + 1) % 3]]);
			
			pair<unsigned int, unsigned int> otherPair = make_pair(EdgeIndex[k].second, EdgeIndex[k].first);
			if (Edges.find(otherPair) != Edges.end())
			{
				Edges[EdgeIndex[k]]->twin = Edges[otherPair];
				Edges[otherPair]->twin = Edges[EdgeIndex[k]];
			}

		}
	}
	int Vmin = 54;
	int Vmax = 445;
	std::vector< pair<sf::Vector2f, sf::Vector2f>> VorEdges;
	HALF_EDGE::HE_Edge* e = Edges.find(EdgeIndex[0])->second;
	std::vector < sf::Vector2f*> faceEdges;
	HALF_EDGE::getFaceVertices(faceEdges, e->face);

	//for (auto &halfedge : Edges)
	//{
	//	if (halfedge.second->twin != nullptr)
	//	{

	//		sf::Vector2f cc1 = halfedge.second->face->circumCenter;
	//		sf::Vector2f cc2 = halfedge.second->twin->face->circumCenter;
	//		bool cc1Inside = (cc1.x < Vmax && cc1.y < Vmax && cc1.x > Vmin && cc1.y > Vmin);
	//		bool cc2Inside = (cc2.x < Vmax && cc2.y < Vmax && cc2.x > Vmin && cc2.y > Vmin);
	//	/*	if (!cc1Inside && !cc2Inside)
	//			continue;

	//		if (!cc1Inside)
	//		{
	//			if (cc1.x < Vmin)
	//				cc1.x = Vmin;
	//			if (cc1.y < Vmin)
	//				cc1.y = Vmin;

	//			if (cc1.x > Vmax)
	//				cc1.x = Vmax;
	//			if (cc1.y > Vmax)
	//				cc1.y = Vmax;
	//		}

	//		if (!cc2Inside)
	//		{
	//			if (cc2.x < Vmin)
	//				cc2.x = Vmin;
	//			if (cc2.y < Vmin)
	//				cc2.y = Vmin;

	//			if (cc2.x > Vmax)
	//				cc2.x = Vmax;
	//			if (cc2.y > Vmax)
	//				cc2.y = Vmax;
	//		}
	//		if(cc1Inside && cc2Inside)*/
	//			VorEdges.push_back(make_pair(cc1, cc2));
	//	}
	//}
	/*std::vector<bool> remove(edges.size(), false);

	for (auto it1 = VorEdges.begin(); it1 != VorEdges.end(); ++it1) {
		for (auto it2 = VorEdges.begin(); it2 != VorEdges.end(); ++it2) {
			if (it1 == it2) {
				continue;
			}
			if (*it1 == *it2) {
				remove[std::distance(VorEdges.begin(), it1)] = true;
				remove[std::distance(VorEdges.begin(), it2)] = true;
			}
		}
	}*/
	//auto is_duplicate = [&](auto const& e) { return remove[&e - &VorEdges[0]]; };
	//erase_where(VorEdges, is_duplicate);


#else
	const auto triangulation = delaunay::triangulate(points);
	std::cout << triangulation.triangles.size() << " triangles generated\n";
#endif

	
	vdg = new Voronoi();
	double minY = 55;
	double maxY = 390 + 55;
	edges = vdg->ComputeVoronoiGraph(ver, minY, maxY);
	delete vdg;
	std::vector<sf::Vertex*> lines;
	std::vector<int> shared;

#if CUSTOM
	int triCounter = 1;
	int triangle = 4;
	size_t index = triangle*3;

	for (auto& tri : triangles)
	{
		int val = (tri.v2->y - tri.v1->y) * (tri.v3->x - tri.v2->x) -
			(tri.v2->x - tri.v1->x) * (tri.v3->y - tri.v2->y);

		if (val == 0) return 0;  // colinear 

		val = (val > 0) ? 1 : 2; // clock or counterclock wise 
		if (val == 1)
		{
			auto lol = 0;
		}

		val = 0;
		if (tri.e1.v2 == tri.e2.v1 && tri.e2.v2 == tri.e3.v1 && tri.e3.v2 == tri.e1.v1)
		{
			val = 1;
		}
		else if (tri.e1.v2 == tri.e3.v1 && tri.e3.v2 == tri.e2.v1 && tri.e2.v2 == tri.e1.v1)
		{
			val = 2;
		}

	}

	


	for (size_t i = 0; i < triEdges.size(); i++)
	{
		sf::Vertex* test = new sf::Vertex[2];
		test[0] = sf::Vector2f((triEdges[i].v1->x), (triEdges[i].v1->y));
		test[1] = sf::Vector2f((triEdges[i].v2->x), (triEdges[i].v2->y));
		lines.push_back(test);
		lines.back()[0].color = sf::Color(0, 0, 255);
		lines.back()[1].color = sf::Color(0, 0, 255);
	}
	for (size_t i = 0; i < faceEdges.size(); i++)
	{

		sf::Vertex* test = new sf::Vertex[2];
		test[0] = sf::Vector2f((faceEdges[i]->x), (faceEdges[i]->y));
		test[1] = sf::Vector2f((faceEdges[(i + 1) % faceEdges.size()]->x), (faceEdges[(i + 1) % faceEdges.size()]->y));
		lines.push_back(test);
		lines.back()[0].color = sf::Color(0, 255, 0);
		lines.back()[1].color = sf::Color(0, 255, 0);
	}

	/*for (size_t i = 0; i < VorEdges.size(); i++)
	{
		sf::Vertex* test = new sf::Vertex[2];
		test[0] = sf::Vector2f((VorEdges[i].first.x), (VorEdges[i].first.y));
		test[1] = sf::Vector2f((VorEdges[i].second.x), (VorEdges[i].second.y));
		lines.push_back(test);
		lines.back()[0].color = sf::Color(255, 0, 0);
		lines.back()[1].color = sf::Color(255, 0, 0);
	}*/
#else
	for (auto const& e : triangulation.edges)
	{
		sf::Vertex* test = new sf::Vertex[2];
		test[0] = sf::Vector2f((e.p0.x), (e.p0.y));
		test[1] = sf::Vector2f((e.p1.x), (e.p1.y));
		lines.push_back(test);
		lines.back()[0].color = sf::Color(0, 0, 255);
		lines.back()[1].color = sf::Color(0, 0, 255);
	}

#endif
	

	//for (int i = 0; i < edges.size(); i++)
	//{
	//	sf::Vertex* test = new sf::Vertex[2];
	//	test[0] = sf::Vector2f((edges[i].VertexA.x), (edges[i].VertexA.y));
	//	test[1] = sf::Vector2f((edges[i].VertexB.x), (edges[i].VertexB.y));
	//	lines.push_back(test);
	//	lines.back()[0].color = sf::Color(0, 255, 0);
	//	lines.back()[1].color = sf::Color(0, 255, 0);
	//}

	sf::Vertex* test = new sf::Vertex[2];
	test[0] = sf::Vector2f(55, 55);
	test[1] = sf::Vector2f(55, 390 + 55);
	lines.push_back(test);

	test = new sf::Vertex[2];
	test[0] = sf::Vector2f(55, 390 + 55);
	test[1] = sf::Vector2f(390 + 55, 390 + 55);
	lines.push_back(test);

	test = new sf::Vertex[2];
	test[0] = sf::Vector2f(390 + 55, 390 + 55);
	test[1] = sf::Vector2f(390 + 55, 55);
	lines.push_back(test);

	test = new sf::Vertex[2];
	test[0] = sf::Vector2f(390 + 55, 55);
	test[1] = sf::Vector2f(55, 55);
	lines.push_back(test);

	sf::RenderWindow window(sf::VideoMode(500, 500), "SFML works!");

	BinTree<int> bT;

	bT.AddNode(bT.GetRoot(), 50); 
	bT.AddNode(bT.GetRoot(), 20); 
	bT.AddNode(bT.GetRoot(), 70); 

	int serachNum = 0;

	/*std::cout << "Please enter the number you want to search for: " << std::endl; 
	std::cin >> serachNum; 

	if (bT.Search(bT.GetRoot(), serachNum))
	{
		std::cout << "Number " << serachNum << " was found!" << std::endl; 
	}
	else
	{
		std::cout << "Number " << serachNum << " was not found!" << std::endl; 
	}*/

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
		for(auto line : lines)
			window.draw(line, 2, sf::Lines);
		window.display();
	}

	return 0; 
}