
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
		ver.push_back(new VoronoiPoint(rand() % 500, rand() % 500));
	}

	vector<sf::Vector2f> BWpoints;
	std::vector<delaunay::Point<float>> points;
	for (size_t i = 0; i < 6; i++)
	{
		float x = float(rand() % 390 + 55);
		float y = float(rand() % 390 + 55);
		points.push_back(delaunay::Point<float>(x,y));
		BWpoints.push_back(sf::Vector2f(x, y));
	}
	
	//BWpoints.push_back(sf::Vector2f(50, 50));
	//BWpoints.push_back(sf::Vector2f(400 + 50, 50));
	//BWpoints.push_back(sf::Vector2f(400 + 50, 400 + 50));
	//BWpoints.push_back(sf::Vector2f(50, 400 + 50));

#if CUSTOM

	Delunay triangulation;
	const std::vector<Triangle> triangles = triangulation.Triangulate(BWpoints);
	std::cout << triangles.size() << " triangles generated\n";
	std::vector<DEdge> triEdges = triangulation.getEdges();
#else
	const auto triangulation = delaunay::triangulate(points);
	std::cout << triangulation.triangles.size() << " triangles generated\n";
#endif

	/*map< sf::Vector2f, DCEL::Half_edge* > Edges;
	for (auto& triangle : triangles)
	{
		  Edges[triangle.v1] = new DCEL::Half_edge();
		  Edges[triangle.v1]->left = new DCEL::Face();
		  Edges[triangle.v1]->left->rep = Edges[triangle.v1];
	   for each edge(u,v) of F
	   {
		  set Edges[pair(u,v)]->nextHalfEdge to next half - edge in F
		  if (Edges.find(pair(v,u)) != Edges.end())
		  {
			 Edges[pair(u,v)]->oppositeHalfEdge = Edges[pair(v,u)];
			 Edges[pair(v,u)]->oppositeHalfEdge = Edges[pair(u,v)];
		   }
		}
	}*/
	
	//map< pair<unsigned int, unsigned int>, HALF_EDGE::HE_Edge* > Edges;
	//for (auto triangle : triangles)
	//{
	//	HALF_EDGE::HE_Face* tempF = new HALF_EDGE::HE_Face();
	//	Edges[make_pair(triangle.e1.v1, triangle.e1.v2)] = new HALF_EDGE::HE_Edge();
	//	Edges[make_pair(triangle.e1.v1, triangle.e1.v2)]->face =tempF;
	//	
	//	/*			   
	//	Edges[make_pair(triangle.e2.v1, triangle.e2.v2)] = new HALF_EDGE::HE_Edge();
	//	Edges[make_pair(triangle.e2.v1, triangle.e2.v2)]->face =tempF;

	//	Edges[make_pair(triangle.e3.v1, triangle.e3.v2)] = new HALF_EDGE::HE_Edge();
	//	Edges[make_pair(triangle.e3.v1, triangle.e3.v2)]->face =tempF;*/

	//	//Find Twins
	//	//e1
	//	Edges[make_pair(triangle.e1.v1, triangle.e1.v2)]->next = 
	//		Edges[make_pair(triangle.e2.v1, triangle.e2.v2)];
	//	if (Edges.find(make_pair(triangle.e1.v2, triangle.e1.v1)) != Edges.end())
	//	{
	//		Edges[make_pair(triangle.e1.v1, triangle.e1.v2)]->twin = Edges[make_pair(triangle.e1.v2, triangle.e1.v1)];
	//		Edges[make_pair(triangle.e1.v2, triangle.e1.v1)]->twin = Edges[make_pair(triangle.e1.v1, triangle.e1.v2)];
	//	}

	//	//e2
	//	Edges[make_pair(triangle.e2.v1, triangle.e2.v2)]->next =
	//		Edges[make_pair(triangle.e3.v1, triangle.e3.v2)];
	//	if (Edges.find(make_pair(triangle.e2.v2, triangle.e2.v1)) != Edges.end())
	//	{
	//		Edges[make_pair(triangle.e2.v1, triangle.e2.v2)]->twin = Edges[make_pair(triangle.e2.v2, triangle.e2.v1)];
	//		Edges[make_pair(triangle.e2.v2, triangle.e2.v1)]->twin = Edges[make_pair(triangle.e2.v1, triangle.e2.v2)];
	//	}

	//	//e3
	//	Edges[make_pair(triangle.e3.v1, triangle.e3.v2)]->next =
	//		Edges[make_pair(triangle.e1.v1, triangle.e1.v2)];
	//	if (Edges.find(make_pair(triangle.e3.v2, triangle.e3.v1)) != Edges.end())
	//	{
	//		Edges[make_pair(triangle.e3.v1, triangle.e3.v2)]->twin = Edges[make_pair(triangle.e3.v2, triangle.e3.v1)];
	//		Edges[make_pair(triangle.e3.v2, triangle.e3.v1)]->twin = Edges[make_pair(triangle.e3.v1, triangle.e3.v2)];
	//	}

	//}

	vdg = new Voronoi();
	double minY = 0;
	double maxY = 500;
	edges = vdg->ComputeVoronoiGraph(ver, minY, maxY);
	delete vdg;
	std::vector<sf::Vertex*> lines;

#if CUSTOM

	for (size_t i = 0; i < triEdges.size(); i++)
	{
		sf::Vertex* test = new sf::Vertex[2];
		test[0] = sf::Vector2f((triEdges[i].v1.x), (triEdges[i].v1.y));
		test[1] = sf::Vector2f((triEdges[i].v2.x), (triEdges[i].v2.y));
		lines.push_back(test);
	}
#else
	for (auto const& e : triangulation.edges)
	{
		sf::Vertex* test = new sf::Vertex[2];
		test[0] = sf::Vector2f((e.p0.x), (e.p0.y));
		test[1] = sf::Vector2f((e.p1.x), (e.p1.y));
		lines.push_back(test);
	}

#endif
	

	/*for (int i = 0; i < triangles.size(); i++)
	{
		sf::Vertex* test = new sf::Vertex[3];
		test[0] = sf::Vector2f((triangles[i].v1->x), (triangles[i].v1->y));
		test[1] = sf::Vector2f((triangles[i].v2->x), (triangles[i].v2->y));
		test[2] = sf::Vector2f((triangles[i].v2->x), (triangles[i].v2->y));
		lines.push_back(test);
	}*/

	/*for (int i = 0; i < edges.size(); i++)
	{
		sf::Vertex* test = new sf::Vertex[2];
		test[0] = sf::Vector2f((edges[i].VertexA.x), (edges[i].VertexA.y));
		test[1] = sf::Vector2f((edges[i].VertexB.x), (edges[i].VertexB.y));
		lines.push_back(test);
	}*/

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