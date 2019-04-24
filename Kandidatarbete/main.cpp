
#include <SFML/Graphics.hpp>
#include "voronoi.h"
#include <iostream>
#include "Fortunes/Data Structures/BinTree.h"
#include "Bowyer-Watson/Delunay.h"


Voronoi* vdg;
vector<VoronoiPoint*> ver;
vector<VEdge> edges;
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

	vector<sf::Vector2f*> BWpoints;
	for (int i = 0; i < 6; i++)
	{
		BWpoints.push_back(new sf::Vector2f(rand() % 400 + 50, rand() % 400 + 50));
	}
	/*BWpoints.push_back(new sf::Vector2f(50, 50));
	BWpoints.push_back(new sf::Vector2f(400 + 50,50));
	BWpoints.push_back(new sf::Vector2f(50, 400 + 50));
	BWpoints.push_back(new sf::Vector2f(400 + 50, 400 + 50));*/

	Delunay triangulation;
	const std::vector<Triangle> triangles = triangulation.Triangulate(BWpoints);
	std::cout << triangles.size() << " triangles generated\n";
	std::vector<DEdge*> triEdges = triangulation.getEdges();

	vdg = new Voronoi();
	double minY = 0;
	double maxY = 500;
	edges = vdg->ComputeVoronoiGraph(ver, minY, maxY);
	delete vdg;
	std::vector<sf::Vertex*> lines;

	for (int i = 0; i < triEdges.size(); i++)
	{
		sf::Vertex* test = new sf::Vertex[2];
		test[0] = sf::Vector2f((triEdges[i]->v1->x), (triEdges[i]->v1->y));
		test[1] = sf::Vector2f((triEdges[i]->v2->x), (triEdges[i]->v2->y));
		lines.push_back(test);
	}

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